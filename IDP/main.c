#define _CRT_SECURE_NO_WARNINGS 1

#include "IDP.h"

int main() 
{
    const char* grayScaleFilename = "H0606Gry.bmp";

    int grayScaleWidth, grayScaleHeight;

   

    int his[256] = { 0 };
    int hisout[256] = { 0 };

    double liangdu1 = 0;
    double duibidu1 = 0;
    double liangdu2 = 0;
    double duibidu2 = 0;

    // 读取8位灰度图像
    uint8_t* grayScaleImageData = readGrayScaleBMP(grayScaleFilename, &grayScaleWidth, &grayScaleHeight);
    if (!grayScaleImageData) {
        fprintf(stderr, "Failed to read gray scale BMP image\n");
        return 1;
    }

    //int* pSumImg = (int*)malloc(grayScaleWidth * grayScaleHeight * sizeof(int));//申请积分图的内存

    //RmwDoSumGryImg(grayScaleImageData, grayScaleWidth, grayScaleHeight,pSumImg);//基于列积分的积分图实现

    GetHistogram(grayScaleImageData, grayScaleWidth, grayScaleHeight,his);

    GetBrightContrast(his, &liangdu1, &duibidu1);
    // 执行对灰度图像的操作
    //LinearStretchDemo(grayScaleImageData, grayScaleWidth, grayScaleHeight,1.8,0);
    //RmwHistogramEqualize(grayScaleImageData, grayScaleWidth, grayScaleHeight);

    uint8_t* pResImg = (uint8_t*)malloc(grayScaleWidth * grayScaleHeight);

    //RmwMedianFilter(grayScaleImageData, grayScaleWidth, grayScaleHeight,5,5,pResImg);//中值滤波
    RmwBinImgFilter(grayScaleImageData, grayScaleWidth, grayScaleHeight,5,5,150,pResImg);//二值滤波

    //RmwAvrFilterBySumImg(pSumImg, grayScaleWidth, grayScaleHeight,5,5, pResImg);//基于积分图的快速均值滤波  

    GetHistogram(pResImg, grayScaleWidth, grayScaleHeight, hisout);

    GetBrightContrast(hisout, &liangdu2, &duibidu2);

    // 保存8位灰度图像数据为BMP文件
    saveGrayScaleBMP("out.bmp", pResImg, grayScaleWidth, grayScaleHeight);


    printf("原始图像的亮度为：%.0f,对比度为%.0f\n", liangdu1, duibidu1);
    printf("输出图像的亮度为：%.0f,对比度为%.0f\n", liangdu2, duibidu2);

    free(grayScaleImageData);
    free(grayScaleImageData);
    //free(pSumImg);
    free(pResImg);


    return 0;
}
