
import os


# [file_name, file_size, file_offset]
def files_tar_recursion(record_list, path, prefix) -> None:
    for item in os.listdir(path):
        if os.path.isdir(path + '\\' + item):
            files_tar_recursion(record_list, path + '\\' + item, prefix)
        if os.path.isfile(path + '\\' + item):
            # if os.path.splitext(item)[1] in ['.c', '.h']: # 打包特定类型文件
            with open(path + '\\' + item, mode='rb+') as file:
                file_data = file.read()
                file_list = [prefix + '\\' + file.name.split(record_list[3])[1], len(file_data), record_list[2]]
                record_list[2] += file_list[1]
                # print(file_list)
                # 文件数据和名字分别写到不同文件中去
                record_list[0].write(str(str(file_list) + '\n').encode())
                record_list[1].write(file_data)


def files_tar(record_list) -> None:
    # 先创建一次确保文件存在
    with open(record_list[0], mode='w') as file1:
        pass
    with open(record_list[1], mode='w') as file2:
        pass
    with open(record_list[0], mode='rb+') as file_item:
        with open(record_list[1], mode='rb+') as file_info:
            # 我们需要引用传参
            args_list = [file_item, file_info, 0, '']
            for idx, path in enumerate(record_list[2]):
                args_list[3] = path
                files_tar_recursion(args_list, args_list[3], 'out' + str(idx))


# [file_name, file_size, file_offset]
def files_untar(record_list) -> None:
    # 提取所有信息到内存中
    with open('file_item.bin', mode='rb+') as record_item:
        record_item_lists = record_item.readlines()
    with open('file_info.bin', mode='rb+') as record_info:
        record_info_bytes = record_info.read()
    # 根据数据表切块提取各个文件的信息
    for item in record_item_lists:
        item = eval(item)
        # 确认文件夹是否存在
        path_local = item[0].rsplit('\\', 1)[0]
        os.makedirs(path_local, mode=0o777, exist_ok=True)
        # print(path_local)
        # 先创建一次确保文件存在
        with open(item[0], mode='w') as file:
            pass
        # 将数据流式写入该文件
        with open(item[0], mode='rb+') as file:
            file.write(record_info_bytes[item[2]:item[1]+item[2]])


if __name__ == '__main__':
    # tar流程
    print('---tar start---')
    files_tar(['file_item.bin', 'file_info.bin',
              [r'dir1', r'dir2']])
    print('---tar end---')

    # untar测试
    print('---untar start---')
    files_untar(['file_item.bin', 'file_info.bin'])
    print('---untar end---')
