#define _CRT_SECURE_NO_WARNINGS 1

#include "IDP.h"

int main() 
{
    const char* Filename = "input/test.bmp";

    int width, height;

   

    int his[256] = { 0 };
    int hisout[256] = { 0 };

    //double liangdu1 = 0;
    //double duibidu1 = 0;
    //double liangdu2 = 0;
    //double duibidu2 = 0;

    uint8_t* ImageData = readColorBMP(Filename,&width,&height);//读取24位彩色图像的BMP文件

    //uint8_t* grayScaleImageData = readGrayScaleBMP(Filename, &width, &height);//读取8位灰度图片

    //uint8_t* pTmpImg = (uint8_t*)malloc(width * height);

    //uint8_t* pResImgout = (uint8_t*)malloc(width * height);

    uint8_t* grayScaleImageData = (uint8_t*)malloc(width * height);//开辟空间存放灰度值图片
    
    convertToGray(ImageData, grayScaleImageData,width,height);//24位彩色图像转8位灰度值

    


    //int* pSumImg = (int*)malloc(grayScaleWidth * grayScaleHeight * sizeof(int));//申请积分图的内存

    //RmwDoSumGryImg(grayScaleImageData, grayScaleWidth, grayScaleHeight,pSumImg);//基于列积分的积分图实现

    //GetHistogram(grayScaleImageData, width, height,his);

    //GetBrightContrast(his, &liangdu1, &duibidu1);
    // 执行对灰度图像的操作
    //RmwHistogramEqualize(grayScaleImageData,width, height);//直方图均衡化
    //LinearStretchDemo(grayScaleImageData, width, height,3,-60);
    //uint8_t* pResImg = (uint8_t*)malloc(width * height);


    //RmwGradientGryImgPlus(grayScaleImageData,width,height, pResImg,15);//梯度算子
    //RmwRobertsGryImg(grayScaleImageData, width, height, pResImg);//罗伯特算子
    //RmwSobelGryImg(grayScaleImageData, width, height, pResImg);//索贝尔算子
    //RmwPrewittGryImg(grayScaleImageData, width, height, pResImg); //Prewitt算子
    //RmwShenJunGryImg(pResImg,pTmpImg, width, height,0.01, pResImgout);//沈俊算子
    
    //RmwExtractRiceEdge(grayScaleImageData, pTmpImg, width, height,0.01,5, pResImg);//索贝尔＋沈俊算子
    //invertImage(pResImg,width,height);//反相
    //LinearStretchDemo(grayScaleImageData, grayScaleWidth, grayScaleHeight,1.8,0);
    //RmwHistogramEqualize(grayScaleImageData, grayScaleWidth, grayScaleHeight);

    

    //RmwMedianFilter(grayScaleImageData, grayScaleWidth, grayScaleHeight,5,5,pResImg);//中值滤波
    //RmwBinImgFilter(grayScaleImageData, grayScaleWidth, grayScaleHeight,5,5,150,pResImg);//二值滤波

    //RmwAvrFilterBySumImg(pSumImg, grayScaleWidth, grayScaleHeight,5,5, pResImg);//基于积分图的快速均值滤波  

    //GetHistogram(pResImg, width, height, hisout);

    //GetBrightContrast(hisout, &liangdu2, &duibidu2);

    // 保存8位灰度图像数据为BMP文件
    saveGrayScaleBMP("output/testout111.bmp", grayScaleImageData, width, height);
    //saveColorBMP("testout222.bmp", ImageData ,width,height);
    //saveGrayScaleBMP("endout111.bmp", pResImg, width, height);
    


    //printf("原始图像的亮度为：%.0f,对比度为%.0f\n", liangdu1, duibidu1);
    //printf("输出图像的亮度为：%.0f,对比度为%.0f\n", liangdu2, duibidu2);

    free(grayScaleImageData);
    //free(pSumImg);
    //free(ImageData);
    //free(pTmpImg);


    return 0;
}
