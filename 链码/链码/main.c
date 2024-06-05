#include <stdio.h>
#include <stdlib.h>

// 定义链码方向
enum Direction {
    N = 0, NE, E, SE, S, SW, W, NW
};

// 读取BMP图像
void readBMP(const char* filename, int* width, int* height) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("无法打开文件：%s\n", filename);
        exit(1);
    }

    // 读取BMP文件头
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file);

    // 获取图像宽度和高度
    *width = *(int*)&header[18];
    *height = *(int*)&header[22];

    fclose(file);
}

// 获取像素值
int getPixelValue(const char* filename, int x, int y) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("无法打开文件：%s\n", filename);
        exit(1);
    }

    // 跳过BMP文件头
    fseek(file, 54, SEEK_SET);

    // 定位到指定像素位置
    fseek(file, (x + y * 512) * sizeof(unsigned char), SEEK_CUR);

    // 读取像素值
    unsigned char pixel_value;
    fread(&pixel_value, sizeof(unsigned char), 1, file);

    fclose(file);

    return pixel_value;
}

// 输出链码
void printChainCode(const char* chain_code_filename) {
    FILE* chain_code_file = fopen(chain_code_filename, "r");
    if (chain_code_file == NULL) {
        printf("无法打开文件：%s\n", chain_code_filename);
        exit(1);
    }

    int code;
    while (fscanf_s(chain_code_file, "%d", &code) != EOF) {
        printf("%d ", code);
    }

    fclose(chain_code_file);
}

// 输出首差循环链码
void printFirstDiffCycleCode(const char* chain_code_filename) {
    FILE* chain_code_file = fopen(chain_code_filename, "r");
    if (chain_code_file == NULL) {
        printf("无法打开文件：%s\n", chain_code_filename);
        exit(1);
    }

    int code;
    int prev_code;
    fscanf_s(chain_code_file, "%d", &prev_code);  // 读取第一个链码
    printf("%d ", prev_code);

    while (fscanf_s(chain_code_file, "%d", &code) != EOF) {
        int diff = (code - prev_code + 8) % 8;
        printf("%d ", diff);
        prev_code = code;
    }

    fclose(chain_code_file);
}

// 输出最小首差循环链码
void printMinFirstDiffCycleCode(const char* chain_code_filename) {
    FILE* chain_code_file = fopen(chain_code_filename, "r");
    if (chain_code_file == NULL) {
        printf("无法打开文件：%s\n", chain_code_filename);
        exit(1);
    }

    int code;
    int prev_code;
    fscanf_s(chain_code_file, "%d", &prev_code);  // 读取第一个链码

    // 寻找最小首差
    int min_diff = 8;
    int min_diff_code = prev_code;

    while (fscanf_s(chain_code_file, "%d", &code) != EOF) {
        int diff = (code - prev_code + 8) % 8;
        if (diff < min_diff) {
            min_diff = diff;
            min_diff_code = code;
        }
        prev_code = code;
    }

    fclose(chain_code_file);

    // 输出最小首差循环链码
    chain_code_file = fopen_s(&chain_code_file, chain_code_filename, "r");
    if (chain_code_file == NULL) {
        printf("无法打开文件：%s\n", chain_code_filename);
        exit(1);
    }

    // 定位到最小首差的位置
    while (fscanf_s(chain_code_file, "%d", &code) != EOF) {
        if (code == min_diff_code) {
            printf("%d ", code);
            break;
        }
    }

    while (fscanf_s(chain_code_file, "%d", &code) != EOF) {
        int diff = (code - prev_code + 8) % 8;
        printf("%d ", diff);
        prev_code = code;
    }

    fclose(chain_code_file);
}

int main() {
    const char* chain_code_filename = "chaincode.txt";

    printf("链码: ");
    printChainCode(chain_code_filename);
    printf("\n");

    printf("首差循环链码: ");
    printFirstDiffCycleCode(chain_code_filename);
    printf("\n");

    printf("最小首差循环链码: ");
    printMinFirstDiffCycleCode(chain_code_filename);
    printf("\n");

    return 0;
}