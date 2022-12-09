// C std lib
// 使用时去查即可
// #include <***.h>

/*  获取文件属性
    int len = 0;
    struct stat f_atti;
    char f_info[1024] = {0};
    if (stat("filename", &f_atti) == -1)
        perror("operation false");
    len += sprintf(f_info + len, "Dev   Id:      %lu\n", (unsigned long)f_atti.st_dev);
    len += sprintf(f_info + len, "Inode Id:      %lu\n", (unsigned long)f_atti.st_ino);
    len += sprintf(f_info + len, "Perm Bit Info: %lu\n", (unsigned long)f_atti.st_mode);
    len += sprintf(f_info + len, "Hard Link Num: %lu\n", (unsigned long)f_atti.st_nlink);
    len += sprintf(f_info + len, "Owner Id:      %lu\n", (unsigned long)f_atti.st_uid);
    len += sprintf(f_info + len, "Group Id:      %lu\n", (unsigned long)f_atti.st_gid);
    len += sprintf(f_info + len, "Dev Id:        %lu\n", (unsigned long)f_atti.st_rdev);
    len += sprintf(f_info + len, "File Size:     %lu\n", (unsigned long)f_atti.st_size);
*/

/* 文件统计流程
    char f_info[1024] = {0};
    if (stat("filename", &f_atti) == -1)
        perror("operation false");
    if (!S_ISDIR(f_atti.st_mode)) {
        printf("普通文件,统计它");
    } else {
        printf("目录文件,迭代它");
        DIR *dir = opendir("dirname");
        if (dir == NULL && errno != 0)
            perror("operation false");
        while (1) {
            struct dirent *d = readdir(dir);
            if (d == NULL && errno == 0)
                break;
            if (d == NULL && errno != 0)
                errno = 0;
            if (strcmp(d->d_name, ".") ||
                strcmp(d->d_name, ".."))
                continue;
            //递归执行此流程
        }
   }
 */









