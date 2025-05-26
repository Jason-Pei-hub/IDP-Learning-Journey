# YOLOv5-lite模型训练环境配置超级无敌详细教程

> 如果你觉得麻烦，请联系我，可以有偿帮助训练模型，我直接把训练好的模型发给你，你部署使用即可
>
> conda的环境安装会占用很大空间
>
> 我会提供YOLO相关网络结构图或者是训练时候的图像，论文必备
>
> 请给个好评！！！

## 查看电脑状况

深度学习的训练对于电脑显卡要求较高，若电脑没有独立显卡（NVIDIA卡）或者是AMD的显卡无法使用GPU进行训练，只可以用CPU进行训练，会慢很多。若不清楚电脑有无显卡可以打开任务管理器，点击性能往下找GPU，就可以看到是否有独立显卡，不清楚是否为独立显卡则搜索型号。

![image-20250414125152767](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141251940.png)

若有独立显卡则安装GPU版本的pytorch，若无独立显卡则安装CPU版本的pytorch。

## 安装所需软件

推荐安装Anaconda3+Pycharm，会安装或者安装过了则跳过这一步

### Anaconda3安装

Anaconda3由于是国外网站下载较慢，推荐通过清华镜像源安装。

https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/?C=M&O=Dhttps://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/?C=M&O=D
https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/?C=M&O=D

选择带有Anaconda3...Windows...exe 字样的进行下载，建议从最新版往后边找几个，最新版一般不够稳定。下载完之后打开Anaconda3进行安装，一直点下一步，选Just Me，安装路径可以直接复制粘贴修改到  D:\Anaconda3  ，也可以修改到其他路径。

![image-20250414125411374](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141254442.png)



![image-20250414125428716](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141254762.png)

安装时全选。

### Pycharm安装

安装Pycharm可以直接去官网下载，速度较快，也可以点击网盘下载相同版本。

https://www.jetbrains.com/zh-cn/pycharm/download/?section=windowshttps://www.jetbrains.com/zh-cn/pycharm/download/?section=windows
https://www.jetbrains.com/zh-cn/pycharm/download/?section=windows

往下拉下载第二个免费版就可以。下载完之后安装，一直点下一步，遇到选择路径 修改路径到D盘或者其它除C盘，再遇到勾选的选项全部勾选就可以。



## 创建虚拟环境，安装pytorch

### 创建虚拟环境

按下Win键，输入anaconda prompt,打开推荐的应用。

![image-20250414125600339](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141256480.png)



默认进入的是base环境，此时需要新建环境

此时新建虚拟环境（需要关闭加速软件），这里创建一个名为yolov5，python版本为3.9的虚拟环境，也可以修改为其他名。

conda create -n yolov5 python=3.9


此时再次创建yolov5虚拟环境

conda create -n yolov5 python=3.9
显示如下则正常创建，输入y 并回车。

![image-20250414130300002](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141303108.png)

创建完之后输入

```
conda activate yolov5
```



## 配置yolov5环境

### 下载yolov5-lite源码

网盘下载后解压，然后pycharm打开

点击鼠标右键yolov5文件夹通过pycharm打开，打开后需要配置虚拟环境，点击file-settings，点击project：ultralytics，点击python interpreter，点击右边add....，若为中文则翻译后点击相同含义的位置。

![image-20250414130820058](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141308104.png)

点击Virtualenv Environment - Existing，点击右边三个点，找到刚才设置的D:\Anaconda3\envs\yolov5\python.exe 路径，之后一直点确定即可，耐心等待环境加载完毕。

![image-20250414130841890](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141308939.png)

或者新版本的界面更为简单，按照下图方式选择就可以

![image-20250414130854572](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141308625.png)

选择python，接着选择刚刚添加的yolov11环境，选择python.exe，

在终端输入

```
pip install -r requirements.txt
```

安装所需库



### 即可配置完成