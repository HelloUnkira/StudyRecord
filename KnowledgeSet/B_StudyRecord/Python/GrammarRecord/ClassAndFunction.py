

'''
# 多线程与多进程
import threading
import multiprocessing
import time


class MyThread(threading.Thread):
    def __init__(self, name):
        super(MyThread, self).__init__()
        self.name = name
        self.count = 0

    def run(self):
        while True:
            self.count += 1
            print("Thread name:%s count:%d" % (self.name, self.count))
            if self.count > 5:
                break
            time.sleep(1)


class MyProcess(multiprocessing.Process):
    def __init__(self, name):
        super(MyProcess, self).__init__()
        self.thread1 = None  # 意外错误捕获:不可以在初始化时调用线程构造
        self.thread2 = None  # 意外错误捕获:不可以在初始化时调用线程构造
        self.name = name
        self.count = 0

    def run(self):
        # 该进程创建俩个线程并运行
        self.thread1 = MyThread(self.name + '_Thread1')
        self.thread2 = MyThread(self.name + '_Thread2')
        self.thread1.start()
        self.thread2.start()
        while True:
            self.count += 1
            print("Process name:%s count:%d" % (self.name, self.count))
            if self.count > 10:
                break
            time.sleep(1)
        self.thread1.join()
        self.thread2.join()


if __name__ == '__main__':
    # 该进程创建俩个线程,俩个进程
    process1 = MyProcess('main' + '_Process1')
    process2 = MyProcess('main' + '_Process2')
    thread1 = MyThread('main' + '_Thread1')
    thread2 = MyThread('main' + '_Thread2')
    process1.start()
    process2.start()
    thread1.start()
    thread2.start()
    process1.join()
    process2.join()
    thread1.join()
    thread2.join()
    print('--over--')
'''

'''
# 摘要模块
import hashlib
import hmac

print(hashlib.md5(''.encode('utf8')).hexdigest())
print(hashlib.sha1(''.encode()).hexdigest())
print(hashlib.sha224(''.encode()).hexdigest())
print(hashlib.sha256(''.encode()).hexdigest())
print(hashlib.sha384(''.encode()).hexdigest())
# hmac
encode_key = 'encode key'
text = 'need encode text'
encode_text = hmac.new(encode_key, text).hexdigest()
'''

'''
#turtle类
import turtle
help(turtle)
input()
#
turtle.speed(1)      #指针调速1-10
turtle.pensize(50)   #调整线的粗细
turtle.undo()        #取消最后一个图操作
turtle.done()        #结束绘制
#
turtle.forward(100)  #指针前进(留下落点)
turtle.backward(100) #指针后退(方向不变)
turtle.penup()       #抬起画笔,指针移动无效
turtle.pendown()     #放下画笔,指针移动有效
#
turtle.goto(0,50)    #指针重定位(绝对未知)
turtle.setx(x)       #指针x轴重定位
turtle.sety(y)       #指针y轴重定位
turtle.home()        #定位(0,0),方向为右
turtle.setheading(0) #指针设定指定角度:东0,北90,西180,南270
turtle.right(90)     #指针方向向右调整,角度
turtle.left(90)      #指针方向向左调整,角度
#
turtle.circle(50)    #指针以指定半径绘制圆
turtle.circle(...)   #指针以指定半径,范围,阶数绘制圆
                     #阶数被指定,绘制正多边形
turtle.dot(...)      #指针以指定直径和颜色绘制圆
#
turtle.color("red")     #设置指针颜色
turtle.fillcolor("red") #设置指针颜色填充
turtle.begin_fill()     #开始填充
turtle.end_fill()       #结束填充
turtle.filling()        #填充状态返回:True填充,False未填充
turtle.clear()          #清除绘制,保留指针状态
turtle.reset()          #清除绘制,重置指针状态
turtle.screensize(w,h)  #设置画布宽度和高度
turtle.hideturtle()     #隐藏指针
turtle.showturtle()     #显示指针
turtle.isvisible()      #指针可见返回True
turtle.write('',1,type) #当前落点书写文字,或指定字体大小和字体类型

'''

'''
#string类(不可变对象, 接近内置)
string = str('...')   #string = '...'
string.isalnum()
string.isalpha()
string.isdigit()
string.islower()
string.isupper()
string.isspace()
string.isidentifier() #Python标识符
string.startswith('...') #以子串开头
string.endswith('...')   #以子串结尾
string.find('...')  #查找子串最低下标(-1)
string.rfind('...') #查找子串最高下标(-1)     
string.count('...') #查找子串次数(无覆盖)
string.replace(old,new) #替换指定字符
string.capitalize()     #首字符大写
string.swapcase()       #大写小写逆转
string.title() #每个单词首字母转大写
string.lower() #全转小写
string.upper() #全转大写
string.strip()  #移除两端'',\t,\f,\r,\n
string.lstrip() #移除前端...
string.rstrip() #移除末端...
string.center(width) #给定宽度居中对齐
string.ljust(width)  #给定宽度左对齐
string.rjust(width)  #给定宽度右对齐
string.format(items) #格式化字符串,按照items
string.split('?')    #参考限定字符切割成列表(默认空格)

#一堆运算符重载了(下述是摘要):
string[index]       #范围[0,length),[-length,-1]
string[start:end]   #提取部分字符[start,end]
                    #start默认0,end默认length-1

string = string * n #复制连接

result = string1 in string2
result = string1 not in string2

'''

'''
#list类(接近内置)
list = list()   #list = []
list.index(element)         #第一次出现下标
list.count(element)         #统计元素
list.append(element)        #追加元素
list.extend(list)           #追加列表
list.sort()                 #升序排序
list.reverse()              #逆序
list.insert(index, element) #插入元素到index中
list.pop(index)             #弹出指定元素,默认最后一个元素
list.remove(element)        #移除指定元素(第一个)




#一堆运算符重载了(下述是摘要):
list[index]       #范围[0,length),[-length,-1]
list[start:end]   #提取部分字符[start,end]
                  #start默认0,end默认length-1

list = list * n   #复制连接

result = element in list
result = element not in list

#列表解析:
list = [i for i in range(9)]
list = [i for i in range(9) if i > 5]


'''

'''
#tuple类(不可变对象, 接近内置)
tuple = tuple(object)

#支持各种内置操作
'''

'''
#set类(接近内置)
set = set(object)
set.add(element)              #添加元素
set.remove(element)           #移除元素
set.issubset(set)             #子集检查
set.issuperset(set)           #超集检查
set.union(set)                #或运算(并集)
set.intersection(set)         #与运算(交集)
set.difference(set)           #(差集,补集)
set.symmetric_difference(set) #异或运算(对称差集)
'''

'''
#dictionary类
dictionary = {...}
dictionary[key] = value #添加(修改)一个条目
del dictionary[key]     #删除一个条目
dictionary.key()        #获得关键字序列(元组)
dictionary.values()     #获得值序列(元组)
dictionary.items()      #获得字典序列(元组序列,每个元组是一个键值对)
dictionary.clear()      #移除所有条目
dictionary.get(key)     #获得关键字对应值
dictionary.pop(key)     #获得关键字对应值,并移除条目
dictionary.popitem()    #随机获得一个条目,并移除条目

'''

'''
#random类
number = random.randint(a,b)   #生成[a,b]范围内整数
number = random.randrange(a,b) #生成[a,b)范围内整数
number = random.random()       #生成[0,1]范围内小数
'''

'''
#math库
#返回x最接近的数(整数/指定位数小数)
number = round(x)
number = round(x,x1)
#向上(向下)取整
number = ceil(x)
number = floor(x)

number = abs(x)
number = fabs(x)
number = max(x1,x2,...)
number = min(x1,x2,...)
number = pow(a,b)
number = exp(x)
number = log(x)         #log(e,x)
number = log(x,a)       #log(a,x)
number = sqrt(x)
number = sin(x)
number = cos(x)
number = asin(x)
number = acos(x)
number = tan(x)
number = degress(x)     #弧度转角度
number = radians(x)     #角度转弧度
'''

'''
#string库
number = ord(char)      #ASCII码值
string = chr(number)    #转字符串(一个字符的)
#格式化字符串或数字,根据格式说明符
string = format(item,format)
string = format(number, '5d')     #10进制
string = format(number, '5x')     #16进制
string = format(number, '5o')     #8进制
string = format(number, '5b')     #2进制
string = format(number, '.1f')
string = format(number, '10.1f')
string = format(number, '10.1e')  #科学记数法
string = format(number, '10.1f%') #10进制
string = format(number, '<10.1f') #左对齐
string = format(string, '20s')
string = format(string, '<20s')   #左对齐
string = format(string, '>20s')   #右对齐

number = len(string)
string = max(string)
string = min(string)

'''
