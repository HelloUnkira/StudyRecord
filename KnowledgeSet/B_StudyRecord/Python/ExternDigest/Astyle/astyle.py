# encoding=utf-8
# astyle  自动化递归astyle脚本
# astyle: https://astyle.sourceforge.net/
# 可执行源: .\astyle.exe
# 执行命令: 见execute
# 编译目标: .


import os
import os.path


compile = r'.\astyle.exe'
# execute = r'--style=allman --indent-switches --indent-preproc-block --min-conditional-indent=0 --break-blocks --pad-oper --pad-header --unpad-paren --align-pointer=name --break-closing-braces --break-one-line-headers --add-braces --max-code-length=120'
# execute = r'--style=allman -p -s4 -S -f -xW -w -xw'
execute = r'--style=allman'


def astyle_recure(path) -> None:
    for item in os.listdir(path):
        if os.path.isfile(path + '\\' + item):
            if item.split('.')[-1] == 'c' or item.split('.')[-1] == 'h':
                print(compile + ' ' + path + '\\' + item + ' ' + execute)
                os.system(compile + ' ' + path + '\\' + item + ' ' + execute)
        if os.path.isdir(path + '\\' + item):
            astyle_recure(path + '\\' + item)


def astyle_recure_clean(path) -> None:
    for item in os.listdir(path):
        if os.path.isfile(path + '\\' + item):
            if item.split('.')[-1] == 'orig':
                os.remove(path + '\\' + item)
        if os.path.isdir(path + '\\' + item):
            astyle_recure_clean(path + '\\' + item)


if __name__ == '__main__':
    astyle_recure(r'.')
    astyle_recure_clean(r'.')
    input('astyle recuse and clean orig finish')

