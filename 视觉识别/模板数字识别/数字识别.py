import sensor, image, time

# 设置摄像头
sensor.reset()  # 初始化感光元件
sensor.set_pixformat(sensor.RGB565)  # 设置为灰度图像
sensor.set_framesize(sensor.QVGA)  # 设置图像的大小

sensor.set_vflip(True)
sensor.set_hmirror(True)

sensor.set_contrast(3)  # 设置对比度 -3~3

num_quantity = 8  # 数字数量
num_model = []  # 储存数字模型图片的列表
# 这里的数字图片模型高度为40pix

for n in range(1, num_quantity + 1):  # 循环将根目录下F文件夹的数字图片载入
    num_model.append(image.Image('/F/' + str(n) + '.pgm'))

clock = time.clock()  # 声明时钟，计算帧率

img_colorful = sensor.alloc_extra_fb(160, 120, sensor.RGB565)
img_to_matching = sensor.alloc_extra_fb(35, 45, sensor.GRAYSCALE)
threshold = (0, 70)

scale = 1

while True:
    clock.tick()  # 记帧
    img = sensor.snapshot()  # 捕获摄像头画面
    img_colorful.draw_image(img, 0, 0)
    blobs = img.find_blobs([threshold])
    if blobs:
        for blob in blobs:
            if blob.pixels() > 50 and 100 > blob.h() > 10 and blob.w() > 3:
                scale = 40 / blob.h()
                img_to_matching.draw_image(img, 0, 0, roi=(blob.x() - 2, blob.y() - 2, blob.w() + 4, blob.h() + 4),
                                           x_scale=scale, y_scale=scale)
                for n in range(0, num_quantity):
                    r = img_to_matching.find_template(num_model[n], 0.7, step=2, search=image.SEARCH_EX)
                    if r:
                        print(str(n + 1))
                        img.draw_rectangle((blob.x(), blob.y(), blob.w(), blob.h()), color=(255, 0, 0))
                        img.draw_string(blob.x(), blob.y(), str(n + 1), scale=2, color=(255, 0, 0))
    img.draw_string(0, 0, str(round(clock.fps(), 2)))

