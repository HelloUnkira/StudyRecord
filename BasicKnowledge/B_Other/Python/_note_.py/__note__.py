
'''
#基本语法备注:
#if表达式,可省else
if  boolean-expression:
    ...
else:
    ...

#嵌套if表达式,可省else
if   boolean-expression:
    ...
elif boolean-expression:
    ...
else:
    ...

#逻辑与或非
#与: (expression1) and (expression2)
#或: (expression1) or (expression2)
#非: not (expression)

#条件表达式
result = result1 if condition else result2

#while表达式
while loop-condition:
    ...

#for表达式(可选:字符串,列表,数组等序列)
for i in sequence:
    ...

'''

'''
#文件传输:

#文件路径:
filename = 'C:\Users\xxx\Desktop\xxx.txt'

#文件存在性检查
import os.path
if os.path.isfile(filename):
    print('file exists');

#打开模式:'r','w','a','rb','wb'
mode = 'r'

file = open(filename, mode)

file.read(number)  #读取number个字符,默认全部字符
file.readline()    #读取下一行
file.readlines()   #读取全部行,列表组织

file.write(string) #写一个字符串

close(file)

'''

'''
#从网络获取数据:
import urllib.request
#网址:
url = 'http://www.baidu.com'
#打开一个文件
file = urllib.reques.urlopen(url)
#读取数据
#...
#Byte数据转字符串
string.decode()
'''

'''
#抛出异常:
raise ExceptionClass('...')
raise RuntimeError('...')
#...

#异常处理:
#在类似文件传输等情况,很适用
try:
    #...
except ExceptType1 as except:
    #...
except ExceptType2 as except:
    #...
else:
    #...
#任何情况都执行,收尾动作
finally:
    #...

#在可能不稳定的逻辑体中
#添加抛出异常的判断条件
#在异常捕获时接收对应的异常并处理

'''

'''
#自定义对象,描述为UML类图
#类数据元不存在绝对的私有属性

class father(object):
    def __init__(self):
        return

class ClassName:
    #构造函数(类数据元定义在此处):
    def __init__(self,member = 0):
        #继承时:
        father.__init__(self)
        #私有属性的定义:
        self.__member = member
        #公有属性的定义:
        self.member = member
    """
    #常用运算符重载:
    #逻辑运算符:
    def __lt__(self, other): #: <
    def __gt__(self, other): #: >
    def __le__(self, other): #: <=
    def __ge__(self, other): #: >=
    def __eq__(self, other): #: ==
    def __ne__(self, other): #: !=
    def __neg__(self): #: -self
    def __pos__(self): #: +self
    #算术运算符:
    #反向运算时,__rxxx__
    #赋值复合时,__ixxx__
    def __add__(self, other):      #: +
    def __sub__(self, other):      #: -
    def __mul__(self, other):      #: *
    def __mod__(self, other):      #: %
    def __pow__(self, other):      #: **
    def __truediv__(self, other):  #: /
    def __floordiv__(self, other): #: //
    #位运算符:
    #赋值复合时,__ixxx__
    def __invert__(self):        #: ~
    def __or__(self, other):     #: |
    def __and__(self, other):    #: &
    def __xor__(self, other):    #: ^
    def __lshift__(self, other): #: <<
    def __rshift__(self, other): #: >>
    #其余运算符:
    def __getitem__(self, index):  #: []
    def __contains__(self, value): #: in
    def __cmp__(self, other):      #: cmp
    def __len__(self):   #: len
    def __str__(self):   #: str
    def __int__(self):   #: int
    def __float__(self): #: float
    
    """
    #私有方法(成员函数):
    def __function(self):
        return
    #公有方法(成员函数):
    def function2(self,member = 0):
        return
    #覆盖方法:
    #重定义对应名称的方法即可
'''

