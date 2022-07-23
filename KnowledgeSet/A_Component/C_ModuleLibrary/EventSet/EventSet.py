
import json
import os
import os.path

#编写集成化源文件
def EncodeEventSetC(File, List):
    #写点简要的说明
    File.write(('/*\n *外部引用,显式引用\n').encode())
    File.write((' *因为依赖是脚本提供的\n').encode())
    File.write((' *所以不采用引入头文件的方式\n */\n').encode())
    File.write(('#include "EventSet.h"\n\n').encode())
    #善后措施
    File.write(('extern void EventSetAuxiliaryRespond(EventSet *Event);\n').encode())
    #提取所有外源依赖
    for Items in List:          #列表中是字典
        for Item in Items:      #从字典提取关键字和值
            if Item == 'Name':
                String = '/* ' + Items[Item] + ' */\n'
                File.write(String.encode())
            if Item == 'brief':
                String = '/* ' + Items[Item] + ' */\n'
                File.write(String.encode())
            if Item == 'Response':
                for element in Items[Item]:
                    String = 'extern void ' + element + '(EventSet *Event);\n'
                    File.write(String.encode())
    File.write(('\n\n').encode())
    #生成派发函数
    File.write(("extern void EventSetRespond(EventSet *Event)\n").encode())
    File.write(('{\n').encode())
    File.write(('\tswitch (Event->Command) {\n').encode())
    for Items in List:          #列表中是字典
        TagCase = 0
        for Item in Items:      #从字典提取关键字和值
            if Item == 'Name':
                TagCase = 1
                String = '\tcase ' + Items[Item] + ':\n'
                File.write(String.encode())
            if Item == 'Response':
                for element in Items[Item]:
                    String  = '\t\t' + element + '(Event);\n'
                    File.write(String.encode())
        if TagCase == 1:
            String = '\t\tbreak;\n'
            File.write(String.encode())
        
    File.write(('\t}').encode())
    File.write(('\n\tif (Event->Auxiliary != 0)').encode())
    File.write(('\n\t\tEventSetAuxiliaryRespond(Event);').encode())
    File.write(('\n}\n').encode())

#编写集成化头文件
def EncodeEventSetH(File, List):
    File.write(('\n#ifndef ' + ('event_set_h').upper()).encode())
    File.write(('\n#define ' + ('event_set_h').upper()).encode())
    File.write(('\n').encode())
    File.write(('//C std lib\n').encode())
    File.write(('#include <stdint.h>\n').encode())
    File.write(('\ntypedef struct EventSetBody {\n').encode())
    File.write(('\n\t/* 如果需要善后,设置标签为1,否则为0 */').encode())
    File.write(('\n\t/* 当事件的响应(0-n)全部执行完毕后 */').encode())
    File.write(('\n\t/* 使用回调对数据进行做最终处理 */').encode())
    File.write(('\n\tuint32_t Command;   /* 响应的指令 */').encode())
    File.write(('\n\tuint32_t Auxiliary; /* 善后措施 */').encode())
    File.write(('\n\tuint32_t Length;    /* 数据元长度 */').encode())
    File.write(('\n\tvoid    *Data;      /* 数据元 */\n').encode())
    File.write(('\n} EventSet;\n').encode())
    File.write(('\n').encode())
    File.write(("extern void EventSetRespond(EventSet *Event);\n").encode())
    File.write(('\n').encode())
    event_set_count = 0
    for Items in List:          #列表中是字典
        for Item in Items:      #从字典提取关键字和值
            if Item == 'Name':
                event_set_count += 1
                String  = '#define ' + Items[Item] + '\t\t0x'
                String += format(event_set_count, '>04x') + '\n'
                File.write(String.encode())
    File.write(('\n#endif\n').encode())

#启用集成化事件集生成
def EncodeEventSet():
    #json转Python字符串并转标准字典
    JsonFile   = open('EventSet.json', 'r')
    JsonString = JsonFile.read()
    Dictionary = json.loads(JsonString)
    #仅支持批量化事件集管理
    if Dictionary['Type'] != 'EventSet':
        return
    #从标准字典获取标准列表
    List = Dictionary['EventSet']
    #开启三个文件
    EventSetH = open('EventSet.h', 'wb')
    EventSetC = open('EventSet.c', 'wb')
    #解析
    EncodeEventSetH(EventSetH, List)
    EncodeEventSetC(EventSetC, List)
    #关闭三个文件
    EventSetH.close()
    EventSetC.close()
    JsonFile.close()

#####
EncodeEventSet()
input('\nScript generation completed\n')

