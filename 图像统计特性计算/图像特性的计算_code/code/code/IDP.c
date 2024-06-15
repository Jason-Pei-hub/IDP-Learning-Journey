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
    assert(filename);
    FILE* file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error opening file %s\n", filename);
        return NULL;
    }

    // 读取BMP文件头部信息
    uint8_t bmpHeader[54];
    fread(bmpHeader, 1, 54, file);

    // 从文件头部提取图像宽度和高度信息方便传入主程序
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

//24位彩色图像转8位灰度值
//rgbImage原始图像
//grayImage输出灰度图像
//width,height图片的宽和高
void convertToGray(uint8_t* rgbImage, uint8_t* grayImage, int width, int height)
{
    assert(rgbImage && grayImage);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            
            // 获取当前像素的 RGB 分量
            uint8_t r = rgbImage[3 * (y * width + x) + 0];
            uint8_t g = rgbImage[3 * (y * width + x) + 1];
            uint8_t b = rgbImage[3 * (y * width + x) + 2];

            // 计算灰度值（常用的加权平均法）
            // 这里使用的加权系数是常见的：R: 0.299, G: 0.587, B: 0.114
            uint8_t gray = (uint8_t)(0.299 * r + 0.587 * g + 0.114 * b);

            // 将灰度值写入灰度图像数组
            grayImage[y * width + x] = gray;
        }
    }
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

//基于列积分的快速均值滤波
//原始灰度图像
//图像的宽度和高度
//滤波邻域：M列N行
//结果图像
void RmwAvrFilterBySumCol(uint8_t* pGryImg,int width, int height,int M, int N,uint8_t* pResImg) 
{
    uint8_t* pAdd, * pDel, * pRes;
    int halfx, halfy;
    int x, y;
    int sum, c;
    int sumCol[4096]; // 约定图像宽度不大于4096

    // step.1------------初始化--------------------------//
    M = M / 2 * 2 + 1; // 奇数化
    N = N / 2 * 2 + 1; // 奇数化
    halfx = M / 2; // 滤波器的半径x
    halfy = N / 2; // 滤波器的半径y
    c = (1 << 23) / (M * N); // 乘法因子
    memset(sumCol, 0, sizeof(int) * width);
    for (y = 0, pAdd = pGryImg; y < N; y++) {
        for (x = 0; x < width; x++) sumCol[x] += *(pAdd++);
    }
    // step.2------------滤波----------------------------//
    for (y = halfy, pRes = pResImg + y * width, pDel = pGryImg; y < height - halfy; y++) {
        // 初值
        for (sum = 0, x = 0; x < M; x++) sum += sumCol[x];
        // 滤波
        pRes += halfx; // 跳过左侧
        for (x = halfx; x < width - halfx; x++) {
            // 求灰度均值
            // *(pRes++)=sum/(N*M);
            *(pRes++) = (sum * c) >> 23; // 用整数乘法和移位代替除法
            // 换列,更新灰度和
            sum -= sumCol[x - halfx]; // 减左边列
            sum += sumCol[x + halfx + 1]; // 加右边列
        }
        pRes += halfx; // 跳过右侧
        // 换行,更新sumCol
        for (x = 0; x < width; x++) {
            sumCol[x] -= *(pDel++); // 减上一行
            sumCol[x] += *(pAdd++); // 加下一行
        }
    }
    // step.3------------返回----------------------------//
    return;
}

//基于列积分的积分图实现
//pGryImg, // 原始灰度图像
//width,       // 图像的宽度 
//height,      // 图像的高度
//pSumImg     // 计算得到的积分图
void RmwDoSumGryImg(uint8_t* pGryImg,int width,int height, int* pSumImg)
{
    uint8_t* pGry;
    int* pRes;
    int x, y;
    int sumCol[4096]; // 约定图像宽度不大于4096

    memset(sumCol, 0, sizeof(int) * width);
    for (y = 0, pGry = pGryImg, pRes = pSumImg; y < height; y++)
    {
        // 最左侧像素的特别处理
        sumCol[0] += *(pGry++);
        *(pRes++) = sumCol[0];
        // 正常处理
        for (x = 1; x < width; x++)
        {
            sumCol[x] += *(pGry++);       // 更新列积分
            int temp = *(pRes - 1);
            *(pRes++) = temp + sumCol[x];
        }
    }
    return;
}

//基于SSE的积分图实现
//pGryImg原始灰度图像
//width图像的宽度，必须是4的倍数
//height图像的高度
//pSumImg计算得到的积分图
void RmwDoSumGryImg_SSE(uint8_t* pGryImg, int width, int height, int* pSumImg)
{
    int sumCol[4096]; //约定图像宽度不大于4096
    __m128i* pSumSSE, A;
    uint8_t* pGry;
    int* pRes;
    int x, y;

    memset(sumCol, 0, sizeof(int) * width);
    for (y = 0, pGry = pGryImg, pRes = pSumImg; y < height; y++)
    {
        // 0:需要特别处理
        sumCol[0] += *(pGry++);
        *(pRes++) = sumCol[0];
        // 1
        sumCol[1] += *(pGry++);
        *(pRes++) = *(pRes - 1) + sumCol[1];
        // 2
        sumCol[2] += *(pGry++);
        *(pRes++) = *(pRes - 1) + sumCol[2];
        // 3
        sumCol[3] += *(pGry++);
        *(pRes++) = *(pRes - 1) + sumCol[3];
        // [4...width-1]
        for (x = 4, pSumSSE = (__m128i*)(sumCol + 4); x < width; x += 4, pGry += 4)
        {
            // 把变量的低32位(有4个8位整数组成)转换成32位的整数
            A = _mm_cvtepu8_epi32(_mm_loadl_epi64((__m128i*)pGry));
            // 4个32位的整数相加
            *(pSumSSE++) = _mm_add_epi32(*pSumSSE, A);
            // 递推
            *(pRes++) = *(pRes - 1) + sumCol[x + 0];
            *(pRes++) = *(pRes - 1) + sumCol[x + 1];
            *(pRes++) = *(pRes - 1) + sumCol[x + 2];
            *(pRes++) = *(pRes - 1) + sumCol[x + 3];
        }
    }
    return;
}

//基于积分图的快速均值滤波
//pSumImg计算得到的积分图
//width,height,图像的宽度和高度
//M, N,滤波邻域：M列N行
//pResImg 结果图像
void RmwAvrFilterBySumImg(int* pSumImg,int width, int height,int M, int N,uint8_t* pResImg)
{
    // 没有对边界上邻域不完整的像素进行处理，可以采用变窗口的策略
    int* pY1, * pY2;
    uint8_t* pRes;
    int halfx, halfy;
    int y, x1, x2;
    int sum, c;

    // step.1------------初始化--------------------------//
    M = M / 2 * 2 + 1; // 奇数化
    N = N / 2 * 2 + 1; // 奇数化
    halfx = M / 2;      // 滤波器的半径x
    halfy = N / 2;      // 滤波器的半径y
    c = (1 << 23) / (M * N); // 乘法因子
    // step.2------------滤波----------------------------//
    for (y = halfy + 1, pRes = pResImg + y * width, pY1 = pSumImg, pY2 = pSumImg + N * width;
        y < height - halfy;
        y++, pY1 += width, pY2 += width)
    {
        pRes += halfx + 1; // 跳过左侧
        for (x1 = 0, x2 = M; x2 < width; x1++, x2++) // 可以简化如此，但不太容易读
        {
            sum = *(pY2 + x2) - *(pY2 + x1) - *(pY1 + x2) + *(pY1 + x1);
            *(pRes++) = (uint8_t)((sum * c) >> 23); // 用整数乘法和移位代替除法
        }
        pRes += halfx; // 跳过右侧
    }
    // step.3------------返回----------------------------//
    return;
}

void GetMedianGry(int* histogram, int N, int* medGry)
{
    int g;
    int num;

    // step.1-------------求灰度中值------------------------//
    num = 0;
    for (g = 0; g < 256; g++)
    {
        num += histogram[g];
        if (2 * num > N) break;  //num>N/2
    }
    *medGry = g;
    // step.2-------------结束------------------------------//
    return;
}

//中值滤波
//pGryImg：指向待处理灰度图像数据的指针。
//width、height：表示图像的宽度和高度。
//M、N：分别表示中值滤波器的水平和垂直邻域大小（以像素为单位）。
//pResImg：指向存储结果图像数据的指针。
double RmwMedianFilter(uint8_t* pGryImg, int width, int height, int M, int N, uint8_t* pResImg) 
{
    uint8_t* pCur, * pRes;
    int halfx, halfy, x, y, i, j, y1, y2;
    int histogram[256];
    int wSize, j1, j2;
    int num, med, v;
    int dbgCmpTimes = 0; // 搜索中值所需比较次数的调试

    M = M / 2 * 2 + 1; // 奇数化
    N = N / 2 * 2 + 1; // 奇数化
    halfx = M / 2;      // x半径
    halfy = N / 2;      // y半径
    wSize = (halfx * 2 + 1) * (halfy * 2 + 1); // 邻域内像素总个数

    for (y = halfy, pRes = pResImg + y * width; y < height - halfy; y++) {
        // step.1----初始化直方图
        y1 = y - halfy;
        y2 = y + halfy;
        memset(histogram, 0, sizeof(int) * 256);

        for (i = y1, pCur = pGryImg + i * width; i <= y2; i++, pCur += width) {
            for (j = 0; j < halfx * 2 + 1; j++) {
                histogram[*(pCur + j)]++;
            }
        }

        // step.2-----初始化中值
        num = 0; // 记录着灰度值从0到中值的个数
        for (i = 0; i < 256; i++) {
            num += histogram[i];
            if (num * 2 > wSize) {
                med = i;
                break;
            }
        }

        // 滤波
        pRes += halfx; // 没有处理图像左边界侧的像素
        for (x = halfx; x < width - halfx; x++) {
            // 赋值
            *(pRes++) = med;

            // step.3-----直方图递推: 减去当前邻域最左边的一列,添加邻域右侧的一个新列
            j1 = x - halfx;     // 最左边列
            j2 = x + halfx + 1; // 右边的新列

            for (i = y1, pCur = pGryImg + i * width; i <= y2; i++, pCur += width) {
                // 减去最左边列
                v = *(pCur + j1);
                histogram[v]--;  // 更新直方图
                if (v <= med) num--; // 更新num

                // 添加右边的新列
                v = *(pCur + j2);
                histogram[v]++; // 更新直方图
                if (v <= med) num++; // 更新num
            }

            // step.4-----更新中值
            if (num * 2 < wSize) { // 到上次中值med的个数不够了,则med要变大
                for (med = med + 1; med < 256; med++) {
                    dbgCmpTimes += 2; // 总的比较次数,调试用
                    num += histogram[med];
                    if (num * 2 > wSize) break;
                }
                dbgCmpTimes += 1; // 总的比较次数,调试用
            }
            else { // 到上次中值med的个数多了,则med要变小
                while ((num - histogram[med]) * 2 > wSize) { // 若减去后,仍变小
                    dbgCmpTimes++; // 总的比较次数,调试用
                    num -= histogram[med];
                    med--;
                }
                dbgCmpTimes += 2; // 总的比较次数,调试用
            }
        }
        pRes += halfx; // 没有处理图像右边界侧的像素
    }
    // 返回搜索中值需要的平均比较次数
    return dbgCmpTimes * 1.0 / ((width - halfx * 2) * (height - halfy * 2));
}

// 均值滤波
// pGryImg：指向待处理灰度图像数据的指针。
// width、height：表示图像的宽度和高度。
// M、N：分别表示均值滤波器的水平和垂直邻域大小（以像素为单位）。
// pResImg：指向存储结果图像数据的指针。
void RmwMeanFilter(uint8_t* pGryImg, int width, int height, int M, int N, uint8_t* pResImg) {
    int borderX = M / 2;
    int borderY = N / 2;

    for (int y = borderY; y < height - borderY; ++y) {
        for (int x = borderX; x < width - borderX; ++x) {
            // 计算邻域内像素的平均值
            int sum = 0;
            for (int j = -borderY; j <= borderY; ++j) {
                for (int i = -borderX; i <= borderX; ++i) {
                    sum += pGryImg[(y + j) * width + (x + i)];
                }
            }
            int avg = sum / (M * N);

            // 将平均值存储在结果图像中
            pResImg[y * width + x] = avg;
        }
    }
}

//二值滤波
//pBinImg,  原始二值图像
// width, height,图像的宽度和高度
// M, N, 滤波邻域：M列N行
// threshold, 灰度阈值,大于等于该值时结果赋255
// pResImg 结果图像
void RmwBinImgFilter(uint8_t* pBinImg,int width, int height,int M, int N,double threshold,uint8_t* pResImg )
{
    // 没有对边界上邻域不完整的像素进行处理，可以采用变窗口的策略
    uint8_t* pAdd, * pDel, * pRes;
    int halfx, halfy;
    int x, y, sum, sumThreshold;
    int sumCol[4096]; //约定图像宽度不大于4096

    // step.1------------初始化--------------------------//
    M = M / 2 * 2 + 1; //奇数化
    N = N / 2 * 2 + 1; //奇数化
    halfx = M / 2; //滤波器的x半径
    halfy = N / 2; //滤波器的y半径
    sumThreshold = max(1, (int)(threshold * M * N)); //转换成邻域内灰度值之和的阈值
    memset(sumCol, 0, sizeof(int) * width);
    for (y = 0, pAdd = pBinImg; y < N; y++)
    {
        for (x = 0; x < width; x++)
            sumCol[x] += *(pAdd++);
    }
    // step.2------------滤波----------------------------//
    for (y = halfy, pRes = pResImg + y * width, pDel = pBinImg; y < height - halfy; y++)
    {
        //初值
        for (sum = 0, x = 0; x < M; x++)
            sum += sumCol[x];
        //滤波
        pRes += halfx; //跳过左侧
        for (x = halfx; x < width - halfx; x++)
        {
            //求灰度均值
            /*if (sum>=sumThreshold)
            {
                *(pRes++) = 255;
            }
            else  *(pRes++) = 0;*/
            *(pRes++) = (sum >= sumThreshold) * 255; //为真时赋255,否则赋0
            //换列,更新灰度和
            sum -= sumCol[x - halfx];     //减左边列
            sum += sumCol[x + halfx + 1]; //加右边列
        }
        pRes += halfx; //跳过右侧
        //换行,更新sumCol
        for (x = 0; x < width; x++)
        {
            sumCol[x] -= *(pDel++); //减上一行
            sumCol[x] += *(pAdd++); //加下一行
        }
    }
    // step.3------------返回----------------------------//
    return;
}

//梯度算子加阈值
//pGryImg：输入的灰度图像数据指针。
//width：图像的宽度。
//height：图像的高度。
//pGrdImg：输出的梯度图像数据指针
void RmwGradientGryImg(uint8_t* pGryImg, int width, int height, uint8_t* pGrdImg)
{
    uint8_t* pGry, * pGrd;
    int dx, dy;
    int x, y;

    for (y = 0, pGry = pGryImg, pGrd = pGrdImg; y < height - 1; y++)
    {
        for (x = 0; x < width - 1; x++, pGry++)
        {
            dx = *pGry - *(pGry + 1);
            dy = *pGry - *(pGry + width);
            int gradient = (int)(sqrt(dx * dx * 1.0 + dy * dy));
            *pGrd++ = (gradient > 255) ? 255 : gradient;
        }
        *pGrd++ = 0; //尾列不做,边缘强度赋0
        pGry++;
    }
    memset(pGrd, 0, width); //尾行不做,边缘强度赋0
}

//梯度算子加阈值
//pGryImg：输入的灰度图像数据指针。
//width：图像的宽度。
//height：图像的高度。
//pGrdImg：输出的梯度图像数据指针
void RmwGradientGryImgPlus(uint8_t* pGryImg, int width, int height, uint8_t* pGrdImg, int threshold)
{
    uint8_t* pGry, * pGrd;
    int dx, dy;
    int x, y;

    for (y = 0, pGry = pGryImg, pGrd = pGrdImg; y < height - 1; y++)
    {
        for (x = 0; x < width - 1; x++, pGry++)
        {
            dx = *pGry - *(pGry + 1);
            dy = *pGry - *(pGry + width);
            int gradient = (int)(sqrt(dx * dx * 1.0 + dy * dy));
            *(pGrd++) = (gradient > threshold) ? min(255, gradient) : 0;
        }
        *(pGrd++) = 0; //尾列不做,边缘强度赋0
        pGry++;
    }
    memset(pGrd, 0, width); //尾行不做,边缘强度赋0
}

//反相
void invertImage(uint8_t* image, int width, int height) {
    for (int i = 0; i < width * height; i++) {
        image[i] = 255 - image[i];
    }
}

//罗伯特算子
void RmwRobertsGryImg(uint8_t* pGryImg, int width, int height, uint8_t* pRbtImg)
{
    uint8_t* pGry, * pRbt;
    int dx, dy;
    int x, y;

    for (y = 0, pGry = pGryImg, pRbt = pRbtImg; y < height - 1; y++)
    {
        for (x = 0; x < width - 1; x++, pGry++)
        {
            dx = *pGry - *(pGry + width + 1);
            dy = *(pGry + 1) - *(pGry + width);
            *pRbt++ = (uint8_t)(dx > dy ? dx : dy); // 使用三目运算符选择较大的值
        }
        *pRbt++ = 0; // 尾列不做, 边缘强度赋0
        pGry++;
    }
    memset(pRbt, 0, width); // 尾行不做, 边缘强度赋0
}

//索贝尔算子
void RmwSobelGryImg(uint8_t* pGryImg, int width, int height, uint8_t* pSbImg)
{
    uint8_t* pGry, * pSb;
    int dx, dy;
    int x, y;

    memset(pSbImg, 0, width); // 首行不做, 边缘强度赋0
    for (y = 1, pGry = pGryImg + width, pSb = pSbImg + width; y < height - 1; y++)
    {
        *pSb++ = 0; // 首列不做, 边缘强度赋0
        pGry++;
        for (x = 1; x < width - 1; x++, pGry++)
        {
            // 求dx
            dx = *(pGry - 1 - width) + (*(pGry - 1) * 2) + *(pGry - 1 + width);
            dx -= *(pGry + 1 - width) + (*(pGry + 1) * 2) + *(pGry + 1 + width);
            // 求dy
            dy = *(pGry - width - 1) + (*(pGry - width) * 2) + *(pGry - width + 1);
            dy -= *(pGry + width - 1) + (*(pGry + width) * 2) + *(pGry + width + 1);
            // 结果
            *pSb++ = (uint8_t)min(255, abs(dx) + abs(dy));
        }
        *pSb++ = 0; // 尾列不做, 边缘强度赋0
        pGry++;
    }
    memset(pSb, 0, width); // 尾行不做, 边缘强度赋0
}

//Prewitt算子
void RmwPrewittGryImg(uint8_t* pGryImg, int width, int height, uint8_t* pPRTImg)
{
    uint8_t* pGry, * pPRT;
    int dx, dy, d45, d135, v1, v2;
    int x, y;

    memset(pPRTImg, 0, width); // 首行不做, 边缘强度赋0
    for (y = 1, pGry = pGryImg + width, pPRT = pPRTImg + width; y < height - 1; y++)
    {
        *pPRT++ = 0; // 首列不做, 边缘强度赋0
        pGry++;
        for (x = 1; x < width - 1; x++, pGry++)
        {
            // 求dx
            dx = *(pGry - 1 - width) + *(pGry - 1) + *(pGry - 1 + width);
            dx -= *(pGry + 1 - width) + *(pGry + 1) + *(pGry + 1 + width);
            // 求dy
            dy = *(pGry - width - 1) + *(pGry - width) + *(pGry - width + 1);
            dy -= *(pGry + width - 1) + *(pGry + width) + *(pGry + width + 1);
            // 求45度
            d45 = *(pGry - width - 1) + *(pGry - width) + *(pGry - 1);
            d45 -= *(pGry + width + 1) + *(pGry + width) + *(pGry + 1);
            // 求135度
            d135 = *(pGry - width) + *(pGry - width + 1) + *(pGry + 1);
            d135 -= *(pGry + width - 1) + *(pGry + width) + *(pGry - 1);
            // 结果
            v1 = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
            v2 = abs(d45) > abs(d135) ? abs(d45) : abs(d135);
            *pPRT++ = (uint8_t)((v1 > v2) ? ((v1 > 255) ? 255 : v1) : ((v2 > 255) ? 255 : v2));
        }
        *pPRT++ = 0; // 尾列不做, 边缘强度赋0
        pGry++;
    }
    memset(pPRT, 0, width); // 尾行不做, 边缘强度赋0
}

//沈俊算子
//pGryImg 和 pTmpImg 是指向 uint8_t 类型的指针，它们分别指向原始灰度图像数据和辅助图像数据。
//width 和 height 是整型参数，表示图像的宽度和高度。
//a0 是双精度浮点型参数，表示滤波系数。
//pSJImg 是指向 uint8_t 类型的指针，它指向了输出的图像数据。
void RmwShenJunGryImg(uint8_t* pGryImg,uint8_t* pTmpImg, int width, int height, double a0, uint8_t* pSJImg)
{
    uint8_t* pGry, * pCur, * pSJ, * pEnd;
    int LUT[512], * ALUT; // a0查找表
    int x, y, pre, dif;

    // Step 1: 初始化查找表
    a0 = (a0 < 0.01) ? 0.01 : ((a0 > 0.99) ? 0.99 : a0); // 安全性检查
    // a0查找表, 进行了四舍五入
    ALUT = LUT + 256;
    for (ALUT[0] = 0, dif = 1; dif < 256; dif++)
    {
        ALUT[dif] = (int)(dif * a0 + 0.5);
        ALUT[-dif] = (int)(-dif * a0 - 0.5);
    }

    // Step 2: 递推实现指数滤波
    // 按行滤波
    for (y = 0, pGry = pGryImg, pCur = pTmpImg; y < height; y++)
    {
        // 从左向右: p1(y,x) = p1(y,x-1) + a * [p(y,x) - p1(y,x-1)]
        *(pCur++) = pre = *(pGry++);
        for (x = 1; x < width; x++, pGry++)
            *(pCur++) = pre = pre + ALUT[*pGry - pre];
        pCur--; // 回到行尾
        // 从右向左: p2(y,x) = p2(y,x+1) - a * [p1(y,x) - p2(y,x+1)]
        for (x = width - 2, pCur = pCur - 1; x >= 0; x--)
            *(pCur--) = pre = pre + ALUT[*pCur - pre];
        pCur += (width + 1); // 回到下一行的开始
    }
    // 按列滤波
    for (x = 0, pCur = pTmpImg; x < width; x++, pCur = pTmpImg + x)
    {
        // 从上向下: p3(y,x) = p3(y-1,x) + a * [p2(y,x) - p3(y-1,x)]
        pre = *pCur;
        for (y = 1, pCur += width; y < height; y++, pCur += width)
            *pCur = pre = pre + ALUT[*pCur - pre];
        pCur -= width; // 回到列尾
        // 从下向上: p4(i,j) = p4(i+1,j) + a * [p3(i,j) - p4(i+1,j)]
        for (y = height - 2, pCur -= width; y >= 0; y--, pCur -= width)
            *pCur = pre = pre + ALUT[*pCur - pre];
    }

    // Step 3: 正导数=1，负导数为0，0必须也是0
    pEnd = pTmpImg + width * height;
    for (pCur = pTmpImg, pGry = pGryImg; pCur < pEnd; pGry++)
    {
        *(pCur++) = (*pCur > *pGry);
    }

    // Step 4: 过零点检测
    memset(pSJImg, 0, width * height); // 边缘强度赋0
    pSJ = pSJImg + width;
    pCur = pTmpImg + width; // 首行不做 
    for (y = 1; y < height - 1; y++)
    {
        pSJ++; pCur++;  // 首列不做
        for (x = 1; x < width - 1; x++, pGry++, pCur++, pSJ++)
        {
            if (*pCur) // 正导数
            {
                // 下面使用4邻域, 边缘为8连通, 不能保证4连通; 使用8邻域才能保证边缘4连通
                if ((!*(pCur - 1)) || // 左, 必须<=0, 不能<0
                    (!*(pCur + 1)) || // 右, 必须<=0, 不能<0
                    (!*(pCur - width)) || // 上, 必须<=0, 不能<0
                    (!*(pCur + width)))   // 下, 必须<=0, 不能<0
                {
                    *pSJ = 255; // 周围有导数小于等于0
                }
            }
        }
        pSJ++; pCur++;  // 尾列不做
    }
}

//沈俊算子加索贝尔算子
//pGryImg：指向原始灰度图像数据的指针
//pTmpImg：指向辅助图像数据的指针
//width：图像的宽度
//height：图像的高度
//a0：这是沈俊算子的参数，用于控制边缘检测的灵敏度。
//grdThre：这是Sobel算子的梯度阈值
//pEdgeImg：最终边缘图像数据的指针
void RmwExtractRiceEdge(uint8_t* pGryImg,uint8_t* pTmpImg,int width,int height,double a0, int grdThre, uint8_t* pEdgeImg)
{
    // step.1------------沈俊算子-----------------------//
    RmwShenJunGryImg(pGryImg, pTmpImg, width, height, a0, pEdgeImg);
    // step.2------------Sobel算子----------------------//
    RmwSobelGryImg(pGryImg, width, height, pTmpImg);
    // step.3------------二者融合-----------------------//
    for (int i = 0; i < width * height; i++)
    {
        *(pEdgeImg + i) = (pEdgeImg[i] && (pTmpImg[i] > grdThre)) * 255;
    }
    // step.4------------结束---------------------------//
    return;
}

////显示直方图
////histogram：直方图数据指针
////size：直方图数据大小
////a、b、c：直方图的三个参数,亮度，对比度，中值
//void displayHistogram(const int* histogram, int size, double a, double b, double c)
//{
//
//    SDL_Init(SDL_INIT_VIDEO);
//
//    char windowTitle[100];
//    sprintf(windowTitle, "Histogram      Brightness: %.2f  Contrast: %.2f  Median: %.2f", a, b, c);
//    SDL_Window* window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 256, SDL_WINDOW_SHOWN);
//    //窗口名称为直方图，接下来两个参数是窗口在屏幕上的位置，分别在水平和垂直都居中显示，窗口的宽度和高度目前按时512和512，这个后面再调整，再后面一个参数的意思是创建窗口立即显示
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

//计算直方图中间值
//histogram：直方图数据指针
//width、height：图像的宽度和高度
double Median(const int* histogram, int width, int height)
{
    int size = width * height;
    int cumulativeSum = 0;
    int middleElement = (size + 1) / 2;

    for (int i = 0; i < 256; ++i)
    {
        cumulativeSum += histogram[i];
        if (cumulativeSum >= middleElement)
        {
            return i;
        }
    }

    return 0;
}


// 打印链表元素
void printList(struct Node* head) {
    struct Node* temp = head;  // 从头节点开始遍历链表
    while (temp != NULL) {
        printf("%d ", temp->data);  // 输出当前节点的数据
        temp = temp->next;           // 移动到下一个节点
    }
    printf("\n");
}

void insertAtEnd(struct Node** head, int value) 
{
    // 创建一个新节点
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = value;  // 设置新节点的数据
    newNode->next = NULL;   // 将新节点的next指针置为NULL

    // 如果链表为空，则新节点即为头节点
    if (*head == NULL) {
        *head = newNode;
    }
    else {
        // 找到链表尾部节点
        struct Node* temp = *head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        // 将新节点链接到尾部节点后面
        temp->next = newNode;
    }
}

int generateChainCode(uint8_t* image, int width, int height, int x, int y, int flag, struct Node** head, struct Node** tail) 
{
    printf("%d ", flag);
    int flag1 = 0;
    int x1 = x;
    int y1 = y;
    if (flag == 0)
    {
        if (flag1 == 1 && x == x1 && y == y1)
            return head;

        if ((image[y  * width / 8 + (x+1) / 8] >> (7 - ((x+1) % 8)) & 1 ) == 0)
        {
            flag = 0;
            insertAtEnd(head, 0);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y, flag, head, tail);
        }
        else if((image[(y + 1) * width / 8 + (x+1) / 8] >> (7 - ((x+1) % 8)) & 1) == 0)
        {
            flag = 1;
            insertAtEnd(head, 1);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 2;
            insertAtEnd(head, 2);
            flag1 = 1;
            generateChainCode(image, width, height, x, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x-1) / 8] >> (7 - ((x-1) % 8)) & 1) == 0)
        {
            flag = 3;
            insertAtEnd(head, 3);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y + 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 5;
            insertAtEnd(head, 5);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 6;
            insertAtEnd(head, 6);
            flag1 = 1;
            generateChainCode(image, width, height, x, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 7;
            insertAtEnd(head, 7);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y - 1, flag, head, tail);
        } 
        else
            return 0;
    }
    if (flag == 2)
    {
        if (flag1 == 1 && x == x1 && y == y1)
            return head;

        if ((image[y * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 0;
            insertAtEnd(head, 0);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 1;
            insertAtEnd(head, 1);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 2;
            insertAtEnd(head, 2);
            flag1 = 1;
            generateChainCode(image, width, height, x, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 3;
            insertAtEnd(head, 3);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y + 1, flag, head, tail);
        }
        else if ((image[y * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 4;
            insertAtEnd(head, 4);
            generateChainCode(image, width, height, x - 1, y, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 5;
            insertAtEnd(head, 5);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 7;
            insertAtEnd(head, 7);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y - 1, flag, head, tail);
        }
        else
            return 0;
    }
    if (flag == 4)
    {
        if (flag1 == 1 && x == x1 && y == y1)
            return head;

        if ((image[(y + 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 1;
            insertAtEnd(head, 1);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 2;
            insertAtEnd(head, 2);
            flag1 = 1;
            generateChainCode(image, width, height, x, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 3;
            insertAtEnd(head, 3);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y + 1, flag, head, tail);
        }
        else if ((image[y * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 4;
            insertAtEnd(head, 4);
            generateChainCode(image, width, height, x - 1, y, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 5;
            insertAtEnd(head, 5);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 6;
            insertAtEnd(head, 6);
            flag1 = 1;
            generateChainCode(image, width, height, x, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 7;
            insertAtEnd(head, 7);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y - 1, flag, head, tail);
        }
        else
            return 0;
    }
    if (flag == 6)
    {
        if (flag1 == 1 && x == x1 && y == y1)
            return head;

        if ((image[y * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 0;
            insertAtEnd(head, 0);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 1;
            insertAtEnd(head, 1);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 3;
            insertAtEnd(head, 3);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y + 1, flag, head, tail);
        }
        else if ((image[y * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 4;
            insertAtEnd(head, 4);
            generateChainCode(image, width, height, x - 1, y, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 5;
            insertAtEnd(head, 5);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 6;
            insertAtEnd(head, 6);
            flag1 = 1;
            generateChainCode(image, width, height, x, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 7;
            insertAtEnd(head, 7);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y - 1, flag, head, tail);
        }
        else
            return 0;
    }
    if (flag == 1)
    {
        if (flag1 == 1 && x == x1 && y == y1)
            return head;

        if ((image[y * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 0;
            insertAtEnd(head, 0);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 1;
            insertAtEnd(head, 1);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 2;
            insertAtEnd(head, 2);
            flag1 = 1;
            generateChainCode(image, width, height, x, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 3;
            insertAtEnd(head, 3);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y + 1, flag, head, tail);
        }
        else if ((image[y * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 4;
            insertAtEnd(head, 4);
            generateChainCode(image, width, height, x - 1, y, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 6;
            insertAtEnd(head, 6);
            flag1 = 1;
            generateChainCode(image, width, height, x, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 7;
            insertAtEnd(head, 7);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y - 1, flag, head, tail);
        }
        else
            return 0;
    }
    if (flag == 3)
    {
        if (flag1 == 1 && x == x1 && y == y1)
            return head;

        if ((image[y * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 0;
            insertAtEnd(head, 0);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 1;
            insertAtEnd(head, 1);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 2;
            insertAtEnd(head, 2);
            flag1 = 1;
            generateChainCode(image, width, height, x, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 3;
            insertAtEnd(head, 3);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y + 1, flag, head, tail);
        }
        else if ((image[y * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 4;
            insertAtEnd(head, 4);
            generateChainCode(image, width, height, x - 1, y, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 5;
            insertAtEnd(head, 5);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 6;
            insertAtEnd(head, 6);
            flag1 = 1;
            generateChainCode(image, width, height, x, y - 1, flag, head, tail);
        }
        else
            return 0;
    }
    if (flag == 5)
    {
        if (flag1 == 1 && x == x1 && y == y1)
            return head;

        if ((image[y * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 0;
            insertAtEnd(head, 0);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 2;
            insertAtEnd(head, 2);
            flag1 = 1;
            generateChainCode(image, width, height, x, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 3;
            insertAtEnd(head, 3);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y + 1, flag, head, tail);
        }
        else if ((image[y * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 4;
            insertAtEnd(head, 4);
            generateChainCode(image, width, height, x - 1, y, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 5;
            insertAtEnd(head, 5);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 6;
            insertAtEnd(head, 6);
            flag1 = 1;
            generateChainCode(image, width, height, x, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 7;
            insertAtEnd(head, 7);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y - 1, flag, head, tail);
        }
        else
            return 0;
    }
    if (flag == 7)
    {
        if (flag1 == 1 && x == x1 && y == y1)
            return head;

        if ((image[y * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 0;
            insertAtEnd(head, 0);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 1;
            insertAtEnd(head, 1);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y + 1, flag, head, tail);
        }
        else if ((image[(y + 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 2;
            insertAtEnd(head, 2);
            flag1 = 1;
            generateChainCode(image, width, height, x, y + 1, flag, head, tail);
        }
        else if ((image[y * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 4;
            insertAtEnd(head, 4);
            generateChainCode(image, width, height, x - 1, y, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x - 1) / 8] >> (7 - ((x - 1) % 8)) & 1) == 0)
        {
            flag = 5;
            insertAtEnd(head, 5);
            flag1 = 1;
            generateChainCode(image, width, height, x - 1, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + x / 8] >> (7 - (x % 8)) & 1) == 0)
        {
            flag = 6;
            insertAtEnd(head, 6);
            flag1 = 1;
            generateChainCode(image, width, height, x, y - 1, flag, head, tail);
        }
        else if ((image[(y - 1) * width / 8 + (x + 1) / 8] >> (7 - ((x + 1) % 8)) & 1) == 0)
        {
            flag = 7;
            insertAtEnd(head, 7);
            flag1 = 1;
            generateChainCode(image, width, height, x + 1, y - 1, flag, head, tail);
        }
        else
            return 0;
    }
}

//链码生成
//image：图像数据指针
//width、height：图像的宽度和高度
// 修改后的链码生成函数
struct Node* ChainCode(uint8_t* image, int width, int height)
{
    int size = width * height;
    int index = 0;
    int flag = -1;

    int firstx = -1;
    int firsty = -1;

    printf("图像大小: %d x %d\n", width, height);

    struct Node* head = NULL;
    printf("\n");

    int found = 0; // 标志变量，用于控制外层循环

    for (int i = 0; i < height && !found; ++i) {
        for (int j = 0; j < width; ++j) {
            // 获取每个像素的值
            uint8_t pixel = (image[i * width / 8 + j / 8] >> (7 - (j % 8))) & 1;
            //[i * width / 8 + j / 8]找到对应的字节
           // >> (7 - (j % 8)) 找到bit在字节中的位置
           // & 1 其他位都附0，保留当前值
           // printf("%d ", pixel);
            if (pixel == 0) {
                if (flag == -1) {
                    flag = 0;
                    firstx = j;
                    firsty = i;
                    // 生成链码
                    uint32_t img = (width * height + 7) / 8;  // 计算图像数据所需的字节数
                    uint8_t* imgcopy = (uint8_t*)malloc(img);
                    memcpy(imgcopy, image, img);
                    generateChainCode(imgcopy, width, height, j, i,0, &head, &head);
                    printf("第一个目标像素点为：(%d, %d)", firstx, firsty);
                    printList(head);
                    found = 1; // 设置标志变量，用于终止外层循环
                }
                break; // 跳出内层循环
            }
        }
        //printf("\n");
    }

    return head;
}


//读取1位灰度图片
//filename：字符数组的指针，用于指定要保存的图像文件的名称或路径。
//imageData：无符号 8 位整型数据的指针，代表要保存的图像数据。
//width：图像的宽度。
//height：图像的高度。
uint8_t* read1BitBMP(const char* filename, int* width, int* height)
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
    uint32_t imageDataSize = (*width * *height + 7) / 8;  // 计算图像数据所需的字节数
    uint8_t* imageData = (uint8_t*)malloc(imageDataSize);
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
    fseek(file, *(int*)&bmpHeader[10], SEEK_SET);  // 使用正确的偏移量
    fread(imageData, 1, imageDataSize, file);  // 读取正确大小的图像数据

    fclose(file);

    return imageData;
}
