# 我甚至不知道我为什么还需要做这些,也许就只是无聊,仅仅
# 获取国内资源 : pip install 包名 -i http://pypi.douban.com/simple/ --trusted-host pypi.douban.com
# 设置国内资源 : pip config set global.index-url http://pypi.douban.com/simple/
# 语法明细网站 : https://www.runoob.com/python3/python3-func-enumerate.html
# 常用模块:os,sys,math,random,datetime,time,calendar


'''
# 条件表达式: expression if condition else expression
# 循环推导式: expression for element in sequence
# 混合推导式: expression for element in sequence if condition
'''


'''
# 可变位置参数(*args, 一个元组),可变关键字参数(**kwargs, 一个字典)
def function(parameter1, parameter2, *args, **kwargs) :
    print('parameter1:', parameter1)
    print('parameter2:', parameter2)
    print('args = {}'.format(args))
    print('kwargs = {}'.format(kwargs))


function(1, 2, 'args1', 'args2', kwargs1 = 'kwargs1', kwargs2 = 'kwargs')
'''


'''
# 上下文管理器
class Context(object):
    def __enter__(self):
        print('__enter__')
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        print('__exit__')

    def execute(self):
        print('execute:', self)


with Context() as context:
    context.execute()
    print(context)
'''


'''
# 装饰器
def outer(callback):
    def inner(parameter, *args, **kwargs):
        if type(parameter) is not str:
            print('need to decorate')
        else:
            callback(parameter)
    return inner


@outer
def target_function(parameter):
    print('do not need to decorate')


target_function(1.0)
target_function(str('string'))
'''


'''
# 生成器
def maker(edge)
    for index in range(0, edge)
        yield index
'''


'''
# 抛出异常:
raise ExceptionClass('...')
raise RuntimeError('...')
# ...

# 异常处理:
# 在类似文件传输等情况,很适用
try:
    # ...
except ExceptType1 as except:
    # ...
except ExceptType2 as except:
    # ...
else:
    # ...
# 任何情况都执行,收尾动作
finally:
    # ...

# 在可能不稳定的逻辑体中
# 添加抛出异常的判断条件
# 在异常捕获时接收对应的异常并处理
'''


'''
# 自定义异常
class CustomException(Exception):
    def __init__(self, string):
        self.string = string

    def __str__(self):
        return self.string


try:
    x = 1000 % 0
    print x
except Exception:
    raise CustomException(str(Exception.message))
finally:
    pass
'''


'''
# 自定义对象,描述为UML类图
# 类数据元不存在绝对的私有属性

class Class(ClassFather):
    # 构造函数(类数据元定义在此处):
    def __init__(self, member = 0):
        # 私有属性的定义:
        self.__member = member
        # 公有属性的定义:
        self.member = member
    """
    # 常用运算符重载:
    # 逻辑运算符:
    def __lt__(self, other): # : <
    def __gt__(self, other): # : >
    def __le__(self, other): # : <=
    def __ge__(self, other): # : >=
    def __eq__(self, other): # : ==
    def __ne__(self, other): # : !=
    def __neg__(self): # : -self
    def __pos__(self): # : +self
    # 算术运算符:
    # 反向运算时,__rxxx__
    # 赋值复合时,__ixxx__
    def __add__(self, other):      # : +
    def __sub__(self, other):      # : -
    def __mul__(self, other):      # : *
    def __mod__(self, other):      # : %
    def __pow__(self, other):      # : **
    def __truediv__(self, other):  # : /
    def __floordiv__(self, other): # : //
    # 位运算符:
    # 赋值复合时,__ixxx__
    def __invert__(self):        # : ~
    def __or__(self, other):     # : |
    def __and__(self, other):    # : &
    def __xor__(self, other):    # : ^
    def __lshift__(self, other): # : <<
    def __rshift__(self, other): # : >>
    # 其余运算符:
    def __getitem__(self, index):  # : []
    def __contains__(self, value): # : in
    def __cmp__(self, other):      # : cmp
    def __len__(self):   # : len
    def __str__(self):   # : str
    def __int__(self):   # : int
    def __float__(self): # : float
    
    """
    # 私有方法(成员函数):
    def __function(self):
        return
    # 公有方法(成员函数):
    def function2(self, member = 0):
        return
    # 覆盖方法:
    # 重定义对应名称的方法即可

# 强行访问私有变量: Object._Class__MemberName
'''


'''
# Python带参多继承((*args, **kwargs)解决传参数量问题)

class Ancestor(object):
    def __init__(self, ancestor, *args, **kwargs):
        self.ancestor = ancestor
        print('Ancestor', ancestor)


class Parent1(Ancestor):
    def __init__(self, parent1, ancestor, *args, **kwargs):
        super(Parent1, self).__init__(ancestor, *args, **kwargs)
        self.parent1 = parent1
        print('ParentType1', parent1)


class Parent2(Ancestor):
    def __init__(self, parent2, ancestor, *args, **kwargs):
        super(Parent2, self).__init__(ancestor, *args, **kwargs)
        self.parent2 = parent2
        print('ParentType2', parent2)


class Parent3(Ancestor):
    def __init__(self, parent3, ancestor, *args, **kwargs):
        super(Parent3, self).__init__(ancestor, *args, **kwargs)
        self.parent3 = parent3
        print('ParentType3', parent3)


class Child(Parent1, Parent2, Parent3):
    def __init__(self, child, parent1, parent2, parent3, ancestor, *args, **kwargs):
        print Child.__mro__
        super(Child, self).__init__(parent1, parent2, parent3, ancestor, *args, **kwargs)
        self.child = child
        print('Child', child)


instance = Child('Child', 'Parent1', 'Parent2', 'Parent3', 'Ancestor')
print(instance.child, instance.parent1, instance.parent2, instance.parent3, instance.ancestor)
'''


