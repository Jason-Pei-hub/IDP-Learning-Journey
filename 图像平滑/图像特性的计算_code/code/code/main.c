#define _CRT_SECURE_NO_WARNINGS 1

#include "IDP.h"

int main()
{
    int width, height;//图像的宽和高

    // 读取图像
    const char* img = "input/test.bmp";

    unsigned char* imageData = readColorBMP(img,&width,&height);//读取24位彩色图像的BMP文件

    //[这里我们读入灰度值图片，所以需要将彩色图像转化为灰度图像]
    unsigned char* grayImage = (unsigned char*)malloc(width*height*1);//为灰度值图片分配内存
    convertToGray(imageData,grayImage,width,height);//24位彩色图像转8位灰度值
    
    //[计算图片的统计特征]
    int histogram[256] = {0};//创建灰度值直方图
    GetHistogram(grayImage,width,height,histogram);//统计图像灰度值

    double bright, contrast;//亮度和对比度
    GetBrightContrast(histogram,&bright,&contrast);//亮度和对比度
    double mean = Median( histogram, width,height);//求中间值

    int size = sizeof(histogram)/sizeof(histogram[0]);//直方图的大小
    displayHistogram(histogram,size,bright,contrast,mean);//显示直方图

    printf("Brightness: %.2f\n", bright);
    printf("Contrast: %.2f\n", contrast);
    printf("Median: %.2f\n", mean);

    printf("正在生成预处理后的图片...\n");

    //[生成预处理后的图片]
    printf("正在进行图像增强...\n");
    printf("###########################################\n");

    //[图像增强]
    unsigned char* grayImage1 = (unsigned char*)malloc(width * height * 1);//为灰度值备份图片分配内存
    memcpy(grayImage1, grayImage, width * height * 1);//备份原图
    RmwHistogramEqualize(grayImage1, width, height);//直方图均衡化
    saveGrayScaleBMP("output/直方图均衡化.bmp", grayImage1,width,height);
    printf("已完成图像增强的直方图均衡化\n");

    unsigned char* grayImage2 = (unsigned char*)malloc(width * height * 1);//为灰度值备份图片分配内存
    memcpy(grayImage2, grayImage, width * height * 1);//备份原图
    LinearStretchDemo(grayImage2,width,height,3,-60);
    saveGrayScaleBMP("output/线性拉伸.bmp", grayImage2,width,height);
    printf("已完成图像增强的线性拉伸\n");

    unsigned char* grayImage3 = (unsigned char*)malloc(width * height * 1);//为灰度值备份图片分配内存
    memcpy(grayImage3, grayImage, width * height * 1);//备份原图
    RmwLogTransform(grayImage3,width,height);//对数变换
    saveGrayScaleBMP("output/对数变换.bmp", grayImage3,width,height);
    printf("已完成图像增强的对数变换\n");

    //[图像滤波]
    printf("正在进行图像滤波...\n");
    printf("###########################################\n");

    unsigned char* grayImage4 = (unsigned char*)malloc(width * height * 1);//为灰度值备份图片分配内存
    memcpy(grayImage4, grayImage, width * height * 1);//备份原图
    int* pSumImg = (int*)malloc(width * height * sizeof(int));//申请积分图的内存
    RmwDoSumGryImg(grayImage4,width,height,pSumImg);//基于列积分的积分图实现
    RmwAvrFilterBySumCol(grayImage,width,height,3,3, grayImage4);//基于列积分的快速均值滤波
    saveGrayScaleBMP("output/基于列积分的快速滤波.bmp", grayImage4,width,height);
    printf("已完成图像滤波的基于列积分的快速滤波\n");

    unsigned char* grayImage5 = (unsigned char*)malloc(width * height * 1);//为灰度值备份图片分配内存
    RmwMeanFilter(grayImage,width,height,3,3, grayImage5);//均值滤波
    saveGrayScaleBMP("output/均值滤波.bmp", grayImage5,width,height);
    printf("已完成图像滤波的均值滤波\n");

    unsigned char* grayImage6 = (unsigned char*)malloc(width * height * 1);//为灰度值备份图片分配内存
    RmwMedianFilter(grayImage,width,height,3,3, grayImage6);//中值滤波
    saveGrayScaleBMP("output/中值滤波.bmp", grayImage6,width,height);
    printf("已完成图像滤波的中值滤波\n");

    unsigned char* grayImage8 = (unsigned char*)malloc(width * height * 1);//为灰度值备份图片分配内存
    RmwBinImgFilter(grayImage, width, height, 3, 3, 100,grayImage8);//二值滤波
    saveGrayScaleBMP("output/二值滤波.bmp", grayImage8,width,height);
    printf("已完成图像滤波的二值滤波.bmp\n");


    unsigned char* grayImage7 = (unsigned char*)malloc(width * height * 1);//为灰度值备份图片分配内存

    //[释放内存]
    free(imageData);
    free(grayImage1);
    free(grayImage);//释放内存
    fclose(img);

    return 0;
}


