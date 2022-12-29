
问题点1:
    warning: '__aligned' attribute directive ignored [-Wattributes]
    Linux PC下无法执行该属性对齐

命令:
    调试时发现链接脚本可以联合而不是合并写入到一起
    gcc *.c -TBase.lds -TSR.lds -o Out
