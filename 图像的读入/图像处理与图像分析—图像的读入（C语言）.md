# 图像处理与图像分析—图像的读入（C语言）

> 学习将会依据教材图像处理与图像分析基础（C/C++）版内容展开

---



### 什么是数字图像处理

一副图像可以定义为一个二维函数 f(x，y) ，其中 x 和 y 是空间（平面）坐标，任意一对空间坐标 (x,y) 处的幅度值 （x，y） 称为图像在该坐标点的强度或灰度。当 x,y 和灰度值 f 都是有限的离散量时，我们称该图像为数字图像。**数字图像处理**是指借助于数字计算机来处理数字图像。注意，数字图像由有限数量的元素组成，每个元素都有一定的位置和数值，这些元素称为**像素**。



### 好，那么图像数字处理的第一步是什么？

一定是对于单个图片的读取，今天的主要目标也是读取一整个图片的完整信息

所用的语言为C语言，更加底层，能够帮助我们更好的理解

C语言编译器主要有Clang、**GCC**、WIN-TC、SUBLIME、**MSVC**、Turbo C等。

> 但是这里我说一下，我之前在编写C语言代码是所使用的都是GCC编译器，但是这一遍学习准备尝试一下MSVC这样一个微软的编译器，同时开始尝试使用VS2022来进行开发，之前使用Clion进行开发

---

#### **我们先来看书上的【程序2-1】点运算的经典程序结构**

```cpp
void F1(BYTE* pimg, int width, int height)
{
	BYTE* pCur, * pEnd;

	pEnd = pImg + width * height;
	for (pCur = pImg; pCur < pEnd)
	{
		*(pCur++) = f(*pCur);
	}
	return;
}
```

我先来分析一下这个代码：

```cpp
//首先这串代码是一个名为F1的函数，输入参数是一个BYTE类型的指针变量，整型的宽度和高度
void F1(BYTE* pimg, int width, int height)
{
	BYTE* pCur, * pEnd;
    //定义了两个指针变量* pCur, * pEnd
	pEnd = pImg + width * height;
    //表示将指针 pEnd 设置为指向图像数据末尾的位置。这里的计算方式是将指针 pImg 指向的内存地址加上图像的宽度乘以高度，从而得到图像数据的最后一个像素的下一个位置。
    
    
	for (pCur = pImg; pCur < pEnd)
    //pCur = pImg;：首先将指针变量 pCur 初始化为指向图像数据的起始位置，即指针 pImg 所指向的位置。
    //pCur < pEnd：这是循环的终止条件。只要 pCur 指针小于 pEnd 指针（即还未到达图像数据的末尾），就会继续执行循环。
	{
		*(pCur++) = f(*pCur);
        //首先会调用函数 f，并将当前指针 pCur 所指向的像素值作为参数传递给函数 f。函数 f 对该像素值进行处理，并返回处理后的结果。然后，将这个处理后的结果写回到当前指针 pCur 所指向的位置，并通过 pCur++ 操作使指针指向下一个像素位置。
	}
	return;
}
```

因为咱们用的是C语言哈，我写这些代码的时候已经发现报错了

原因是啥？ —— C语言里面没有BYTE类型，所以只能用别的类型替代，BYTE我去查了一下，人如其名，一个字节的存储空间，那我这里就用uint8_t来代替这个指针类型

**那C语言版本就呼之欲出了**

```c
void F1(uint8_t* pImg, int width, int height)
{
    uint8_t* pCur;
    uint8_t* pEnd;

    pEnd = pImg + width * height;
    for (pCur = pImg; pCur < pEnd; pCur++)
    {
        *pCur = f(*pCur);
    }
    return;
}
```

其实uint8_t变量还是报错了，是因为没有引用头文件，引用\#include <stdint.h>就完美解决

**那接下来的问题，f函数是什么？**—我们先来看下一个例子，再来探讨这个问题

---

【程序2-2】邻域运算的典型程序结构

```cpp

void F2(BYTE* pOrgImg, int width, int height, BYTE* pResImg)
{
	BYTE* pCur, * pRes;
	int x, y;
	for (y = 0, pCur = pOrgImg, pRes = pResImg; y < height; y++)
	{
		for (x = 0; x < width; x++, pCur++, pRes++)
		{
			*pRes = f(pOrgImg, x, y);
		}
	}
	return;
}
```

我们还是先看一下这个cpp代码，先根据我的理解注释一下

```cpp
//四个传入值，指针为BYTE型
//pOrgImg：这是指向原始图像数据的指针。原始图像数据包含了待处理的像素值。
//width：这是图像的宽度，表示图像的水平像素数。
//height：这是图像的高度，表示图像的垂直像素数。
//pResImg：这是指向结果图像数据的指针。结果图像数据将存储经过处理后的像素值。
void F2(BYTE* pOrgImg, int width, int height, BYTE* pResImg)
{
	BYTE* pCur, * pRes;
    //创建两个指针，通过指针 pCur 和 pRes 分别指向原始图像和结果图像中当前位置的像素。
	int x, y;
    
  
	for (y = 0, pCur = pOrgImg, pRes = pResImg; y < height; y++)
    //y = 0，从顶部开始处理，原始图像指针指向原始图像开始的位置，结果图像指针只想结果图像开始的部分
    //y一直递增，直到便利完所有的行数
	{
		for (x = 0; x < width; x++, pCur++, pRes++)
		{
            //x循环来保证遍历完每一行的所有像素块
            //并且，每次原始图像指针和结果图像指针都往后递增来访问正确的操作像素
			*pRes = f(pOrgImg, x, y);
            //调用函数 f，并将原始图像数据指针 pOrgImg、当前位置的水平坐标 x 和垂直坐标 y 作为参数传递给 f 函数。f 函数会对该像素进行处理，并返回处理后的结果。
		}
	}
	return;
}
```

C语言版本顺手写一下

```c
void F2(uint8_t* pOrgImg, int width, int height, uint8_t* pResImg)
{
    uint8_t* pCur;
    uint8_t* pRes;
    int x, y;

    for (y = 0, pCur = pOrgImg, pRes = pResImg; y < height; y++)
    {
        for (x = 0; x < width; x++, pCur++, pRes++)
        {
            *pRes = f(pOrgImg, x, y);
        }
    }
    return;
}
```

没有报错！

---

### 分析F1和F2

##### 函数 `F1`：

- **参数**
  - 接收三个参数：图像数据指针 `pImg`、图像宽度 `width` 和图像高度 `height`。
- **功能**
  - 使用一个循环遍历图像中的每个像素。
  - 对每个像素调用函数 `f` 进行处理，并将处理结果直接更新到原始图像数据中。

##### 函数 `F2`：

- **参数**
  - 接收四个参数：原始图像数据指针 `pOrgImg`、图像宽度 `width`、图像高度 `height` 和结果图像数据指针 `pResImg`。
- **功能**
  - 使用两个嵌套循环遍历原始图像的每个像素。
  - 对每个像素调用函数 `f` 进行处理，并将处理结果存储到结果图像数据中。

##### 区别：

1. **循环方式**：函数 `F1` 使用单层循环来遍历像素，而函数 `F2` 使用嵌套循环进行遍历。
2. **参数传递**：函数 `F1` 直接操作原始图像数据，而函数 `F2` 在处理像素时需要额外传递像素的坐标信息 `x` 和 `y`。
3. **结果存储**：函数 `F1` 直接在原始图像数据中更新处理结果，而函数 `F2` 将处理结果存储到另一个结果图像数据中，保持了原始数据的不变性。



#### 函数f

函数 `f` 被用作对图像数据进行处理的函数

接受一个像素值，处理后在返回一个像素值存入

---

### 过程实现

需要读取的图片

![ME](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403091947315.bmp)

本次要求是可以读取数据，那么我们实现的话F1和F2都可以针对像素进行操作

我们这次就通过F1来实现图像的读取

想了一下F1和F2其实都是针对于像素进行编辑操作，那我们的目标就是通过F1实现对于图片数据的读取，并打印在终端

```c
void F1(uint8_t* pImg, int width, int height)
{
    uint8_t* pCur;
    uint8_t* pEnd;

    pEnd = pImg + width * height;
    for (pCur = pImg; pCur < pEnd; pCur++)
    {
        //*pCur = f(*pCur);
        printf("%d",*pCur);
    }
    return;
}
```

C语言怎么去读图片呢，问了一下chatgpt

```c
const char* filename = "path/to/your/image.jpg"; // 图片文件路径

// 打开文件
FILE* file = fopen(filename, "rb");
if (file == NULL) {
    printf("无法打开文件：%s\n", filename);
    return 1;
}

// 获取图像宽度和高度（根据图像格式进行解析）
int width = 0;  // 替换为实际的宽度值
int height = 0; // 替换为实际的高度值

// 分配内存来存储图像数据
uint8_t* imageData = (uint8_t*)malloc(width * height);
if (imageData == NULL) {
    printf("内存分配失败\n");
    fclose(file);
    return 1;
}

// 读取图像数据
size_t bytesRead = fread(imageData, 1, width * height, file);
if (bytesRead != width * height) {
    printf("读取图像数据失败\n");
    free(imageData);
    fclose(file);
    return 1;
}
```



可以参考一下，其中就发现了几个问题

高度和宽度又该如何去自动计算呢？

> 查了一下jpg格式的图片，其文件结构更为复杂，直接解析文件头部来获取图像的宽度和高度相对困难。JPEG 文件通常包含了大量的压缩数据和标记信息，因此需要专门的 JPEG 解码器来读取并解析这些数据。深度搜索后发现，可以通过开源的 JPEG 解码库来获得

有点难，咱们还是先写死程序

![image-20240309175426145](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403091947814.png)

原码：

```c
#include <stdio.h>
#include <stdint.h>

void F1(uint8_t* pImg, int width, int height)
{
    uint8_t* pCur;
    uint8_t* pEnd;

    pEnd = pImg + width * height;
    for (pCur = pImg; pCur < pEnd; pCur++)
    {
        //*pCur = f(*pCur);
        printf("%d", *pCur);
    }
    return;
}

int main()
{
    const char* filename = "C:/Users/25706/Pictures/Camera Roll/ME.jpg";

    FILE* file = fopen(filename, "rb");
    if (file == NULL) {
        printf("无法打开文件：%s\n", filename);
        return 1;
    }

    int width = 1107;  //通过Windows自带的画图工具看到图片尺寸为1107
    int height = 1107; 

    uint8_t* imageData = (uint8_t*)malloc(width * height);//按需分配内存
    if (imageData == NULL) {
        printf("内存分配失败\n");
        fclose(file);
        return 1;
    }

    size_t bytesRead = fread(imageData, 1, width * height, file);
    if (bytesRead != width * height) {
        printf("读取图像数据失败\n");
        free(imageData);
        fclose(file);
        return 1;
    }

    F1(imageData, width, height);//调用F1来打印图片信息

    fclose(file);    // 关闭文件和释放内存
    free(imageData);

    return 0;
}
```

运行结果：

```c
读取图像数据失败
```

后面又尝试了png、bmp等图片格式，全都失败了。

失败原因，图片不止这么大，**bmp图片还包含别的信息**

查了bmp文件格式

> BMP文件由4部分组成：
>
> 1. 位图文件头(bitmap-file header)
> 2. 位图信息头(bitmap-informationheader)
> 3. 颜色表(color table)
> 4. 颜色点阵数据(bits data)
>
> 24位真彩色位图没有颜色表，所以只有1、2、4这三部分。

所以在 `main` 函数中，读取图像数据时需要考虑 BMP 图像文件头的大小（通常为 54 字节）。你需要跳过文件头，才能正确读取图像的 RGB 数据。可以通过 `fseek` 函数将文件指针移动到图像数据的起始位置。



#### 重新修改

**重新整理思路**

**第一步：打开文件**

```c
 const char* filename = "E:/code/IDP-Learning-Journey/images/ME.bmp";
//定义常变量指针变量指向文件地址

 FILE* file = fopen(filename, "rb");
//把未见打开为只读模式
```

**第二部：处理图像数据**

```c
uint8_t bmpHeader[54];  
// 定义一个数组来存储 BMP 文件头信息，总共 54 字节
fread(bmpHeader, 1, 54, file);  
// 从文件中读取 54 字节的数据到 bmpHeader 数组中

int width = *(int*)&bmpHeader[18];   // 从偏移量为 18 的位置读取图像宽度信息，使用指针强制类型转换将字节数据转换为整型数据
int height = *(int*)&bmpHeader[22];  // 从偏移量为 22 的位置读取图像高度信息，同样使用指针强制类型转换

uint8_t* imageData = (uint8_t*)malloc(width * height);  // 根据图像宽度和高度动态分配内存，用于存储图像数据

fseek(file, 54, SEEK_SET);  
// 将文件指针移动到 54 字节的位置，跳过 BMP 文件头部分

fread(imageData, 1, width * height, file);  
// 从文件中读取图像数据，每个像素点占用 1 个字节
```

`*(int*)&bmpHeader[18]` 这个表达式的含义是：

- `&bmpHeader[18]` 取得 `bmpHeader` 数组中第 18 个元素的地址，也就是指向宽度信息的起始位置。
- `(int*)` 表示将这个地址强制转换为指向整型数据的指针。
- `*` 表示解引用这个指针，获取该地址上的值，即宽度信息。

**第三步：调用函数，打印信息**

```c
F1(imageData, width, height);
```

**函数在上面讲过了，可以回去看看**

```c
void F1(uint8_t* pImg, int width, int height)
{
    uint8_t* pCur;
    uint8_t* pEnd;

    pEnd = pImg + width * height;
    for (pCur = pImg; pCur < pEnd; pCur++)
    {
        //*pCur = f(*pCur);
        printf("%d ", *pCur);  // 输出灰度值
    }
    return;
}
```

第四步：好程序员的良好习惯

```c
fclose(file);
free(imageData);

return 0;
```

**成功！！！！！！！！！！！！！！**

![image-20240309183813597](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403091947753.png)

> 真的太难了，本来以为不是很难的

原码必须记录：

```c
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


void F1(uint8_t* pImg, int width, int height)
{
    uint8_t* pCur;
    uint8_t* pEnd;

    pEnd = pImg + width * height;
    for (pCur = pImg; pCur < pEnd; pCur++)
    {
        //*pCur = f(*pCur);
        printf("%d ", *pCur);  // 输出修改后的像素值
    }
    return;
}

int main()
{
    const char* filename = "E:/code/IDP-Learning-Journey/images/ME.bmp";

    FILE* file = fopen(filename, "rb");

    uint8_t bmpHeader[54];
    size_t bytesRead = fread(bmpHeader, 1, 54, file);

    int width = *(int*)&bmpHeader[18];   // 宽度信息位于偏移量为 18 的位置
    int height = *(int*)&bmpHeader[22];  // 高度信息位于偏移量为 22 的位置

    uint8_t* imageData = (uint8_t*)malloc(width * height); 

    fseek(file, 54, SEEK_SET);  // 跳过 BMP 文件头

    bytesRead = fread(imageData, 1, width * height, file); 

    F1(imageData, width, height);

    fclose(file);
    free(imageData);

    return 0;
}
```

---

灰度值和rgb有什么区别呢，查了一下，区别就是在读取像素点的时候所分配的字节个数

那么如果我们给图片的每个像素点分配三个字节

用F1中`pCur`的遍历图像数据中的每个像素点。`pCur` 指向当前正在处理的像素点的起始位置，通过 `pCur` 指针就可以逐个访问每个像素点的颜色数据。

试验了一下也是可以成功的

```c
#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

void F1(uint8_t* pImgRGB, int width, int height)
{
    uint8_t* pCur;
    uint8_t* pEnd;

    pEnd = pImgRGB + width * height * 3;  // 每个像素点占用 3 个字节（BGR）
    for (pCur = pImgRGB; pCur < pEnd; pCur += 3) {
        uint8_t blue = pCur[0];
        uint8_t green = pCur[1];
        uint8_t red = pCur[2];
        printf("R: %d, G: %d, B: %d ", red, green, blue);
    }
}

int main()
{
    const char* filename = "E:/code/IDP-Learning-Journey/images/ME.bmp";

    FILE* file = fopen(filename, "rb");

    uint8_t bmpHeader[54];
    fread(bmpHeader, 1, 54, file);

    int width = *(int*)&bmpHeader[18];   // 宽度信息位于偏移量为 18 的位置
    int height = *(int*)&bmpHeader[22];  // 高度信息位于偏移量为 22 的位置

    uint8_t* imageData = (uint8_t*)malloc(width * height * 3);  // 每个像素点占用 3 个字节（BGR）

    fseek(file, 54, SEEK_SET);  // 跳过 BMP 文件头

    fread(imageData, 3, width * height, file);  // 每个像素点占用 3 个字节（BGR）

    F1(imageData, width, height);

    fclose(file);
    free(imageData);

    return 0;
}
```

![image-20240309191106732](https://gitee.com/jason_pei/typora-bed/raw/master/image/202403091947686.png)

---

**感谢您的观看！！！**