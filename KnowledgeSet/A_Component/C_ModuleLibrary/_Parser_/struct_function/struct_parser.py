
import json

# -- coding: utf-8 --**

def struct_parser():
    #FileOperation
    file_in     = open('struct.json', 'r')
    file_out    = open('struct.h', 'w')
    file_in_str = file_in.read()
    file_in_dic = json.loads(file_in_str)
    print('-----parser start------')
    #部分成员
    name       = file_in_dic['name']
    typedef    = file_in_dic['typedef']
    property   = file_in_dic['property']
    type_width = file_in_dic['type_width']
    name_width = file_in_dic['name_width']
    #depend-on
    file_out.write('//C std lib\n')
    file_out.write('#include <stdint.h>\n\n')
    #struct
    file_out.write('//struct define\n')
    file_out.write('typedef struct ' + name + '\n{\n')
    #property
    for dic in property:
        file_out.write(('\t%-'+ str(type_width) +'s')%(dic['type']))
        file_out.write(('\t%-'+ str(name_width) +'s')%(dic['name'] + ';'))
        file_out.write('//')
        file_out.write(dic['comment'])
        file_out.write('\n')
    #struct
    file_out.write('\n} ' + typedef + ';\n\n')
    #set method
    file_out.write('//set method')
    for dic in property:
        file_out.write('\n')
        file_out.write('static inline void ' + typedef + '_set_' + dic['name'])
        file_out.write('(')
        file_out.write(typedef + ' *instance, '+ dic['type'] + ' ' + dic['name'])
        file_out.write(')')
        file_out.write('\n{\n')
        file_out.write('\tinstance->' + dic['name'] + ' = ' + dic['name'] + ';')
        file_out.write('\n}\n')
    #get method
    file_out.write('//get method')
    for dic in property:
        file_out.write('\n')
        file_out.write('static inline ' + dic['type'] + ' ' + typedef + '_get_' + dic['name'])
        file_out.write('(')
        file_out.write(typedef + ' *instance')
        file_out.write(')')
        file_out.write('\n{\n')
        file_out.write('\treturn instance->' + dic['name'] + ';')
        file_out.write('\n}\n')
    #FileOperation
    print('-----parser end--------')
    file_out.close()
    file_in.close()

'''parser'''
struct_parser()
