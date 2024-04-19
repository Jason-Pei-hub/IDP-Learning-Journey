#define _CRT_SECURE_NO_WARNINGS 1

#include "IDP.h"

//读取8位灰度图片
//filename：字符数组的指针，用于指定要保存的图像文件的名称或路径。
//imageData：无符号 8 位整型数据的指针，代表要保存的图像数据。
//width：图像的宽度。
//height：图像的高度。
uint8_t* readGrayScaleBMP(const char* filename, int* width, int* height) 
{
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
//filename：字符数组的指针，用于指定要保存的图像文件的名称或路径。
//imageData：无符号 8 位整型数据的指针，代表要保存的图像数据。
//width：图像的宽度。
//height：图像的高度。
void saveGrayScaleBMP(const char* filename, const uint8_t* imageData, int width, int height) 
{
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
//filename：字符数组的指针，用于指定要读取的 BMP 格式图像文件的名称或路径。
//width：整型变量的指针，用于存储读取的图像的宽度。
//height：整型变量的指针，用于存储读取的图像的高度。
uint8_t* readColorBMP(const char* filename, int* width, int* height) 
{
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

//将24位彩色图像数据保存为BMP文件
//filename：字符数组的指针，用于指定要保存的图像文件的名称或路径。
//imageData：无符号 8 位整型数据的指针，代表要保存的图像数据。
//width：图像的宽度。
//height：图像的高度。
void saveColorBMP(const char* filename, const uint8_t* imageData, int width, int height) 
{
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

//灰度线性拉伸
//pGryImg：灰度图像数据的指针。
//width：图像的宽度。
//height：图像的高度。
//k：线性拉伸的斜率。它控制着拉伸的速率或程度。当(k) 大于 1 时，图像的对比度增加；当(k) 小于 1 时，对比度降低。
//b：线性拉伸的偏移。它控制着拉伸后灰度值的起始位置。当(b) 大于 0 时，图像的整体亮度增加；当(b) 小于 0 时，整体亮度减小。
void LinearStretchDemo(uint8_t* pGryImg, int width, int height, double k, double b)
{
    uint8_t* pCur, * pEnd;
    int LUT[256];    //因为只有[0,255]共256个灰度值

    //step1. 生成查找表
    for (int g = 0; g < 256; g++)
    {
        LUT[g] = max(0, min(255, k * g + b));
    }

    //step2. 进行变换
    for (pCur = pGryImg, pEnd = pGryImg + width * height; pCur < pEnd; pCur++)
    {
        *pCur = LUT[*pCur];
    }
    //step3. 结束
    return;
}

//统计图像灰度值
//pImg：灰度图像数据的指针。
//width：图像的宽度。
//height：图像的高度。
//* histogram：数组首元素地址，需要一个能储存256个变量的整型数组
void GetHistogram(uint8_t* pImg, int width, int height, int* histogram)
{
    uint8_t* pCur;
    uint8_t* pEnd = pImg + width * height;

    // 初始化直方图数组
    memset(histogram, 0, sizeof(int) * 256);

    // 直方图统计
    for (pCur = pImg; pCur < pEnd;)
    {
        histogram[*pCur]++;
        pCur++;
    }

    // 函数结束
    return;
}

//亮度和对比度
//储存histogram灰度直方图的指针
//接收亮度的变量地址
//接收对比度的变量地址
void GetBrightContrast(int* histogram, double* bright, double* contrast)
{
    int g;
    double sum, num; //书上说图像很亮时，int有可能会溢出，所以我这里直接用double
    double fsum;

    //step.1 求亮度
    for (sum = num = 0, g = 0; g < 256; g++)
    {
        sum += histogram[g] * g;
        num += histogram[g];
    }
    *bright = sum * 1.0 / num;

    //step.2 求对比度
    for (fsum = 0.0, g = 0; g < 256; g++)
    {
        fsum += histogram[g] * (g - *bright) * (g - *bright);
    }
    *contrast = sqrt(fsum / (num - 1)); //即Std Dev

    //step.3 结束
    return;
}

//pGryImg：灰度图像数据的指针。
//width：图像的宽度。
//height：图像的高度。
void RmwHistogramEqualize(uint8_t* pGryImg, int width, int height)
{
    uint8_t* pCur, * pEnd = pGryImg + width * height; // 指针变量，指向当前像素和图像末尾
    int histogram[256], LUT[256], A, g; // 直方图数组、查找表数组、累积直方图、灰度级

    // step.1-------------求直方图--------------------------//
    memset(histogram, 0, sizeof(int) * 256); // 初始化直方图数组为0
    for (pCur = pGryImg; pCur < pEnd;)
        histogram[*(pCur++)]++; // 统计每个灰度级出现的频率

    // step.2-------------求LUT[g]-------------------------//
    A = histogram[0]; // 初始化累积直方图的值为第一个灰度级的频率
    LUT[0] = 255 * A / (width * height); // 计算第一个灰度级对应的均衡化后的灰度值
    for (g = 1; g < 256; g++) {
        A += histogram[g]; // 更新累积直方图的值
        LUT[g] = 255 * A / (width * height); // 计算当前灰度级对应的均衡化后的灰度值
    }

    // step.3-------------查表------------------------------//
    for (pCur = pGryImg; pCur < pEnd;)
        *(pCur++) = LUT[*pCur]; // 使用查找表对每个像素进行灰度映射

    // step.4-------------结束------------------------------//
    return;
}

//对数变换
//pGryImg：灰度图像数据的指针。
//width：图像的宽度。
//height：图像的高度。
void RmwLogTransform(uint8_t* pGryImg, int width, int height)
{
    uint8_t* pCur, * pEnd = pGryImg + width * height; // 指向灰度图像数据的当前指针和结束指针
    int histogram[256], LUT[256], gmax, g; // 声明直方图数组、查找表数组、最大灰度值、当前灰度值
    double c; // 声明常数c

    // step.1-------------求直方图--------------------------//
    memset(histogram, 0, sizeof(int) * 256); // 初始化直方图数组为0
    for (pCur = pGryImg; pCur < pEnd;)
        histogram[*(pCur++)]++; // 遍历图像数据，统计每个灰度级的像素数量

    // step.2-------------最大值---------------------------//
    for (gmax = 255; gmax >= 0; gmax++)
        if (histogram[gmax]) break; // 从最大灰度级开始向低灰度级搜索，找到第一个非零灰度级，即最大灰度值

    // step.3-------------求LUT[g]-------------------------//
    c = 255.0 / log(1 + gmax); // 计算常数c
    for (g = 0; g < 256; g++)
    {
        LUT[g] = (int)(c * log(1 + g)); // 根据对数变换公式计算查找表中每个灰度级的映射值
    }

    // step.4-------------查表------------------------------//
    for (pCur = pGryImg; pCur < pEnd;)
        *(pCur++) = LUT[*pCur]; // 使用查找表将图像数据进行对数变换

    // step.5-------------结束------------------------------//
    return; // 函数结束
}
