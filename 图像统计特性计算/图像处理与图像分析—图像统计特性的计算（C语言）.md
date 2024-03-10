# 图像处理与图像分析—图像统计特性的计算（C语言）



> 根据输入的灰度图像，分别计算图像的均值、方差等统计特征，并计算图像的直方图特征并以图形方式显示图像的直方图（用C或C++语言实现）。
>
> 学习将会依据教材图像处理与图像分析基础（C/C++）版内容展开

在上个笔记中，我们成功对bmp图像进行了读取，那么这次我们将会尝试计算图像的均值、方差等统计特征，并计算图像的直方图特征并以图形方式显示图像的直方图

**首先分析一下我们需要的功能：**

* 能够读取图片
* 分析图片每一个像素的灰度值
* 把每一个像素的灰度值存入灰度数组
* 用直方图的形式进行展示
* 计算图像的均值，方差

那么上笔记的代码已经可以实现：图片的读取，分析灰度值，那么我们只需要再加上后面三个功能即可

#### 首先我们还是先来分析书上的代码

算法【2-2】统计灰度直方图

```c
void GetHistogram(BYTE* pGryImg, int width, int height, int* histogram)
{
	BYTE * pCur, * pEnd = pGryImg + width * height;
	//step.1 初始化
	//for(int g = 0;g < 256;g++) histogram[g] = 0;
	memset(histogram, 0, sizeof(int) * 256);
	//step.2 直方图统计
	for (pCur = pGryImg; pCur < pEnd;)histogram[*(pCur++)]++;
	//step.3 结束
	return;
}
```

还是先让我来浅浅的注释一下，看看能理解多少

```c

//pGryImg: 指向灰度图像数据的指针
//width: 图像宽度
//height: 图像高度
//histogram: 存储直方图数据的数组
void GetHistogram(BYTE* pGryImg, int width, int height, int* histogram) {
    BYTE *pCur, *pEnd = pGryImg + width * height; // 定义指针变量并指向图像数据起始位置和结束位置

    // Step 1: 初始化直方图数组
    // 使用memset函数将直方图数组所有元素初始化为0
    memset(histogram, 0, sizeof(int) * 256);

    // Step 2: 直方图统计
    // 遍历灰度图像数组，对每个灰度值出现的次数进行统计
    for (pCur = pGryImg; pCur < pEnd;) {
        histogram[*(pCur++)]++; // 获取当前像素的灰度值并递增对应直方图计数
    }

    // Step 3: 函数结束
    return;
}
```

不是很难，可以试一下，首先还是写成C语言

```c
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
        pCur++；
    }

    // 函数结束
    return;
}
```



> 我们用之前写好的代码(图片的读入中的源代码)稍稍修改一下

还是使用这张图片

![ME](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101335144.bmp)

传入参数少了一个histogram，我们需要创建一个histogram数组

```c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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
        pCur++；
    }

    // 函数结束
    return;
}

int main()
{
    int histogram[256];
    //建立一个数组
    const char* filename = "E:/code/IDP-Learning-Journey/images/ME.bmp";

    FILE* file = fopen(filename, "rb");

    uint8_t bmpHeader[54];
    size_t bytesRead = fread(bmpHeader, 1, 54, file);

    int width = *(int*)&bmpHeader[18];   // 宽度信息位于偏移量为 18 的位置
    int height = *(int*)&bmpHeader[22];  // 高度信息位于偏移量为 22 的位置

    uint8_t* imageData = (uint8_t*)malloc(width * height); 

    fseek(file, 54, SEEK_SET);  // 跳过 BMP 文件头

    bytesRead = fread(imageData, 1, width * height, file); 

    GetHistogram(imageData, width, height，histogram);
    
    	for (int i = 0; i < 256; i++)
	{
    	printf("histogram[%d]: %d\n", i, histogram[i]);
	}//打印出来看看结果是否成功

    fclose(file);
    free(imageData);

    return 0;
}
```

![image-20240309230227278](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403092302332.png)

**成功实现预期功能，接下来查找如何显示为直方图**

通过我的不懈努力，变成了这样

![image-20240309230633643](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101916938.png)

很显然，还是没达到要求，这个时候只能网上查一下了

好像解决办法很少，但是还是查到了一个头文件，可以解决C语言生成图片的问题

> #include <SDL.h>
>
> SDL（Simple DirectMedia Layer）是一个跨平台的开源多媒体库，用于实现音频、视频、图形和输入设备的访问。SDL提供了一套简单而强大的接口，使得开发者能够在不同平台上编写高性能的多媒体应用程序，而不需要关注底层的细节。

我们用不到那么多功能，只需要能打开一个图形化页面就行

---

## 配置SDL

首先需要去GitHub上找到相应的包克隆到本地

[Release 2.30.1 · libsdl-org/SDL (github.com)](https://github.com/libsdl-org/SDL/releases/tag/release-2.30.1)

![image-20240310121243846](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101212897.png)

选择对应的版本，要区分mingw和MSVC编译器，后者也就是vs的编译器

下载到一个能找到的位置解压缩

![image-20240310121528238](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101215265.png)

这里我演示一下如何配置vs2022的环境，如果是mingw，国内很多开源社区都有教程，就不过多去演示了，参考官方文档

#### step.1 点击项目中的属性

#### ![image-20240310121717504](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101217537.png)

#### step.2 修改VC++目录下的包含目录为解压目录下的include文件夹的地址

![image-20240310121935263](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101219297.png)

#### step.3 修改库目录为解压目录下lib地址

![image-20240310122117131](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101221166.png)

#### step.4 在链接器选择输入，修改附加依赖项为lib目录下lib后缀文件的名称

![image-20240310122305329](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101223371.png)

#### step5 修改环境变量

首先需要添加lib目录下64位文件夹的地址

![image-20240310122451516](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101224550.png)

接着是我实验发现，总是会报找不到SDL2.dll这个文件的错误，导致程序无法编译，我尝试把这个文件直接放在.c文件的目录下，可以解决问题，但是以后不想这么麻烦，所以我们把这个文件也添加系统变量

![image-20240310122744885](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101227916.png)

问题解决，编译器可以编译，我们现在让chatgpt写一段小程序验证一下

![image-20240310122943972](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101229037.png) 

> 测试代码

```c
#include <stdio.h>
#include <SDL.h>

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    // 创建窗口
    SDL_Window* window = SDL_CreateWindow("SDL Example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);

    // 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // 设置绘制颜色为红色
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // 清空屏幕
    SDL_RenderClear(renderer);

    // 创建一个矩形
    SDL_Rect rect = { 100, 100, 200, 150 };

    // 填充矩形
    SDL_RenderFillRect(renderer, &rect);

    // 更新屏幕
    SDL_RenderPresent(renderer);

    // 等待2秒
    SDL_Delay(2000);

    // 清理资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

```

这样就配置成功了

---

## SDL是如何绘图的

> 这个问题很重要，如果不搞清楚我们将无法绘制灰度值直方图

查了一下，分为以下几步

* **初始化sdl**

  > 这一步是为了指定要使用的SDL子系统，比如视频子系统、音频子系统、定时器子系统等。可以选择仅初始化所需的子系统，以提高性能和减少资源占用。* 

```c
SDL_Init(SDL_INIT_VIDEO);//我们使用的画图窗口用的是视频子系统
```

* 创建窗口

  > 创建一个来展示我们直方图数据的窗口

```c
SDL_Window* window = SDL_CreateWindow("Histogram", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 512, SDL_WINDOW_SHOWN);//窗口名称为直方图，接下来两个参数是窗口在屏幕上的位置，分别在水平和垂直都居中显示，窗口的宽度和高度目前按时512和512，这个后面再调整，再后面一个参数的意思是创建窗口立即显示
```

* 创建渲染器

  > 这一步不是很能理解，在我的理解里应该是类似于创建一个画板，可以在窗口里作图的功能

```c
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);//window为要在其上创建渲染器的窗口指针，-1：指定要使用的渲染器索引。通常将其设置为-1，以便SDL自动选择可用的索引。SDL_RENDERER_ACCELERATED：指定渲染器使用硬件加速，以提高性能。
```

* 设置背景颜色

  ```c
      SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//设置绘制颜色设置为黑色（RGB值为0, 0, 0，表示红、绿、蓝通道均为0，即黑色），同时将透明度设置为255（不透明）
  
      SDL_RenderClear(renderer);//然后通过SDL_RenderClear函数清空渲染器，实际上是在规定背景颜色。
  ```

  > 在使用SDL进行图形绘制时，首先通常会设置绘制颜色，然后使用`SDL_RenderClear`函数填充整个渲染目标（通常是窗口）的背景色。

* 绘制直方图

  > 我尝试了使用for循环每次改变x坐标，y坐标为数组中的值，但是好像失败了

![image-20240310130657225](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101306256.png)

先看代码

```c
 int barWidth = 2;// 设置每个直方条的宽度

 for (int i = 0; i < size; i++) {

     SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//设置白色

     // 计算直方条的位置和高度
     int barX = i * (barWidth + 1);
     int barY = histogram[i];

     
     SDL_Rect bar = { barX, barY, barWidth, (int)(histogram[i]) };
     SDL_RenderFillRect(renderer, &bar);
 }// 绘制

 SDL_RenderPresent(renderer); // 更新renderer
```

难道是因为，直方图太小了直接看不到吗，如果限定一个高度区间会不会好一点

我思路是这样的

1. 找到灰度值的最大值
2. 用规定的最大值去比上灰度值的最大值得到一个比例
3. 这样所有的灰度值都乘上这个比例，就可以等比例缩放了

ok，用for循环找最大值和最小值

```c
    int maxBarHeight = 0;
    int minBarHeight = 0;

    for (int i = 0; i < size; i++) 
    {
        if (histogram[i] > maxBarHeight) 
        {
            maxBarHeight = histogram[i];
        }
    }

    for (int i = 0; i < size; i++) 
    {
        if (histogram[i] < minBarHeight) 
        {
            minBarHeight = histogram[i];
        }
    }
```

这样子就有最大值和最小值了，我们的最大值设定为400，因为窗口为512，

宽度还为2，刚好填满窗口

```c
int barWidth = 2;
double k = (double)(400) / maxBarHeight;//设比值为k，定义双精度浮点数400，防止整数相除默认算整数

for (int i = 0; i < size; i++) {
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);//设置绘制颜色为白色

    int barX = i * barWidth + 1;//每一个x坐标都为循环次数X宽度
    int barY = y - (int)(histogram[i] * k);//这里xy都为每一个柱子左上角点的坐标

    // 绘制直方条
    SDL_Rect bar = { barX, barY, barWidth, (int)(histogram[i] * k) };
    //传递参数为，直方图左上角点的坐标，直方图的宽度和高度
    SDL_RenderFillRect(renderer, &bar);
}
```

看一下效果

![image-20240310133228536](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101332570.png)

对比一下Photoshop的直方图

![image-20240310133811221](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101338289.png)

再调整一下，把窗口改为512X256，高度改为两百

![image-20240310134311229](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101343267.png)

但是只能展示一瞬间，然后就会结束程序，查过资料之后发现可以添加以下代码来延长展示时间

```c
    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }
```

再次测试可以正常展示，完成目标。

## 源代码：

```c
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL.h>

void displayHistogram(const int* histogram, int size) {

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Histogram", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 256, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);

    int maxBarHeight = 0;
    int minBarHeight = 0;

    for (int i = 0; i < size; i++) 
    {
        if (histogram[i] > maxBarHeight) 
        {
            maxBarHeight = histogram[i];
        }
    }

    for (int i = 0; i < size; i++) 
    {
        if (histogram[i] < minBarHeight) 
        {
            minBarHeight = histogram[i];
        }
    }


    int y = 250;


    int barWidth = 2;
    double k = (double)(200) / maxBarHeight;


    for (int i = 0; i < size; i++) {

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        int barX = i * barWidth + 1;
        int barY = y - (int)(histogram[i] * k);

        SDL_Rect bar = { barX, barY, barWidth, (int)(histogram[i] * k) };
        
        SDL_RenderFillRect(renderer, &bar);
    }

    SDL_RenderPresent(renderer);

    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

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

int main()
{
    int histogram[256];
    //建立一个数组
    const char* filename = "E:/code/IDP-Learning-Journey/images/ME.bmp";

    FILE* file = fopen(filename, "rb");

    uint8_t bmpHeader[54];
    size_t bytesRead = fread(bmpHeader, 1, 54, file);

    int width = *(int*)&bmpHeader[18];   // 宽度信息位于偏移量为 18 的位置
    int height = *(int*)&bmpHeader[22];  // 高度信息位于偏移量为 22 的位置

    uint8_t* imageData = (uint8_t*)malloc(width * height);

    fseek(file, 54, SEEK_SET);  // 跳过 BMP 文件头

    bytesRead = fread(imageData, 1, width * height, file);

    GetHistogram(imageData, width, height,histogram);

    displayHistogram(histogram, 256);

    fclose(file);
    free(imageData);

    return 0;
}
```

---

好的，我们目前还需要展示图像的均值和方差以及各部分信息，首先是均值

### 均值

没有想到特别好的办法，只能想到所有的灰度值相加，除以总的像素数

用C语言写一个函数，传入值为数组元素和宽度以及高度像素信息

```c
double mean(const int* histogram, int width, int height)
{
    size_t sum = 0;
    size_t totalPixels = width * height;

    for (size_t i = 0; i < 256; i++)
    {     
        sum += i * histogram[i];
    }

    double a = (double)sum / totalPixels;

    return a;
}
```

其实这里没必要用size_t，是因为我之前思路有问题，一直内存溢出，数字不够存，改变思路后可以成功计算



![image-20240310173812290](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101738362.png)

### 标准差

也同样没想到什么好的方法，只能想到用循环，对每个像素值与均值之差的平方求和，并除以像素总数减一

写一下代码

```c
double Variance(const int* histogram, int width, int height，double mean)
{
    double variance = 0.0;
    
    for (size_t i = 0; i < width * height; i++) 
    {
        variance += pow(histogram[i] - mean, 2);
    }
    
    variance /= (width * height - 1);
    return variance
}
```

出现问题

```c
for (size_t i = 0; i < width * height; i++)
{
    printf("%d\n", i);//检查发现循环次数无法达到1000000次以上
    variance += pow(histogram[i] - mean, 2);
}
原因是因为width * height数值太大，需要强制类型转换
不是上面的原因，因为灰度值图像中并没有这么多内存格子，我理解错了
```

这才是正确的，我真的是看了半个小时，取的数据不是图片所有的数据，而是直方图数据

![image-20240310181201358](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101845259.png)

```c
double StandardDeviation(const int* histogram, int width, int height, double mean)
{
    double variance = 0.0;
    int totalPixels = width * height;

    for (int i = 0; i < 256; i++)
    {
        variance += pow(i - mean, 2) * histogram[i];
    }
	//计算每一个色度值和均值的差的平方再乘上个数得到和最后开平方就可以得到标准差
    variance /= (totalPixels - 1);

    double standardDeviation = sqrt(variance);

    return standardDeviation;
}
```

![image-20240310182432079](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101824126.png)

差的不是很多，我查了一下原因，应该是因为Photoshop统计的是rgb的标准差和均值，我们计算的是灰度值的标准差和均值

### 中间值

字如其名，我们只要找到最中间的像素即可

```c
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
```

真的是最顺利的一次

![image-20240310182856703](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101845044.png)

### 明度和对比度

这个我们一起来看书上【程序 2-5】

```c
void GetBrightContrast(int * histogram,double * bright,double * contrast)
{
    int g;
    double sum,num;//书上说图像很亮时，int有可能会溢出，所以我这里直接用double
    double fsum;
    
    //step.1 求亮度
    for(sum = num = 0, g = 0; g < 256 ; g++)
    {
        sum += histogram[g] * g;
        num += histogram[g];
    }
    * bright = sum * 1.0/num;
    
    //step.2 求对比度
    for(fsum = 0.0,g = 0;g < 256;g++)
    {
        fsum += histogram[g]*(g - *bright) * (g - *bright);
    }
    * contrast = sqrt(fsum/(num - 1));//即Std Dev
    
    //step.3 结束
    return;
}
```

好的观察后发现，明度就是均值，对比度就是标准差

我前面相当于白头脑风暴了

很容易理解我就不再注释了，看一下运行结果

![image-20240310184356037](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101843090.png)

前面还是有点小问题，咱们先用后面的代码，那这样，我们就已经成功得到了图片的亮度，对比度，以及中间值，那么怎么写进直方图里呢

> 好的，我也没找到好的办法，最好的办法是用SDL的文字库，继续网上加
>
> 但是我在测试代码的时候一直提示没有这个库，后来我就想到了能不能直接加在标题上
>
> 于是我就创建了一串字符串用作标题，上面传入了计算的明度，对比度以及中间值的数据

代码如下

```c
    char windowTitle[100];
    sprintf(windowTitle, "Histogram      Brightness: %.2f  Contrast: %.2f  Median: %.2f", a, b, c);
    SDL_Window* window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 256, SDL_WINDOW_SHOWN);
```

详细的讲解在上面讲过，这里就不过多赘述了

> 我知道这个方法可能的确有点投机取巧了，后面也会去寻找解决办法，我觉得Qt虽然能解决，但是还是更想用C语言去自己敲一遍，这个找到解决办法了再来更新笔记

## 结果

不过我们看一下运行结果

![image-20240310191200504](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101912545.png)

还是很不错的，对比一下Photoshop

![image-20240310191246210](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403101912251.png)

是很接近的，可能偏差的原因就是ps用的是rgb，我只用了单通道的灰度值

总体来看不是很难，但是细节很多，需要对C语言有深刻的了解，接下来我把源码放在下面

```c
#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <SDL.h>


void displayHistogram(const int* histogram, int size,double a,double b,double c) 
{

    SDL_Init(SDL_INIT_VIDEO);

    char windowTitle[100];
    sprintf(windowTitle, "Histogram      Brightness: %.2f  Contrast: %.2f  Median: %.2f", a, b, c);
    SDL_Window* window = SDL_CreateWindow(windowTitle, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 512, 256, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);

    int maxBarHeight = 0;
    int minBarHeight = 0;

    for (int i = 0; i < size; i++) 
    {
        if (histogram[i] > maxBarHeight) 
        {
            maxBarHeight = histogram[i];
        }
    }

    for (int i = 0; i < size; i++) 
    {
        if (histogram[i] < minBarHeight) 
        {
            minBarHeight = histogram[i];
        }
    }


    int y = 250;


    int barWidth = 2;
    double k = (double)(200) / maxBarHeight;


    for (int i = 0; i < size; i++) {

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        int barX = i * barWidth + 1;
        int barY = y - (int)(histogram[i] * k);

        SDL_Rect bar = { barX, barY, barWidth, (int)(histogram[i] * k) };
        
        SDL_RenderFillRect(renderer, &bar);
    }

    SDL_RenderPresent(renderer);

    SDL_Event event;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
}

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

double mean(const int* histogram, int width, int height)
{
    size_t sum = 0;
    size_t totalPixels = width * height;

    for (size_t i = 0; i < 256; i++)
    {
        sum += i * histogram[i]; // 将像素值乘以出现次数再累加
    }

    double a = (double)sum / totalPixels;

    return a;
}

double StandardDeviation(const int* histogram, int width, int height, double mean)
{
    double variance = 0.0;
    int totalPixels = width * height;

    for (int i = 0; i < 256; i++)
    {
        variance += pow(i - mean, 2) * histogram[i];
    }

    variance /= (totalPixels - 1);

    double standardDeviation = sqrt(variance);

    return standardDeviation;
}

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

void GetBrightContrast(int * histogram,double * bright,double * contrast)
{
    int g;
    double sum,num;//书上说图像很亮时，int有可能会溢出，所以我这里直接用double
    double fsum;
    
    //step.1 求亮度
    for(sum = num = 0, g = 0; g < 256 ; g++)
    {
        sum += histogram[g] * g;
        num += histogram[g];
    }
    * bright = sum * 1.0/num;
    
    //step.2 求对比度
    for(fsum = 0.0,g = 0;g < 256;g++)
    {
        fsum += histogram[g]*(g - *bright) * (g - *bright);
    }
    * contrast = sqrt(fsum/(num - 1));//即Std Dev
    
    //step.3 结束
    return;
}

int main()
{
    int histogram[256];
    //建立一个数组
    const char* filename = "E:/code/IDP-Learning-Journey/images/ME.bmp";

    FILE* file = fopen(filename, "rb");

    uint8_t bmpHeader[54];
    size_t bytesRead = fread(bmpHeader, 1, 54, file);

    int width = *(int*)&bmpHeader[18];   // 宽度信息位于偏移量为 18 的位置
    int height = *(int*)&bmpHeader[22];  // 高度信息位于偏移量为 22 的位置

    uint8_t* imageData = (uint8_t*)malloc(width * height);

    fseek(file, 54, SEEK_SET);  // 跳过 BMP 文件头

    bytesRead = fread(imageData, 1, width * height, file);

    GetHistogram(imageData, width, height,histogram);

    double meanValue = 0;
    double StandardDeviationValue = 0;
    double median = 0;
    double bright, contrast;

    GetBrightContrast(histogram, &bright, &contrast);

    median = Median(histogram, width, height);

    //printf("中间值（Median）：%f\n", median);
    //printf("亮度（Brightness）: %lf\n", bright);
    //printf("对比度（Contrast）: %lf\n", contrast);

    displayHistogram(histogram, 256, bright, contrast,median);

    fclose(file);
    free(imageData);

    return 0;
}
```



## 感谢您的阅读！
