#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


uint8_t* readGrayScaleBMP(const char* filename, int* width, int* height) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return NULL;
    }

    // 读取BMP文件头部信息
    uint8_t bmpHeader[54];
    fread(bmpHeader, 1, 54, file);

    // 从文件头部提取图像宽度和高度信息
    *width = *(int*)&bmpHeader[18];
    *height = *(int*)&bmpHeader[22];

    // 分配存储图像数据的内存
    uint8_t* imageData = (uint8_t*)malloc(*width * *height);
    if (!imageData) {
        fprintf(stderr, "内存分配失败\n");
        fclose(file);
        return NULL;
    }

    // 计算调色板的大小
    int paletteSize = *(int*)&bmpHeader[46];
    if (paletteSize == 0)
        paletteSize = 256;

    // 读取调色板数据
    uint8_t palette[1024];
    fread(palette, 1, paletteSize * 4, file);

    // 读取图像数据
    fseek(file, *(int*)&bmpHeader[10], SEEK_SET);
    fread(imageData, 1, *width * *height, file);

    fclose(file);

    return imageData;
}

// 将8位灰度图像数据保存为BMP文件
void saveGrayScaleBMP(const char* filename, const uint8_t* imageData, int width, int height) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error creating file %s\n", filename);
        return;
    }

    // BMP文件头部信息
    uint8_t bmpHeader[54] = {
        0x42, 0x4D,             // 文件类型标识 "BM"
        0x36, 0x00, 0x0C, 0x00, // 文件大小（以字节为单位，此处假设图像数据大小不超过4GB）
        0x00, 0x00,             // 保留字段
        0x00, 0x00,             // 保留字段
        0x36, 0x00, 0x00, 0x00, // 位图数据偏移（以字节为单位）
        0x28, 0x00, 0x00, 0x00, // 位图信息头大小（40字节）
        0x00, 0x00, 0x00, 0x00, // 图像宽度
        0x00, 0x00, 0x00, 0x00, // 图像高度
        0x01, 0x00,             // 目标设备的级别（此处为1，不压缩）
        0x08, 0x00,             // 每个像素的位数（8位）
        0x00, 0x00, 0x00, 0x00, // 压缩类型（此处为不压缩）
        0x00, 0x00, 0x00, 0x00, // 图像数据大小（以字节为单位，此处为0，表示不压缩）
        0x00, 0x00, 0x00, 0x00, // 水平分辨率（像素/米，此处为0，表示未知）
        0x00, 0x00, 0x00, 0x00, // 垂直分辨率（像素/米，此处为0，表示未知）
        0x00, 0x00, 0x00, 0x00, // 使用的颜色索引数（0表示使用所有调色板项）
        0x00, 0x00, 0x00, 0x00  // 重要的颜色索引数（0表示所有颜色都重要）
    };

    // 更新BMP文件头部信息中的宽度和高度
    *(int*)&bmpHeader[18] = width;
    *(int*)&bmpHeader[22] = height;

    // 写入BMP文件头部信息
    fwrite(bmpHeader, 1, 54, file);

    // 写入调色板数据
    for (int i = 0; i < 256; i++) {
        fputc(i, file);  // 蓝色分量
        fputc(i, file);  // 绿色分量
        fputc(i, file);  // 红色分量
        fputc(0, file);  // 保留字节
    }

    // 写入图像数据
    fwrite(imageData, 1, width * height, file);

    fclose(file);
}

// 读取24位彩色图像的BMP文件
uint8_t* readColorBMP(const char* filename, int* width, int* height) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return NULL;
    }

    // 读取BMP文件头部信息
    uint8_t bmpHeader[54];
    fread(bmpHeader, 1, 54, file);

    // 从文件头部提取图像宽度和高度信息
    *width = *(int*)&bmpHeader[18];
    *height = *(int*)&bmpHeader[22];

    // 分配存储图像数据的内存
    uint8_t* imageData = (uint8_t*)malloc(*width * *height * 3);
    if (!imageData) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    // 读取图像数据
    fseek(file, *(int*)&bmpHeader[10], SEEK_SET);
    fread(imageData, 1, *width * *height * 3, file);

    fclose(file);

    return imageData;
}

void saveColorBMP(const char* filename, const uint8_t* imageData, int width, int height) {
    FILE* file = fopen(filename, "wb");
    if (!file) {
        fprintf(stderr, "Error creating file %s\n", filename);
        return;
    }

    // BMP文件头部信息
    uint8_t bmpHeader[54] = {
        0x42, 0x4D,             // 文件类型标识 "BM"
        0x00, 0x00, 0x00, 0x00, // 文件大小（占位，稍后计算）
        0x00, 0x00,             // 保留字段
        0x00, 0x00,             // 保留字段
        0x36, 0x00, 0x00, 0x00, // 位图数据偏移（以字节为单位）
        0x28, 0x00, 0x00, 0x00, // 位图信息头大小（40字节）
        0x00, 0x00, 0x00, 0x00, // 图像宽度
        0x00, 0x00, 0x00, 0x00, // 图像高度
        0x01, 0x00,             // 目标设备的级别（此处为1，不压缩）
        0x18, 0x00,             // 每个像素的位数（24位）
        0x00, 0x00, 0x00, 0x00, // 压缩类型（此处为不压缩）
        0x00, 0x00, 0x00, 0x00, // 图像数据大小（占位，稍后计算）
        0x00, 0x00, 0x00, 0x00, // 水平分辨率（像素/米，此处为0，表示未知）
        0x00, 0x00, 0x00, 0x00, // 垂直分辨率（像素/米，此处为0，表示未知）
        0x00, 0x00, 0x00, 0x00, // 使用的颜色索引数（0表示使用所有调色板项）
        0x00, 0x00, 0x00, 0x00  // 重要的颜色索引数（0表示所有颜色都重要）
    };

    // 更新BMP文件头部信息中的宽度和高度
    *(int*)&bmpHeader[18] = width;
    *(int*)&bmpHeader[22] = height;

    // 计算图像数据大小
    uint32_t imageDataSize = width * height * 3 + 54; // 加上文件头部大小
    bmpHeader[2] = (uint8_t)(imageDataSize & 0xFF);
    bmpHeader[3] = (uint8_t)((imageDataSize >> 8) & 0xFF);
    bmpHeader[4] = (uint8_t)((imageDataSize >> 16) & 0xFF);
    bmpHeader[5] = (uint8_t)((imageDataSize >> 24) & 0xFF);

    // 写入BMP文件头部信息
    fwrite(bmpHeader, 1, 54, file);

    // 写入图像数据
    fwrite(imageData, width * height * 3, 1, file);

    fclose(file);
}


int main() {
    const char* grayScaleFilename = "gray_scale.bmp";
    const char* colorFilename = "FXT12111.bmp";

    int grayScaleWidth, grayScaleHeight;
    int width, height;

    // 读取8位灰度图像
    uint8_t* grayScaleImageData = readGrayScaleBMP(grayScaleFilename, &grayScaleWidth, &grayScaleHeight);
    if (!grayScaleImageData) {
        fprintf(stderr, "Failed to read gray scale BMP image\n");
        return 1;
    }

    // 读取24位彩色图像
    uint8_t* colorImageData = readColorBMP(colorFilename, &width, &height);
    if (!colorImageData) {
        fprintf(stderr, "Failed to read color BMP image\n");
        return 1;
    }

    // 执行对灰度图像的操作
    // 保存8位灰度图像数据为BMP文件
    saveGrayScaleBMP("out.bmp", grayScaleImageData, grayScaleWidth, grayScaleHeight);

    // 保存24位灰度图像数据为BMP文件
    saveColorBMP("outcolor.bmp", colorImageData, width, height);
    

    free(grayScaleImageData);
    free(colorImageData);


    return 0;
}