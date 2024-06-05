#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "bmpFile.h"
#include "RmwALG_ContourTrace.h"
#include "RmwALG_ContourFill.h"

///////////////////////////////////////////////////////////////////////////////////////
//
//  全局变量
//
///////////////////////////////////////////////////////////////////////////////////////
#define MAX_CONOUR_LENGTH  (512*512)
BYTE gChainCode[MAX_CONOUR_LENGTH];

void testTrace(BYTE*pBinImg,int width,int height, BYTE *pChainCode,BYTE *pDbgImg)
{
	BYTE *pCur;
	int x, y;
	int N,S;
	int x1, x2, y1, y2;

	//调试
	memset(pDbgImg, 0, width*height);
	// step.1-----为了避免跟踪时的超界检测,首先将图像边框赋0-------//
	RmwSetImageBoundary(pBinImg, width, height, 0);
	// step.2-----从上向下,从左向右扫描图像，寻找新的轮廓----------//
	for (y = 1, pCur = pBinImg+y*width; y<height-1; y++) //从上向下
	{
		pCur++; //跳过最左侧点
		for (x = 1; x<width-1; x++, pCur++) //从左向右
		{
			if ( (*pCur==255)&& (*(pCur-1)<=1) ) //发现一条外轮廓
			{   //调用轮廓跟踪算法,进行外轮廓跟踪
				N = RmwTraceContour( pBinImg, width, height,
					                 x, y, //轮廓起点
					                 true, //是外轮廓
					                 pChainCode, //用来存放链码的数组
					                 MAX_CONOUR_LENGTH //数组的大小
				                   );
				//面积
				S = RmwContourPixels(pChainCode, N);
				printf("\nS=%d", S);
				//外接矩形
				RmwContourRect(x, y, pChainCode, N, &x1, &x2, &y1, &y2);
				if ( (x1>1)&&(y1>1)&&(x2<width-2)&&(y2<height-2)&& //去掉贴边者
					 (S>200)&& //去掉残缺者 
					 (S<400) //去掉粘连者
				   )
				{
					RmwFillContour( pDbgImg, width, height,//待填充图像
						            x, y, //轮廓起点
						            true, //是否是外轮廓
						            pChainCode, //链码
						            N, //链码个数
						            255, //区域内部的颜色
						            255, //轮廓点颜色
						            250 //不存在的颜色
					              );
				}
				else RmwDrawContour(pDbgImg, width, x, y, pChainCode, N, 253);
			}
			else if ( (*pCur==0)&& (*(pCur-1)>=254) ) //发现一条内轮廓
			{   //调用轮廓跟踪算法,进行内轮廓跟踪
				N = RmwTraceContour( pBinImg, width, height,
					                 x-1, y,  //轮廓起点,注意是x-1
					                 false, //是内轮廓
					                 pChainCode, //用来存放链码的数组
					                 MAX_CONOUR_LENGTH //数组的大小
				                   );
				//调试
				RmwDrawContour(pDbgImg, width, x-1, y, pChainCode, N, 253);
			}
		}
		pCur++; //跳过最右侧点
	}
}

void main()
{
	unsigned char *pBinImg;
	int width, height;
	int i;

	// step.1-------------读图像文件------------------------//
	pBinImg = Rmw_Read8BitBmpFile2Img("..\\RmwCPP0603\\Fig0520h.bmp", &width, &height);
	if (!pBinImg)
	{
		printf("*file open err!\n");
		getchar();
		return;
	}
	// step.2-------------处理图像--------------------------//
	BYTE *pDbgImg = new BYTE[width*height];
	testTrace(pBinImg, width, height, gChainCode, pDbgImg);
 	// step.3-------------保存图像--------------------------//
	Rmw_Write8BitImg2BmpFileMark(pDbgImg, width, height, "..\\RmwCPP0603\\Fig0520h_traceAndFill.bmp");
	//可用于3x3的快速膨胀
	for (i = 0; i<width*height; i++)
	{
		pDbgImg[i] = (pBinImg[i]>200)*255;
	}
	Rmw_Write8BitImg2BmpFile(pDbgImg, width, height, "..\\RmwCPP0603\\Fig0520h_膨胀.bmp");
	//可用于3x3的快速腐蚀
	for (i = 0; i<width*height; i++)
	{
		pDbgImg[i] = (pBinImg[i]==255)*255;
	}
	Rmw_Write8BitImg2BmpFile(pDbgImg, width, height, "..\\RmwCPP0603\\Fig0520h_腐蚀.bmp");
	// step.4-------------结束------------------------------//
	delete pBinImg; //释放自己申请的内存
	delete pDbgImg; //释放自己申请的内存
	printf("\nPress any key to exit!\n");
	getchar();
	return;
}
