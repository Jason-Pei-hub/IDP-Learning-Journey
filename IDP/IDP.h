#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <nmmintrin.h>
 
uint8_t* readGrayScaleBMP(const char* filename, int* width, int* height);//读取8位灰度图片
void saveGrayScaleBMP(const char* filename, const uint8_t* imageData, int width, int height);// 将8位灰度图像数据保存为BMP文件
uint8_t* readColorBMP(const char* filename, int* width, int* height);//读取24位彩色图像的BMP文件
void saveColorBMP(const char* filename, const uint8_t* imageData, int width, int height);//将24位彩色图像数据保存为BMP文件
void LinearStretchDemo(uint8_t* pGryImg, int width, int height, double k, double b);//灰度线性拉伸
void GetHistogram(uint8_t* pImg, int width, int height, int* histogram);//统计图像灰度值
void GetBrightContrast(int* histogram, double* bright, double* contrast);//亮度和对比度
void RmwHistogramEqualize(uint8_t* pGryImg, int width, int height);//直方图均衡化
void RmwLogTransform(uint8_t* pGryImg, int width, int height);//对数变换
void RmwAvrFilterBySumCol(uint8_t* pGryImg, int width, int height, int M, int N, uint8_t* pResImg);//基于列积分的快速均值滤波
void RmwDoSumGryImg(uint8_t* pGryImg, int width, int height, int* pSumImg);//基于列积分的积分图实现
void RmwDoSumGryImg_SSE(uint8_t* pGryImg, int width, int height, int* pSumImg);//基于SSE的积分图实现
void RmwAvrFilterBySumImg(int* pSumImg, int width, int height, int M, int N, uint8_t* pResImg);//基于积分图的快速均值滤波  
void GetMedianGry(int* histogram, int N, int* medGry);//求灰度值中值
double RmwMedianFilter(uint8_t* pGryImg, int width, int height, int M, int N, uint8_t* pResImg);//中值滤波
void RmwBinImgFilter(uint8_t* pBinImg, int width, int height, int M, int N, double threshold, uint8_t* pResImg);//二值滤波