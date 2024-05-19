import sensor,image,time

# 设置摄像头
sensor.reset()#初始化感光元件
sensor.set_pixformat(sensor.RGB565)#设置为彩色
sensor.set_framesize(sensor.QVGA)#设置图像的大小

sensor.set_vflip(True)
sensor.set_hmirror(True)


sensor.skip_frames(time = 2000)#跳过n张照片，在更改设置后，跳过一些帧，等待感光元件变稳定。

Range = (0,0,320,240)#设定感兴趣区域

#【第一步】记忆色块阈值
for i in range(200):
    img = sensor.snapshot()#拍摄一张照片，img为一个image对象
    img.draw_rectangle((150,110,20,20),color=(255,0,0))
    Statistics = img.get_statistics(roi = (150,110,20,20))
    Threshold = [Statistics.l_min(),Statistics.l_max(),
                 Statistics.a_min(),Statistics.a_max(),
                 Statistics.b_min(),Statistics.b_max()]
    print(Threshold)

# 一直拍照
while(True):
    img = sensor.snapshot()#拍摄一张照片，img为一个image对象
    #【第二步】找色块
    for blob in img.find_blobs([Threshold], roi=Range, area_threshold=100, pixels_threshold=100, merge=True, margin=10):
         #【第三步】输出
         img.draw_rectangle(blob.rect())#画框
         img.draw_cross(blob.cx(),blob.cy())#画十字
         print(blob.cx(),blob.cy())#打印中心坐标
