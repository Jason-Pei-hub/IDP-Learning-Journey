# YOLOv5-lite模型训练超详细教程

## 模型训练

### 采集训练数据（图片）

在树莓派上运行下面的捕获代码，每0.4s捕获一张，捕获的照片会存在images文件夹下，可以通过WinSCP把采集的图片从树莓派传回电脑，可以查找相关博客查看使用教程，同时，用网上找的训练集也是可以的

```
import cv2
from threading import Thread
import uuid
import os
import time
count = 0
def image_collect(cap):
    global count
    while True:
        success, img = cap.read()
        if success:
            file_name = str(uuid.uuid4())+'.jpg'
            cv2.imwrite(os.path.join('images',file_name),img)
            count = count+1
            print("save %d %s"%(count,file_name))
        time.sleep(0.4)
 
if __name__ == "__main__":
    
    os.makedirs("images",exist_ok=True)
    
    # 打开摄像头
    cap = cv2.VideoCapture(0)
 
    m_thread = Thread(target=image_collect, args=([cap]),daemon=True)
    
    while True:
 
        # 读取一帧图像
 
        success, img = cap.read()
 
        if not success:
 
            continue
 
        cv2.imshow("video",img)
 
        key =  cv2.waitKey(1) & 0xFF   
 
        # 按键 "q" 退出
        if key ==  ord('c'):
            m_thread.start()
            continue
        elif key ==  ord('q'):
            break
 
    cap.release() 
```

### 训练集制作

我们需要用到一个工具labelme，这个工具的安装直接使用命令安装，这里是在自己的电脑上

```
pip install labelme
```

使用时按下win+R，输入cmd调出命令行，输入命令labelme即可调出来，可能是由于我使用了清华的镜像源，所以是中文界面

首先需要打开你存放照片的文件夹

![image-20250414191228142](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141912327.png)

选择你存放图片的路径

> 如果你觉得下面json转txt麻烦，请直接输出txt格式，然后略过下面，我实测以后出现bug的情况几乎没有

然后在存放图片的文件夹同级目录下创建一个labels文件夹，里面用来存放生成的JSON文件

![image-20250414191427616](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141914655.png)

然后在labelme里面更改输出路径，更改为上面创建的labels文件夹，以及打开这个上面的自动保存

![image-20250414191447899](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141914036.png)

然后就是愉快的（费时间的）打标签工作了

### 转换JSON文件为yolov5格式的txt文件

SON格式不是我们训练yolov5需要的格式，下面我们就需要对JSON文件进行转换，变成我们需要的格式，当然，labelme也可以直接输出yolo格式，但是听说不一次性打完标签会有bug，所以还是建议使用JSON转txt，转换格式我们需要用到下面的代码

dic_lab.py，上面的''中的0-9是你打的标签，后面的0-9是这个标签在yolov5格式中的编号，ratio的0.9是把训练集和验证机集以9：1的比例分配

dic_lab.py

```
dic_labels= {'0':0,
            '1':1,
            '2':2,
			'3':3,
			'4':4,
			'5':5,
			'6':6,
			'7':7,
			'8':8,
			'9':9,
            'path_json':'D:/laboratory/Raspberry_PI/yolov5-lite train/cap image/labels', #这里是labelme输出路径
            'ratio':0.9}
```

labelmetoyolo.py

```
import os
import json
import random
import base64
import shutil
import argparse
from pathlib import Path
from glob import glob
from dic_lab import dic_labels
 
def generate_labels(dic_labs):
    path_input_json = dic_labels['path_json']
    ratio = dic_labs['ratio']
    for index, labelme_annotation_path in enumerate(glob(f'{path_input_json}/*.json')):
 
        # 读取文件名
        image_id = os.path.basename(labelme_annotation_path).rstrip('.json')
        
        # 计算是train 还是 valid
        train_or_valid = 'train' if random.random() < ratio else 'valid'
 
        # 读取labelme格式的json文件
        labelme_annotation_file = open(labelme_annotation_path, 'r')
        labelme_annotation = json.load(labelme_annotation_file)
 
        # yolo 格式的 lables
        yolo_annotation_path = os.path.join(train_or_valid, 'labels',image_id + '.txt')
        yolo_annotation_file = open(yolo_annotation_path, 'w')
        
        # yolo 格式的图像保存
        yolo_image = base64.decodebytes(labelme_annotation['imageData'].encode())
        yolo_image_path = os.path.join(train_or_valid, 'images', image_id + '.jpg')
        
        yolo_image_file = open(yolo_image_path, 'wb')
        yolo_image_file.write(yolo_image)
        yolo_image_file.close()
     
 
        # 获取位置信息
        for shape in labelme_annotation['shapes']:
            if shape['shape_type'] != 'rectangle':
                print(
                    f'Invalid type `{shape["shape_type"]}` in annotation `annotation_path`')
                continue
           
 
            points = shape['points']
            scale_width = 1.0 / labelme_annotation['imageWidth']
            scale_height = 1.0 / labelme_annotation['imageHeight']
            width = (points[1][0] - points[0][0]) * scale_width
            height = (points[1][1] - points[0][1]) * scale_height
            x = ((points[1][0] + points[0][0]) / 2) * scale_width
            y = ((points[1][1] + points[0][1]) / 2) * scale_height
            object_class = dic_labels[shape['label']]
            yolo_annotation_file.write(f'{object_class} {x} {y} {width} {height}\n')
        yolo_annotation_file.close()
        print("creat lab %d : %s"%(index,image_id))
 
 
if __name__ == "__main__":
    os.makedirs(os.path.join("train",'images'),exist_ok=True)
    os.makedirs(os.path.join("train",'labels'),exist_ok=True)
    os.makedirs(os.path.join("valid",'images'),exist_ok=True)
    os.makedirs(os.path.join("valid",'labels'),exist_ok=True)
    generate_labels(dic_labels)
```

运行labelmetoyolo.py即可完成格式转换

### 准备工作完成

## 模型开始训练

我们准备好数据集

最好在这个地方创建一个文件夹

![image-20250414192125588](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141921653.png)

里面是两个文件夹，分别存放图片和标签

![image-20250414192152755](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141921794.png)

在每个文件夹下面都创建两个文件夹，分别是刚刚的训练集和验证集

![image-20250414192250322](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141922366.png)

### ok,开始看工程

我们打开工程文件

![image-20250414191826784](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141918911.png)

在data目录下新建文件，命名为mydata.yaml

![image-20250414192441622](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141924673.png)

文件里面写入这些代码

```c
# train and val data as 1) directory: path/images/, 2) file: path/images.txt, or 3) list: [path1/images/, path2/images/]
train: D:\YOLOv5\YOLOv5-Lite-1.4\YOLOv5-Lite\data\dataset\images\train # 训练集图片的目录
val: D:\YOLOv5\YOLOv5-Lite-1.4\YOLOv5-Lite\data\dataset\images\val  # 验证集图片的目录



# number of classes
nc: 4  #你的标签数量

# class names
names: ["car","bus","van","truck"]    #你的所有标签名称


```

在 **Yolov5-Lite** 的目录下找到 **train.py** (训练文件)的 **main** 函数入口

![image-20250414192648173](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141926256.png)

找到第460行代码，进行如下配置：

![image-20250414195116685](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141951788.png)

其中

```
parser.add_argument('--epochs', type=int, default=400)     default为训练轮次，根据需要来调整
```

#### 将训练环境与数据集都搞定之后，就可以点击运行按钮进行 **Yolov5-Lite** 的模型训练了！

![image-20250414195512900](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141955061.png)

## 树莓派4B部署YOLOv5-Lite

### ONNX概述

Open Neural Network Exchange(ONNX)是一个开放的生态系统，它使人工智能开发人员在推进项目时选择合适的工具，不用被框架或者生态系统所束缚。ONNX支持不同框架之间的互操作性，简化从研究到生产之间的道路。ONNX支持许多框架（TensorFlow, Pytorch, Keras, MxNet, MATLAB等等），这些框架中的模型都可以导出或者转换为标准ONNX格式。模型采用ONNX格式后，就可在各种平台和设备上运行。

### ONNX模型转换和移植

如今的开源 **YOLO** 系列神经网络模型的目录下作者都会预留 **export.py** 文件将该神经网络模型进行转换到 **ONNX** 模型，方便大家实际情况下部署使用！

![1744631902544](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141958950.png)

### 点击运行即可生成.onnx文件！

![1744631960303](https://gitee.com/jason_pei/typora-bed/raw/master/image/202504141959677.png)