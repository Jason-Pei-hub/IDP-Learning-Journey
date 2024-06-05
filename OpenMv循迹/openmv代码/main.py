# Untitled - By: lenovo - 周四 11月 4 2021
import sensor, image,time,lcd,utime
from pyb import UART
from pyb import LED
black=[0, 35, -58, 36, -49, 46]
blackTHRESHOLD=(0, 35, -58, 36, -49, 46)
uart=UART(3,9600)

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QVGA)
sensor.skip_frames(time = 2000)
sensor.set_auto_whitebal(False)
sensor.set_auto_gain(False)

#LED(1).on()
#LED(2).on()
#LED(3).on()
areas = [(15,100,40,40),(55,100,40,40), (95,100,40,40),(135,100,40,40),(175,100,40,40),(215,100,40,40),(255,100,40,40)]

def getTrace(img):
    Trace = [0,0,0,0,0,0,0]
    for area in areas:
        img.draw_rectangle(area)   #圈出识别区块
        statistics = img.get_statistics(roi=area) #统计像素颜色

        if black[0]<statistics.l_mode()<black[1] and black[2]<statistics.a_mode()<black[3] and black[4]<statistics.b_mode()<black[5]: #如果该区域是红色
            img.draw_string(area[0]+5, area[1]+3, "1")    #标记为1
            Trace[areas.index(area)] = 1
        else: img.draw_string(area[0]+5, area[1]+3, "0")  #标记为0
    return Trace

num=[0,0,0,0,0,0,0]

clock = time.clock()

while(True):
    clock.tick()
    img = sensor.snapshot()
    num=getTrace(img)
    #print(num)

    Trance_Data=num[0]*64+num[1]*32+num[2]*16+num[3]*8+num[4]*4+num[5]*2+num[6]*1
    print(Trance_Data)
    data = bytearray([0xaf,0xbf,Trance_Data,0xcf])    #帧头 + 帧头 + x + y + 帧尾
    uart.write(data)




