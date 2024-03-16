#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL.h>
#include <time.h>

//void displayHistogram(const int* histogram, int size,double a,double b,double c) 
//{
//
//    SDL_Init(SDL_INIT_VIDEO);
//
//    char windowTitle[100];
//    sprintf(windowTitle, "Histogram      Brightness: %.2f  Contrast: %.2f  Median: %.2f", a, b, c);
//    SDL_Window* window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 256, SDL_WINDOW_SHOWN);
//
//    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
//    if (renderer == NULL) {
//        printf("Failed to create renderer: %s\n", SDL_GetError());
//        return;
//    }
//
//    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//
//    SDL_RenderClear(renderer);
//
//    int maxBarHeight = 0;
//    int minBarHeight = 0;
//
//    for (int i = 0; i < size; i++) 
//    {
//        if (histogram[i] > maxBarHeight) 
//        {
//            maxBarHeight = histogram[i];
//        }
//    }
//
//    for (int i = 0; i < size; i++) 
//    {
//        if (histogram[i] < minBarHeight) 
//        {
//            minBarHeight = histogram[i];
//        }
//    }
//
//
//    int y = 250;
//
//
//    int barWidth = 2;
//    double k = (double)(200) / maxBarHeight;
//
//
//    for (int i = 0; i < size; i++) {
//
//        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
//
//        int barX = i * barWidth + 1;
//        int barY = y - (int)(histogram[i] * k);
//
//        SDL_Rect bar = { barX, barY, barWidth, (int)(histogram[i] * k) };
//        
//        SDL_RenderFillRect(renderer, &bar);
//    }
//
//    SDL_RenderPresent(renderer);
//
//    SDL_Event event;
//    int quit = 0;
//    while (!quit) {
//        while (SDL_PollEvent(&event)) {
//            if (event.type == SDL_QUIT) {
//                quit = 1;
//            }
//        }
//    }
//
//    SDL_DestroyRenderer(renderer);
//    SDL_DestroyWindow(window);
//
//    SDL_Quit();
//}
//
//void GetHistogram(uint8_t* pImg, int width, int height, int* histogram)
//{
//    uint8_t* pCur;
//    uint8_t* pEnd = pImg + width * height;
//
//    // 初始化直方图数组
//    memset(histogram, 0, sizeof(int) * 256);
//    //int i = 1;
//    // 直方图统计
//    for (pCur = pImg; pCur < pEnd;)
//    {
//        histogram[*pCur]++;
//        pCur++;
//    }
//
//    // 函数结束
//    return;
//}
//
//double mean(const int* histogram, int width, int height)
//{
//    size_t sum = 0;
//    size_t totalPixels = width * height;
//
//    for (size_t i = 0; i < 256; i++)
//    {
//        sum += i * histogram[i]; // 将像素值乘以出现次数再累加
//    }
//
//    double a = (double)sum / totalPixels;
//
//    return a;
//}
//
//
//
//double StandardDeviation(const int* histogram, int width, int height, double mean)
//{
//    double variance = 0.0;
//    int totalPixels = width * height;
//
//    for (int i = 0; i < 256; i++)
//    {
//        variance += pow(i - mean, 2) * histogram[i];
//    }
//
//    variance /= (totalPixels - 1);
//
//    double standardDeviation = sqrt(variance);
//
//    return standardDeviation;
//}
//
//double Median(const int* histogram, int width, int height)
//{
//    int size = width * height;
//    int cumulativeSum = 0;
//    int middleElement = (size + 1) / 2;
//
//    for (int i = 0; i < 256; ++i)
//    {
//        cumulativeSum += histogram[i];
//        if (cumulativeSum >= middleElement)
//        {
//            return i;
//        }
//    }
//
//    return 0;
//}
//
//void GetBrightContrast(int * histogram,double * bright,double * contrast)
//{
//    int g;
//    double sum,num;//书上说图像很亮时，int有可能会溢出，所以我这里直接用double
//    double fsum;
//    
//    //step.1 求亮度
//    for(sum = num = 0, g = 0; g < 256 ; g++)
//    {
//        sum += histogram[g] * g;
//        num += histogram[g];
//    }
//    * bright = sum * 1.0/num;
//    
//    //step.2 求对比度
//    for(fsum = 0.0,g = 0;g < 256;g++)
//    {
//        fsum += histogram[g]*(g - *bright) * (g - *bright);
//    }
//    * contrast = sqrt(fsum/(num - 1));//即Std Dev
//    
//    //step.3 结束
//    return;
//}
//
//void LinearStretchDemo(uint8_t* pGryImg, int width, int height, double k, double b)
//{
//    uint8_t* pCur, * pEnd;
//    int LUT[256];
//    for (int g = 0; g < 256; g++)
//    {
//        LUT[g] = max(0, min(255, k * g + b));
//    }
//    for (pCur = pGryImg, pEnd = pGryImg + width * height; pCur < pEnd; pCur++)
//        {
//            *pCur = LUT[*pCur];
//        }
//    return;
//}
//
//typedef struct {
//    uint8_t* imageData;
//    int width;
//    int height;
//} ImageData;
//
//ImageData readImage(const char* filename) {
//    ImageData imageData;
//
//    FILE* file = fopen(filename, "rb");
//    if (!file) {
//        printf("无法打开文件：%s\n", filename);
//        exit(1);
//    }
//
//    uint8_t bmpHeader[54];
//    size_t bytesRead = fread(bmpHeader, 1, 54, file);
//
//    int width = *(int*)&bmpHeader[18];   // 宽度信息位于偏移量为 18 的位置
//    int height = *(int*)&bmpHeader[22];  // 高度信息位于偏移量为 22 的位置
//
//    uint8_t* imageDataArray = (uint8_t*)malloc(width * height);
//    if (!imageDataArray) {
//        printf("内存分配失败\n");
//        fclose(file);
//        exit(1);
//    }
//
//    fseek(file, 54, SEEK_SET);  // 跳过 BMP 文件头
//
//    bytesRead = fread(imageDataArray, 1, width * height, file);
//
//    imageData.imageData = imageDataArray;
//    imageData.width = width;
//    imageData.height = height;
//
//    fclose(file);
//
//    return imageData;
//}
//
//void writeGrayscaleImageWithTime(uint8_t* grayscaleData, int width, int height, const char* filename) 
//{
//    FILE* file = fopen(filename, "wb");
//
//    if (!file) {
//        printf("无法打开文件：%s\n", filename);
//        exit(1);
//    }
//    // BMP 文件头
//    uint8_t bmpHeader[54] = {
//        0x42, 0x4D,            // 文件类型（"BM"）
//        0x36, 0x00, 0x0E, 0x00,  // 文件大小
//        0x00, 0x00, 0x00, 0x00,  // 保留字段
//        0x36, 0x00, 0x00, 0x00,  // 数据偏移量
//        0x28, 0x00, 0x00, 0x00,  // 信息头大小
//        0x00, 0x00, 0x00, 0x00,  // 宽度
//        0x00, 0x00, 0x00, 0x00,  // 高度
//        0x01, 0x00,            // 颜色平面数
//        0x08, 0x00,            // 位深
//        0x00, 0x00, 0x00, 0x00,  // 压缩方式
//        0x00, 0x00, 0x00, 0x00,  // 图像大小
//        0x00, 0x00, 0x00, 0x00,  // 水平分辨率
//        0x00, 0x00, 0x00, 0x00,  // 垂直分辨率
//        0x00, 0x00, 0x00, 0x00,  // 使用的颜色数
//        0x00, 0x00, 0x00, 0x00   // 重要颜色数
//    };
//
//    // 更新 BMP 头信息中的宽度和高度
//    *((int*)&bmpHeader[18]) = width;
//    *((int*)&bmpHeader[22]) = height;
//
//    fwrite(bmpHeader, 1, 54, file);  // 写入 BMP 头
//
//    // 写入灰度数据
//    fwrite(grayscaleData, 1, width * height, file);
//
//    fclose(file);
//}
//
//int main()
//{
//    int histogram[256];
//    
//    const char* filename = "E:/code/IDP-Learning-Journey/images/ME.bmp";
//
//    ImageData imageData = readImage(filename);
//
//    GetHistogram(imageData.imageData, imageData.width, imageData.height,histogram);
//
//    double meanValue = 0;
//    double StandardDeviationValue = 0;
//    double median = 0;
//    double bright, contrast;
//
//    GetBrightContrast(histogram, &bright, &contrast);
//
//    writeGrayscaleImageWithTime(imageData.imageData, imageData.width, imageData.height, "output.bmp");
//
//    median = Median(histogram, imageData.width, imageData.height);
//
//    //printf("中间值（Median）：%f\n", median);
//    //printf("亮度（Brightness）: %lf\n", bright);
//    //printf("对比度（Contrast）: %lf\n", contrast);
//
//    displayHistogram(histogram, 256, bright, contrast,median);
//    //displayImage(imageData, imageData.width, imageData.height);
//
// 
//    free(imageData.imageData);
//
//    return 0;
//}
//
//
