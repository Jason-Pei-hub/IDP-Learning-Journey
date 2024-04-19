#define _CRT_SECURE_NO_WARNINGS 1

#include "IDP.h"

int main() 
{
    const char* grayScaleFilename = "H0701Gry.bmp";

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

    GetHistogram(grayScaleImageData, grayScaleWidth, grayScaleHeight,his);

    GetBrightContrast(his, &liangdu1, &duibidu1);
    // 执行对灰度图像的操作
    //LinearStretchDemo(grayScaleImageData, grayScaleWidth, grayScaleHeight,1.8,0);
    RmwHistogramEqualize(grayScaleImageData, grayScaleWidth, grayScaleHeight);

    GetHistogram(grayScaleImageData, grayScaleWidth, grayScaleHeight, hisout);

    GetBrightContrast(hisout, &liangdu2, &duibidu2);

    // 保存8位灰度图像数据为BMP文件
    saveGrayScaleBMP("out.bmp", grayScaleImageData, grayScaleWidth, grayScaleHeight);


    printf("原始图像的亮度为：%.0f,对比度为%.0f\n", liangdu1, duibidu1);
    printf("输出图像的亮度为：%.0f,对比度为%.0f\n", liangdu2, duibidu2);

    free(grayScaleImageData);


    return 0;
}
