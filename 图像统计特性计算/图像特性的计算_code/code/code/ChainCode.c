#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>

#define IMAGE_WIDTH  512  // 图像宽度
#define IMAGE_HEIGHT 512  // 图像高度

// 定义链码方向
enum Direction {
    N = 0, NE, E, SE, S, SW, W, NW
};

// 定义链码结构体
typedef struct {
    int x;
    int y;
    int direction;
} ChainCode;

// 读取BMP图像
void readBMP(const char* filename, int image[IMAGE_HEIGHT][IMAGE_WIDTH]) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("无法打开文件：%s\n", filename);
        exit(1);
    }

    // 读取BMP文件头
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file);

    // 获取图像宽度和高度
    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    if (width != IMAGE_WIDTH || height != IMAGE_HEIGHT) {
        printf("图像尺寸不正确：%dx%d\n", width, height);
        exit(1);
    }

    // 读取图像像素数据
    unsigned char data[IMAGE_HEIGHT][IMAGE_WIDTH];
    fread(data, sizeof(unsigned char), IMAGE_WIDTH * IMAGE_HEIGHT, file);

    // 将像素数据转换为灰度图像
    for (int y = 0; y < IMAGE_HEIGHT; y++) {
        for (int x = 0; x < IMAGE_WIDTH; x++) {
            image[y][x] = data[y][x];
        }
    }

    fclose(file);
}

// 获取像素值
int getPixelValue(int image[IMAGE_HEIGHT][IMAGE_WIDTH], int x, int y) {
    // 检查坐标是否越界
    if (x < 0 || x >= IMAGE_WIDTH || y < 0 || y >= IMAGE_HEIGHT) {
        return 0;  // 返回默认值
    }
    return image[y][x];
}

// 链码追踪
void traceContour(int image[IMAGE_HEIGHT][IMAGE_WIDTH], int start_x, int start_y, ChainCode chainCode[], int* chainCodeLength) {
    // 初始化链码长度
    *chainCodeLength = 0;

    // 初始化当前位置
    int curr_x = start_x;
    int curr_y = start_y;

    // 初始化当前方向
    int curr_direction = NE;

    // 进行链码追踪
    do {
        // 记录当前位置和方向
        chainCode[*chainCodeLength].x = curr_x;
        chainCode[*chainCodeLength].y = curr_y;
        chainCode[*chainCodeLength].direction = curr_direction;

        // 更新链码长度
        (*chainCodeLength)++;

        // 根据当前方向选择下一个像素点
        switch (curr_direction) {
        case N:
            curr_y--;
            break;
        case NE:
            curr_x++;
            curr_y--;
            break;
        case E:
            curr_x++;
            break;
        case SE:
            curr_x++;
            curr_y++;
            break;
        case S:
            curr_y++;
            break;
        case SW:
            curr_x--;
            curr_y++;
            break;
        case W:
            curr_x--;
            break;
        case NW:
            curr_x--;
            curr_y--;
            break;
        }

        // 根据八方向链码模板选择下一个方向
        int next_direction = (curr_direction + 1) % 8;
        int min_pixel_value = getPixelValue(image, curr_x, curr_y);
        for (int i = 0; i < 8; i++) {
            int x = curr_x;
            int y = curr_y;
            int pixel_value = getPixelValue(image, x, y);

            // 寻求最小像素值的方向
            if (pixel_value < min_pixel_value) {
                min_pixel_value = pixel_value;
                next_direction = (curr_direction + i) % 8;
            }

            // 更新下一个像素点的坐标
            switch (next_direction) {
            case N:
                y--;
                break;
            case NE:
                x++;
                y--;
                break;
            case E:
                x++;
                break;
            case SE:
                x++;
                y++;
                break;
            case S:
                y++;
                break;
            case SW:
                x--;
                y++;
                break;
            case W:
                x--;
                break;
            case NW:
                x--;
                y--;
                break;
            }

            // 更新当前方向
            curr_direction = next_direction;
            curr_x = x;
            curr_y = y;
        }
    } while (curr_x != start_x || curr_y != start_y);  // 直到回到起始点为止
}

// 输出链码
void printChainCode(ChainCode chainCode[], int chainCodeLength) {
    printf("Chain Code: ");
    for (int i = 0; i < chainCodeLength; i++) {
        printf("%d ", chainCode[i].direction);
    }
    printf("\n");
}

// 计算链码的首差循环链码
void computeCircularCode(ChainCode chainCode[], int chainCodeLength) {
    printf("Circular Code: ");
    for (int i = 0; i < chainCodeLength - 1; i++) {
        int circularCode = (chainCode[i + 1].direction - chainCode[i].direction + 8) % 8;
        printf("%d ", circularCode);
    }
    printf("%d\n", (chainCode[0].direction - chainCode[chainCodeLength - 1].direction + 8) % 8);
}

// 计算链码的最小首差循环链码
void computeMinimalCircularCode(ChainCode chainCode[], int chainCodeLength) {
    int minCircularCode = chainCode[0].direction;
    int minIndex = 0;

    // 找到最小的首差循环链码
    for (int i = 1; i < chainCodeLength - 1; i++) {
        int circularCode = (chainCode[i + 1].direction - chainCode[i].direction + 8) % 8;
        if (circularCode < minCircularCode) {
            minCircularCode = circularCode;
            minIndex = i;
        }
    }

    // 输出最小的首差循环链码
    printf("Minimal Circular Code: ");
    for (int i = minIndex; i < chainCodeLength - 1; i++) {
        int circularCode = (chainCode[i + 1].direction - chainCode[i].direction + 8) % 8;
        printf("%d ", circularCode);
    }
    for (int i = 0; i < minIndex; i++) {
        int circularCode = (chainCode[i + 1].direction - chainCode[i].direction + 8) % 8;
        printf("%d ", circularCode);
    }
    printf("\n");
}

int main() 
{
    int image[IMAGE_HEIGHT][IMAGE_WIDTH];  // 存储图像像素值的数组
    const char* filename = "input/链码跟踪实验图像.bmp";    // 图像文件名

    // 读取BMP图像
    readBMP(filename, image);

    // 链码追踪
    int start_x = 0;  // 设置起始点的坐标
    int start_y = 0;
    ChainCode chainCode[IMAGE_WIDTH * IMAGE_HEIGHT];
    int chainCodeLength;
    traceContour(image, start_x, start_y, chainCode, &chainCodeLength);

    // 输出链码
    printChainCode(chainCode, chainCodeLength);

    // 计算链码的首差循环链码
    computeCircularCode(chainCode, chainCodeLength);

    // 计算链码的最小首差循环链码
    computeMinimalCircularCode(chainCode, chainCodeLength);

    return 0;
}