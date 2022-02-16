
import json
import os
import os.path

#编写集成化源文件
def encode_event_set_c(file, list):
    #写点简要的说明
    file.write(('/*\n *外部引用,显式引用\n').encode())
    file.write((' *因为依赖是脚本提供的\n').encode())
    file.write((' *所以不采用引入头文件的方式\n */\n').encode())
    file.write(('#include "event_set.h"\n\n').encode())
    #善后措施
    file.write(('extern void event_set_auxiliary_respond(event_set *event);\n').encode())
    #提取所有外源依赖
    for items in list:          #列表中是字典
        for item in items:      #从字典提取关键字和值
            if item == 'name':
                string  = '/* ' + items[item] + ' */\n'
                file.write(string.encode())
            if item == 'brief':
                string  = '/* ' + items[item] + ' */\n'
                file.write(string.encode())
            if item == 'response':
                for element in items[item]:
                    string  = 'extern void ' + element + '(event_set *event);\n'
                    file.write(string.encode())
    file.write(('\n\n').encode())
    #生成派发函数
    file.write(("extern void event_set_respond(event_set *event)\n").encode())
    file.write(('{\n').encode())
    file.write(('\tswitch (event->command) {\n').encode())
    for items in list:          #列表中是字典
        tag_case = 0
        for item in items:      #从字典提取关键字和值
            if item == 'name':
                tag_case = 1
                string  = '\tcase ' + items[item] + ' :\n'
                file.write(string.encode())
            if item == 'response':
                for element in items[item]:
                    string  = '\t\t' + element + '(event);\n'
                    file.write(string.encode())
        if tag_case == 1:
            string  = '\t\tbreak;\n'
            file.write(string.encode())
        
    file.write(('\t}').encode())
    file.write(('\n\tif (event->auxiliary != 0)').encode())
    file.write(('\n\t\tevent_set_auxiliary_respond(event);').encode())
    file.write(('\n}\n').encode())

#编写集成化头文件
def encode_event_set_h(file, list):
    file.write(('\n#ifndef ' + ('event_set_h').upper()).encode())
    file.write(('\n#define ' + ('event_set_h').upper()).encode())
    file.write(('\n').encode())
    file.write(('//C std lib\n').encode())
    file.write(('#include <stdint.h>\n').encode())
    file.write(('\ntypedef struct {\n').encode())
    file.write(('\n\t/* 如果需要善后,设置标签为1,否则为0 */').encode())
    file.write(('\n\t/* 当事件的响应(0-n)全部执行完毕后 */').encode())
    file.write(('\n\t/* 使用回调对数据进行做最终处理 */').encode())
    file.write(('\n\tuint32_t command;   /* 响应的指令 */').encode())
    file.write(('\n\tuint32_t auxiliary; /* 善后措施 */').encode())
    file.write(('\n\tuint32_t length;    /* 数据元长度 */').encode())
    file.write(('\n\tvoid    *data;      /* 数据元 */\n').encode())
    file.write(('\n} event_set;\n').encode())
    file.write(('\n').encode())
    file.write(("extern void event_set_respond(event_set *event);\n").encode())
    file.write(('\n').encode())
    event_set_count = 0
    for items in list:          #列表中是字典
        for item in items:      #从字典提取关键字和值
            if item == 'name':
                event_set_count += 1
                string  = '#define ' + items[item] + '\t\t0x'
                string += format(event_set_count, '>04x') + '\n'
                file.write(string.encode())
    file.write(('\n#endif\n').encode())

#启用集成化事件集生成
def sys_eventset_encode():
    #json转Python字符串并转标准字典
    json_file   = open('event_set.json', 'r')
    json_string = json_file.read()
    dictionary  = json.loads(json_string)
    #仅支持批量化事件集管理
    if dictionary['type'] != 'event_set':
        return
    #从标准字典获取标准列表
    list = dictionary['event_set']
    #开启三个文件
    event_set_h = open('event_set.h', 'wb')
    event_set_c = open('event_set.c', 'wb')
    #解析
    encode_event_set_h(event_set_h, list)
    encode_event_set_c(event_set_c, list)
    #关闭三个文件
    eventset_h.close()
    eventset_c.close()
    json_file.close()

#####
sys_eventset_encode()
input('\nScript generation completed\n')

