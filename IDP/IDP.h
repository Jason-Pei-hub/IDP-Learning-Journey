#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
 
uint8_t* readGrayScaleBMP(const char* filename, int* width, int* height);//读取8位灰度图片
void saveGrayScaleBMP(const char* filename, const uint8_t* imageData, int width, int height);// 将8位灰度图像数据保存为BMP文件
uint8_t* readColorBMP(const char* filename, int* width, int* height);//读取24位彩色图像的BMP文件
void saveColorBMP(const char* filename, const uint8_t* imageData, int width, int height);//将24位彩色图像数据保存为BMP文件
void LinearStretchDemo(uint8_t* pGryImg, int width, int height, double k, double b);//灰度线性拉伸
void GetHistogram(uint8_t* pImg, int width, int height, int* histogram);//统计图像灰度值
void GetBrightContrast(int* histogram, double* bright, double* contrast);//亮度和对比度
void RmwHistogramEqualize(uint8_t* pGryImg, int width, int height);//直方图均衡化
void RmwLogTransform(uint8_t* pGryImg, int width, int height);//对数变换