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


# 一朵简单的小心心
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







