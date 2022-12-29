
file_name = 'SR.lds'
level = 7
priority = 7
gap = '  '

if __name__ == '__main__':
    print(file_name + ' make...')
    print(file_name + ' level:[0, %d]' % level)
    print(file_name + ' level:[0, %d]' % priority)
    with open(file_name, 'w', encoding='utf-8') as file:
        file.write('SECTIONS\n')
        file.write('{\n')
        file.write(gap + '.SR_Table :\n')
        file.write(gap + '{\n')
        file.write(gap + gap + '. = ALIGN(8);\n')
        file.write(gap + gap + 'SR_Table_S = .;\n')
        for i in range(0, level):
            for j in range(0, priority):
                file.write(gap + gap + 'KEEP(*(.SR_%d_%d))\n' % (i, j))
        file.write(gap + gap + 'SR_Table_E = .;\n')
        file.write(gap + '}\n')
        file.write('}\n')
