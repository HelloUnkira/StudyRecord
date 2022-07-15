
#ifndef LOG(format, ...)
#define LOG(format, ...)
#endif

static DL_BYTE1 dlelf_strcmp(DL_CHAR1 *str1, DL_CHAR1 *str2)
{
    while(*str1 != '\0' && *str2 != '\0' && *str1 == *str2) {
        str1++;
        str2++;
    }
    if (*str1 == '\0' && *str2 == '\0')
        return DL_TRUE;
    return DL_FALSE;
}

static DL_BYTE4 dlelf_strlen(DL_CHAR1 *str)
{
    DL_BYTE4 len = 0;
    while (*str != '\0') {
        len++;
        str++;
    }
    return len;
}

static DL_BYTE1 dlelf_memcmp(DL_BYTE1 *str1, DL_BYTE1 *str2, DL_BYTE4 len)
{
    DL_BYTE4 i = 0;
    for (i = 0; i < len; i++)
        if (str1[i] != str2[i])
            return DL_FALSE;
    return DL_TRUE;
}

static void dlelf_memset(DL_BYTE1 *str, DL_BYTE1 data, DL_BYTE4 len)
{
    DL_BYTE4 i = 0;
    for (i = 0; i < len; i++)
        str[i] = data;
}

static void dlelf_memcpy(DL_BYTE1 *str1, DL_BYTE1 *str2, DL_BYTE4 len)
{
    DL_BYTE4 i = 0;
    for (i = 0; i < len; i++)
        str1[i] = str2[i];
}

//加载共享对象
DL_CHAR4 dlmod_load_shared_object(dlmod_t *module, void *module_ptr)
{
    if (DL_NULL == module ||
        DL_NULL == module_ptr)
        return DL_FALSE;
    //
    Elf32_Addr vstart_addr = DL_NULL;
    Elf32_Addr vend_addr   = DL_NULL;
    Elf32_Addr vmem_space  = DL_NULL;
    //
    DL_BYTE1 linked   = DL_FALSE;
    DL_BYTE1 vstarted = RT_FALSE;
    DL_BYTE4 size = 0;
    //
    DL_BYTE4 index = 0;
    //
    //1.检查e_ident魔数
    linked = dlelf_memcmp(elf_module->e_ident, RTMMAG, SELFMAG); 
    //2.得到ELF映像大小(顺序加载各个段)
    for (index = 0; index < elf_module->e_phnum; index++) {
        //1.段类型不是可加载类型
        if (phdr[index].p_type != PT_LOAD)
            continue;
        //2.段内存占用小于段文件尺寸
        if (phdr[index].p_memsz < phdr[index].p_filesz)
            return DL_FALSE;
        //3.1.第一个可加载段到来
        if (vstarted == DL_FALSE) {
            vstarted  = DL_TRUE;
            //获取该段的起始地址
            //计算该段的结束地址
            vstart_addr = phdr[index].p_vaddr;
            vend_addr   = phdr[index].p_vaddr + phdr[index].p_memsz;
            //段地址溢出
            if (vend_addr < vstart_addr)
                return DL_FALSE;
        }
        //3.2.其余可加载段到来
        if (vstarted == DL_TRUE) {
            //在对象文件中不应该有太多的填充
            //段地址溢出
            if (phdr[index].p_vaddr < vend_addr)
                return DL_FALSE;
            //计算该段的结束地址
            vend_addr = phdr[index].p_vaddr + phdr[index].p_memsz;
            //段地址溢出
            if (vend_addr < phdr[index].p_vaddr)
                return DL_FALSE;
        }
    }
    //4.计算模块的大小
    size = vend_addr - vstart_addr;
    //
    if (size == 0)
        return DL_FALSE;
    //4.为模块分配空间用于拷贝ELF
    vmem_space = dl_malloc(size);
    if (vmem_space == DL_NULL)
        return DL_FALSE;
    //4.重置分配的空间
    dlelf_memset(vmem_space, 0, size);
    //4.确认模块参数,关联此ELF
    module->vstart_addr = vstart_addr; //设置首地址为ELF的起始地址
    module->mem_space = vmem_space;    //设置生成空间
    module->mem_size = size;           //设置生成空间大小
    module->nref = 0;                  //引用计数重置为0
    //4.设置模块入口
    module->entry_addr  = module->mem_space;
    module->entry_addr += elf_module->e_entry;
    module->entry_addr -= vstart_addr;
    //5.拷贝ELF各个段到模块中
    for (index = 0; index < elf_module->e_phnum; index++)
        //选取一个可加载段
        if (phdr[index].p_type == PT_LOAD)
            //拷贝一个可加载段
            dlelf_memcpy(module->mem_space + phdr[index].p_vaddr - vstart_addr,
                        (DL_BYTE1 *)elf_module + phdr[index].p_offset,
                         phdr[index].p_filesz);
    //6.处理可重定向段
    for (index = 0; index < elf_module->e_shnum; index++) {
        //
        static DL_BYTE1 unsolved = DL_FALSE;
        //
        DL_BYTE1  *temp   = DL_NULL;
        DL_BYTE1  *strtab = DL_NULL; //字符串表追踪索引
        Elf32_Sym *symtab = DL_NULL; //符号表追踪索引
        Elf32_Rel *rel    = DL_NULL; //重定位项追踪索引
        //可重定位项的数目,及其迭代器
        DL_BYTE4 nr_reloc = 0, i = 0;
        //是一个可重定向段
        if (!IS_REL(shdr[index]))
            continue;
        //得到一个可重定位项
        temp   = (DL_BYTE1 *)module_ptr;
        temp  += shdr[index].sh_offset;
        rel    = (Elf32_Rel *)temp;
        //加载.rel.plt段和.rel.dyn段
        //加载段字符串表
        temp   = (DL_BYTE1 *)module_ptr;
        temp  += shdr[shdr[shdr[index].sh_link].sh_link].sh_offset;
        strtab = temp;
        //加载段符号表
        temp   = (DL_BYTE1 *)module_ptr;
        temp  += shdr[shdr[index].sh_link].sh_offset
        symtab = (Elf32_Sym *)temp;
        //计算可重定位项的数目
        nr_reloc = (DL_BYTE4)(shdr[index].sh_size / sizeof(Elf32_Rel));
        //重定位每一个项
        for (i = 0; i < nr_reloc; i++, rel++) {
            //提取重定位信息的符号表索引,并获得对应的符号表
            Elf32_Sym *sym = &symtab[ELF32_R_SYM(rel->r_info)];
            //符号表段头不为闲置段或者符号表被BIND
            if ((sym->st_shndx != SHT_NULL) || (ELF_ST_BIND(sym->st_info) == STB_LOCAL)) {
                Elf32_Addr addr;
                addr = (Elf32_Addr)(module->mem_space + sym->st_value - vstart_addr);
                //重加载
                dlmod_relocate(module, rel, addr);
                continue;
            }
            //
            if (!linked) {
                //
                Elf32_Addr addr;
                //需要解析内核符号表中的符号
                addr = dlmod_symbol_find((DL_CHAR1 *)(strtab + sym->st_name));
                //内核符号表存在,重加载,不存在,记录错误
                if (addr != DL_NULL)
                    dlmod_relocate(module, rel, addr);
                if (addr == DL_NULL)
                    unsolved = DL_TRUE;
            }
        }
        if (unsolved) 
            return DL_FALSE;
    }
    //7.构造模块符号表
    //7.1.寻找.dynsym段
    for (index = 0; index < elf_module->e_shnum; index ++) {
        //
        DL_BYTE1 *shstrab;
        shstrab  = (DL_BYTE1 *)module_ptr;
        shstrab += shdr[elf_module->e_shstrndx].sh_offset;
        //检查符号表段字是否匹配
        if (dlelf_strcmp((DL_CHAR1 *)(shstrab + shdr[index].sh_name), ELF_DYNSYM))
            break;
    }
    //7.2.找到.dynsym段
    if (index != elf_module->e_shnum) {
        //
        DL_BYTE1   *temp    = DL_NULL;
        DL_BYTE1   *strtab  = DL_NULL; //字符串表追踪索引
        Elf32_Sym  *symtab  = DL_NULL; //符号表追踪索引
        //迭代器和计数器
        DL_BYTE4 i = 0, count = 0;
        //加载.dynsym段
        //加载段字符串表
        temp   = (DL_BYTE1 *)module_ptr;
        temp  += shdr[shdr[index].sh_link].sh_offset;
        strtab = temp;
        //加载段符号表
        temp   = (DL_BYTE1 *)module_ptr;
        temp  += shdr[index].sh_offset;
        symtab = (Elf32_Sym *)temp;
        //寻找符号表中全局代码段数目
        for (i = 0; i < shdr[index].sh_size / sizeof(Elf32_Sym); i++)
            if ((ELF_ST_BIND(symtab[i].st_info) == STB_GLOBAL) &&
                (ELF_ST_TYPE(symtab[i].st_info) == STT_FUNC))
                count++;
        //为模块分配空间用于拷贝全局代码段
        temp = dl_malloc(count * sizeof(dlsymtab_t));
        //记录模块的符号表及其数量
        module->symtab = (dlsymtab_t *)temp;
        module->nsym   = count;
        //寻找符号表中全局代码段并为其拷贝
        for (i = 0, count = 0; i < shdr[index].sh_size / sizeof(Elf32_Sym); i++) {
            DL_BYTE4 len = 0;
            //寻找符号表中全局代码段
            if ((ELF_ST_BIND(symtab[i].st_info) != STB_GLOBAL) ||
                (ELF_ST_TYPE(symtab[i].st_info) != STT_FUNC))
                continue;
            //获取段名长度
            len = dlelf_strlen((DL_CHAR1 *)(strtab + symtab[i].st_name)) + 1;
            //生成段名空间,清空它,然后拷贝段名
            temp = dl_malloc(len);
            dlelf_memset((DL_BYTE1 *)(module->symtab[count].name), 0, len);
            dlelf_memcpy((DL_BYTE1 *)(module->symtab[count].name),
                          strtab + symtab[i].st_name, len);
            //更新模块中的记录
            module->symtab[count].name = temp;
            temp  = module->mem_space;
            temp += symtab[i].st_value;
            temp -= module->vstart_addr;
            module->symtab[count].addr = temp;
            //进动到下一个记录点位
            count++;
        }

        //获取优先级和堆栈大小参数
        DL_BYTE4 flag = 0;
        for (i = 0; i < shdr[index].sh_size / sizeof(Elf32_Sym); i++) {
            DL_BYTE1 result    = 0;
            DL_BYTE2 priority  = 0;
            DL_BYTE4 stacksize = 0;
            //如果当前已经匹配到:下述各个字段,位图,或运算
            if ((flag & 0x03) == 0x03)
                break;
            //如果之前没有匹配到且当前已经匹配到:线程优先级信息段
            result = dlelf_memcmp((DL_CHAR *)(strtab + symtab[i].st_name), "dlmodule_thread_priority");
            if (((flag & 0x01) == 0) && (result == DL_TRUE)) {
                flag |= 0x01;
                //计算优先级存储地址
                temp  = module->mem_space;
                temp += symtab[i].st_value;
                temp -= module->vstart_addr;
                //计算优先级
                priority = *(rt_uint16_t*)(temp);
                if (priority >= DL_THREAD_PRIORITY_MAX)
                    continue;
                //如果有效则存储它
                module->priority = priority;
            }
            //如果之前没有匹配到且当前已经匹配到:线程堆栈大小信息段
            result = dlelf_memcmp((DL_CHAR *)(strtab + symtab[i].st_name), "dlmodule_thread_stacksize");
            if (((flag & 0x02) == 0) && (result == DL_TRUE)) {
                flag |= 0x02;
                //计算堆栈大小存储地址
                temp  = module->mem_space;
                temp += symtab[i].st_value;
                temp -= module->vstart_addr;
                //计算堆栈大小
                stacksize = *(rt_uint32_t*)(temp);
                if ((stacksize >= 1024 * 2) && (stacksize <= 1024 * 32))
                    continue;
                //如果有效则存储它
                module->stack_size = stacksize;
            }
        }
    }

    return DL_FALSE;
}

//加载可重定向对象
DL_CHAR4 dlmod_load_relocated_object(dlmod_t *module, void *module_ptr)
{
    Elf32_Addr bss_addr    = DL_NULL;
    Elf32_Addr data_addr   = DL_NULL;
    Elf32_Addr rodata_addr = DL_NULL;
    Elf32_Addr module_addr = DL_NULL;

    DL_BYTE1 *ptr     = DL_NULL;
    DL_BYTE1 *strtab  = DL_NULL;
    DL_BYTE1 *shstrab = DL_NULL;

    DL_BYTE4 index = 0;
    DL_BYTE4 size  = 0;
    
    //1.获得ELF映像大小
    for (index = 0; index < elf_module->e_shnum; index ++) {
        //获得模块起始地址
        if (IS_PROG(shdr[index]) && IS_AX(shdr[index]))
            module_addr = shdr[index].sh_addr;
        //计算几种类型的段的占用情况

        //1.text段,文本段
        if (IS_PROG(shdr[index]) && IS_AX(shdr[index]))
            size += shdr[index].sh_size;
        //2.rodata段,只读数据段
        if (IS_PROG(shdr[index]) && IS_ALLOC(shdr[index]))
            size += shdr[index].sh_size;
        //3.data段,数据段
        if (IS_PROG(shdr[index]) && IS_AW(shdr[index]))
            size += shdr[index].sh_size;
        //4.bss段
        if (IS_NOPROG(shdr[index]) && IS_AW(shdr[index]))
            size += shdr[index].sh_size;
    }
    //映像中没有上述的几个段存在
    if (size == 0)
        return DL_FALSE;
    //2.1.为模块生成段存储空间
    ptr = dl_malloc(size);
    if (ptr == DL_NULL)
        return DL_FALSE;
    //2.2.设置新生成的空间到模块内部
    dlelf_memset(ptr, 0, size);
    module->vstart_addr = DL_NULL;
    module->mem_space   = ptr;
    module->mem_size    = size;
    //3.加载文本段和数据段
    for (index = 0; index < elf_module->e_shnum; index++) {
        //
        if (IS_PROG(shdr[index]) && IS_ALLOC(shdr[index]))
            rodata_addr = (Elf32_Addr)ptr;
        if (IS_PROG(shdr[index]) && IS_AW(shdr[index]))
            data_addr = (Elf32_Addr)ptr;
        if (IS_NOPROG(shdr[index]) && IS_AW(shdr[index]))
            bss_addr = (Elf32_Addr)ptr;
        //1.加载text段
        //2.加载rodata段
        //3.加载data段
        if ((IS_PROG(shdr[index]) && IS_AX(shdr[index]))    ||
            (IS_PROG(shdr[index]) && IS_ALLOC(shdr[index])) ||
            (IS_PROG(shdr[index]) && IS_AW(shdr[index])) {
            dlelf_memcpy(ptr, (DL_BYTE1 *)elf_module +
                         shdr[index].sh_offset, shdr[index].sh_size);
            ptr += shdr[index].sh_size;
        }
        //4.加载bss段
        if (IS_NOPROG(shdr[index]) && IS_AW(shdr[index]))
            dlelf_memset(ptr, 0, shdr[index].sh_size);
    }

    //4.设置模块项函数
    ptr  = (DL_BYTE1 *)module->mem_space;
    ptr += elf_module->e_entry;
    ptr -= module_addr;  
    module->entry_addr = (dlmod_entry_t)ptr;

    //5.处理可重定向段
    for (index = 0; index < elf_module->e_shnum; index++) {
        //
        DL_BYTE1  *temp   = DL_NULL;
        Elf32_Sym *symtab = DL_NULL; //符号表追踪索引
        Elf32_Rel *rel    = DL_NULL; //重定位项追踪索引
        //可重定位项的数目,及其迭代器
        DL_BYTE4 nr_reloc = 0, i = 0;
        //是一个可重定向段
        if (!IS_REL(shdr[index]))
            continue;
        //得到一个可重定位项
        temp    = (DL_BYTE1 *)module_ptr;
        temp   += shdr[index].sh_offset;
        rel     = (Elf32_Rel *)temp;
        //加载.dynsym和.dynstr段
        //加载段字符串表
        temp    = (DL_BYTE1 *)module_ptr;
        temp   += shdr[shdr[shdr[index].sh_link].sh_link].sh_offset;
        strtab  = temp;
        //加载段符号表
        temp    = (DL_BYTE1 *)module_ptr;
        temp   += shdr[shdr[index].sh_link].sh_offset;
        symtab  = (Elf32_Sym *)temp;
        //
        temp    = (DL_BYTE1 *)module_ptr;
        temp   += shdr[elf_module->e_shstrndx].sh_offset;
        shstrab = temp;
        //计算可重定位项的数目
        nr_reloc = (DL_BYTE4)(shdr[index].sh_size / sizeof(Elf32_Rel));
        //重定位每一个项
        for (i = 0; i < nr_reloc; i++, rel++) {
            //
            Elf32_Addr addr = DL_NULL;
            //提取重定位信息的符号表索引,并获得对应的符号表
            Elf32_Sym *sym = &symtab[ELF32_R_SYM(rel->r_info)];
            //符号表段头不为未定义段
            if (sym->st_shndx != STN_UNDEF) {
                //
                if ((ELF_ST_TYPE(sym->st_info) == STT_SECTION) ||
                    (ELF_ST_TYPE(sym->st_info) == STT_OBJECT)) {
                    //
                    DL_BYTE1 result = 0;
                    //
                    ptr = (DL_BYTE1 *)(shstrab + shdr[sym->st_shndx].sh_name);
                    result = dlelf_memcmp(ptr, ELF_RODATA, 8);
                    //重定向rodata段
                    if (result == DL_TRUE)
                        addr = (Elf32_Addr)(rodata_addr + sym->st_value);
                    if (result == DL_TRUE)
                        continue;
                    //
                    ptr = (DL_BYTE1 *)(shstrab + shdr[sym->st_shndx].sh_name);
                    result = dlelf_memcmp(ptr, ELF_BSS, 5);
                    //重定向bss段
                    if (result == DL_TRUE)
                        addr = (Elf32_Addr)(bss_addr + sym->st_value);
                    if (result == DL_TRUE)
                        continue;
                    //
                    ptr = (DL_BYTE1 *)(shstrab + shdr[sym->st_shndx].sh_name);
                    result = dlelf_memcmp(ptr, ELF_DATA, 6);
                    //重定向data段
                    if (result == DL_TRUE)
                        addr = (Elf32_Addr)data_addr + sym->st_value;
                    if (result == DL_TRUE)
                        continue;
                    //
                    if (addr != 0)
                        dlmod_relocate(module, rel, addr);
                    continue;
                }
                //
                if (ELF_ST_TYPE(sym->st_info) == STT_FUNC) {
                    ptr  = module->mem_space;
                    ptr -= module_addr;
                    ptr += sym->st_value;
                    addr = (Elf32_Addr)ptr;
                    //重定向函数
                    dlmod_relocate(module, rel, addr);
                    continue;
                }
                continue;
            }
            //
            if (ELF_ST_TYPE(sym->st_info) == STT_FUNC)
            {
                ptr  = module->mem_space;
                ptr -= module_addr;
                ptr += sym->st_value;
                addr = (Elf32_Addr)ptr;
                //重定向函数
                dlmod_relocate(module, rel, addr);
                continue;
            }
            //
            if (1) {
                //
                if (ELF32_R_TYPE(rel->r_info) != R_ARM_V4BX) {
                    //需要解析内核符号表中的符号
                    addr = dlmod_symbol_find((DL_CHAR1 *)(strtab + sym->st_name));
                    //
                    if (addr != DL_NULL)
                        dlmod_relocate(module, rel, addr);
                }
                //
                if (ELF32_R_TYPE(rel->r_info) == R_ARM_V4BX) {
                    ptr  = module->mem_space;
                    ptr -= module_addr;
                    ptr += sym->st_value;
                    addr = (Elf32_Addr)ptr;
                    //重定向函数
                    dlmod_relocate(module, rel, addr);
                    continue;
                }
                continue;
            }
        }
    }

    return DL_TRUE;
}
