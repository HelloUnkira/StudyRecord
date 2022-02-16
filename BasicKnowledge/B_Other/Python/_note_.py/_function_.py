
'''
#基本交互
string = input('提示语句:')
number = eval(string)
string = str(number)
print('提示语句:',number,end = '\n')

#获取一个整数列表[start, end)
#start默认为0,step默认1(step可为负)
list = range(start, end, step)

#跨文件编辑
from 文件 import 目标(类,函数等)

#判定object是class的对象
#isinstance(object, class)

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

'''
#泽勒一致性算法:
temp = 0
temp += day
temp += (month + 1) * 26 / 10
temp += (year % 100) * (5 / 4)
temp += (year / 100) * (21 / 5)
#范围时[0,6] 
week = int(temp) % 7

'''








