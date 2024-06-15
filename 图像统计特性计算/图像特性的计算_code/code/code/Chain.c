#define _CRT_SECURE_NO_WARNINGS 1


#include "IDP.h"


int main() 
{
    int width, height;
    uint8_t* imageData = read1BitBMP("input/链码跟踪实验图像.bmp", &width, &height);

    printf("图像宽度：%d\n", width);
    printf("图像高度：%d\n", height);


    ChainCode(imageData, width, height);

   
    printf("程序结束\n");
    free(imageData);

    return 0;
}

