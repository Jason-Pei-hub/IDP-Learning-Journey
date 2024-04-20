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
void RmwDoSumGryImg_SSE(uint8_t* pGryImg,int width,int height,int* pSumImg)
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
            *(pRes++) = (sum >= sumThreshold) * 255; //请理解这个表达式的含义
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