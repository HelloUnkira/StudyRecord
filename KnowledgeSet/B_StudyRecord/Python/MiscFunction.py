#杂七杂八


# 筛选range (2, edge)中的素数,并生成素数列表
def prime_sizer(edge):
    if edge < 2:
        return []
    filter_list = [element for element in range(2, edge + 1)]
    index1 = 0
    while index1 < len(filter_list):
        if filter_list[index1] != 0:
            index2 = index1 + 1
            while index2 < len(filter_list):
                if filter_list[index2] != 0:
                    if filter_list[index2] % filter_list[index1] == 0:
                        filter_list[index2] = 0
                index2 += 1
        index1 += 1
    prime_list = [prime for prime in filter_list if prime != 0]
    # prime_list = filter(lambda prime : prime != 0, filter_list)
    return prime_list


# 汉诺塔,二元组列表
def hanoi(tower_level, tower_start='A', tower_temp='B', tower_end='C'):
    result_list = []

    def hanoi_run(record, level, start, temp, end):
        if level <= 0: return
        hanoi_run(record, level - 1, start, end, temp)
        record.append(tuple([start, end]))
        hanoi_run(record, level - 1, temp, start, end)

    hanoi_run(result_list, tower_level, tower_start, tower_temp, tower_end)
    return result_list


# 泽勒一致性算法:(星期一到星期日:[0, 6])
def datetime_to_week(year, month, day):
    week = 0
    week += day
    week += (month + 1) * 26 / 10
    week += (year % 100) * (5 / 4)
    week += (year / 100) * (21 / 5)
    # 范围时[0,6]
    week = int(week) % 7
    return week


# 摘录:一朵简单的小心心
def draw_simple_heart():
    import numpy as np
    import matplotlib.pyplot as plt
    x = np.linspace(-2, 2, 1500)                    # 生成x坐标 -2到2范围 的 等差数列数组，数组元素一共1500个
    y1 = np.sqrt(1 - (np.abs(x) - 1) ** 2)          # 上半部分爱心函数线段
    y2 = np.sqrt(1 - (np.abs(x) / 2) ** 0.5) * -3   # 下半部分爱心函数线段
    plt.fill_between(x, y1, color='red')            # 填充x与y1围成的图形的内部
    plt.fill_between(x, y2, color='red')            # 填充x与y2围成的图形的内部
    plt.xlim([-2.5, 2.5])                           # 控制x轴的范围
    plt.text(0, -0.4, 'I love you!',                # 生成文本, 指定文本位置, 字体大小
            fontsize=30,
            fontweight='bold',
            color='yellow',
            horizontalalignment='center')
    plt.axis('off')     # 去除刻度
    plt.show()


# 摘录,渐变1
def draw_gradient_picture1():
    import numpy
    from PIL import Image
    r = numpy.tile(np.linspace(192, 255, 300, dtype=np.uint8), (600, 1)).T
    g = numpy.tile(np.linspace(192, 255, 600, dtype=np.uint8), (300, 1))
    b = numpy.tile(np.linspace(192, 255, 300, dtype=np.uint8), (600, 1)).T
    img = numpy.dstack((r, g, b))
    x = numpy.arange(600)
    y = numpy.sin(np.linspace(0, 2*np.pi, 600))
    y = numpy.int32((y+1)*0.9*300/2 + 0.05*300)
    Image.fromarray(img, mode='RGB').show()


# 摘录,渐变2
def draw_gradient_picture2():
    import numpy as np
    from PIL import Image
    import matplotlib.cm as cm
    width = 1200
    height = 900
    cm1 = cm.get_cmap('jet')    # 选择调色板1
    colormap1 = np.array([cm1(k) for k in range(cm1.N)])
    i = np.repeat(np.arange(height), width).reshape(height, width) - height // 2  # 同行值相同, 行与行单位递增
    j = np.tile(np.arange(width), (height, 1)) - width // 2                       # 同列值相同, 列与列单位递增
    d = np.hypot(i, j)      # 行列混合(平方和)
    d = np.int32((cm1.N - 1) * (d - d.min()) / (d.max() - d.min()))
    d = np.uint8(255 * colormap1[d])
    Image.fromarray(d).show()


# 摘录:视觉冲击
def draw_visual_shock():
    import turtle
    turtle.setup(1280, 720)
    turtle.bgcolor('black')
    turtle.speed(10000)
    for i in range(600):
        # 每一次绘制都重置颜色,长度,并设置乌龟的旋转角度
        turtle.color(['red', 'blue', 'green', 'purple', 'gold', 'pink'][i % 6])
        # 视觉冲击1
        # turtle.fd(i)  # 长度
        # 视觉冲击2
        turtle.circle(i)
        # ......
        turtle.rt(60 + 5 - 5)  # 角度 + 偏移量
    turtle.done()


# 摘录,3D玫瑰花
def draw_3d_rose():
    import numpy as np
    import matplotlib.pyplot as plt
    from mpl_toolkits.mplot3d import Axes3D
    fig = plt.figure()
    ax = fig.gca(projection='3d')
    # 将相位向后移动了6*pi
    [x, t] = np.meshgrid(np.array(range(25)) / 24.0, np.arange(0, 575.5, 0.5) / 575 * 20 * np.pi + 4*np.pi)
    p = (np.pi / 2) * np.exp(-t / (8 * np.pi))
    # 添加边缘扰动
    change = np.sin(15*t)/150
    # 将t的参数减少，使花瓣的角度变大
    u = 1 - (1 - np.mod(3.3 * t, 2 * np.pi) / np.pi) ** 4 / 2 + change
    y = 2 * (x ** 2 - x) ** 2 * np.sin(p)
    r = u * (x * np.sin(p) + y * np.cos(p))
    h = u * (x * np.cos(p) - y * np.sin(p))
    c= plt.get_cmap('Reds')
    surf = ax.plot_surface(r * np.cos(t), r * np.sin(t), h, rstride=1, cstride=1,
                           cmap= c, linewidth=0, antialiased=True)
    plt.show()


