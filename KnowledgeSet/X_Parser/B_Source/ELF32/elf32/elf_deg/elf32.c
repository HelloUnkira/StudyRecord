#include <stdio.h>
#include "elf32.h"

#define ELF32_LENGTH (1024 * 1024 * 500)
static ELF32_1BYTE1 module_file[ELF32_LENGTH] = {0};
static ELF32_1BYTE1 kernel_file[ELF32_LENGTH] = {0};
//文件路径和文件名后续使用命令行参数去替换
//并对.exe + 参数进一步嵌套出.bat
#define ELF32_PATH   "C:\\Users\\dlh8101\\Desktop\\"\
                         "RT_Thread\\_elf32_\\"

#define ELF32_OUTPUT_NAME "output.elf"
#define ELF32_KERNEL_NAME "rtthread.elf"
#define ELF32_MODULE_NAME "hello.mo"

int main(int argc, char *argv[])
{
    ELF32_1BYTE4 i = 0;
    ELF32_0BYTE4 c = -1;

    //1.打开所需的elf文件
    FILE *fp0 = fopen(ELF32_PATH ELF32_OUTPUT_NAME, "wb");
    FILE *fp1 = fopen(ELF32_PATH ELF32_MODULE_NAME, "rb");
    FILE *fp2 = fopen(ELF32_PATH ELF32_KERNEL_NAME, "rb");
    if (fp0 == NULL || fp1 == NULL || fp2 == NULL) {
        printf("elf_file open error!!!\n");
        exit(-1);
    }
    //2.将内容载入到内存中
    ELF32_1BYTE4 module_count = 0;
    for (i = 0, c = -1; feof(fp1) == 0; i++)
        if ((c = fgetc(fp1)) != -1) {
            module_file[i] = (ELF32_1BYTE1)c;
            module_count++;
        }
    for (i = 0, c = -1; feof(fp2) == 0; i++)
        if ((c = fgetc(fp2)) != -1)
            kernel_file[i] = (ELF32_1BYTE1)c;

    //3.解析ELF
    //elf32_parse(module_file);
    //elf32_parse(kernel_file);
    //4.导出Kernel符号表
    Elf32_KSymtab ksymtab = {0};
    //elf32_export_ksymtab(kernel_file, &ksymtab);
    //5.复写module符号表
    
    
    //6.复写后数据回写
    //fwrite(module_file, sizeof(ELF32_1BYTE1), module_count, fp0);
    //7.关闭elf文件
    fclose(fp0);
    fclose(fp1);
    fclose(fp2);
    return 0;
}

//启用日志加载
#define ELF32_LOG   printf
//C89不存在变参宏,所以这里只能是空替换
//逗号表达式不影响代码正常运行
#ifndef ELF32_LOG
#define ELF32_LOG
#endif

#define ELF32_FIELD_PARSE
#ifdef  ELF32_FIELD_PARSE

void elf32_ehdr(Elf32_Ehdr *ehdr)
{
    ELF32_0BYTE4 i = 0;
    ELF32_LOG("\n");
    for (i = 0; i < EI_NIDENT; i++)
        ELF32_LOG("%c",(ehdr->e_ident)[i]);
    ELF32_LOG("\ne_type:%d",      ehdr->e_type);
    ELF32_LOG("\ne_machine:%d",   ehdr->e_machine);
    ELF32_LOG("\ne_version:%d",   ehdr->e_version);
    ELF32_LOG("\ne_entry:0x%x",   ehdr->e_entry);
    ELF32_LOG("\ne_phoff:0x%x",   ehdr->e_phoff);
    ELF32_LOG("\ne_shoff:0x%x",   ehdr->e_shoff);
    ELF32_LOG("\ne_flags:0x%x",   ehdr->e_flags);
    ELF32_LOG("\ne_ehsize:%d",    ehdr->e_ehsize);
    ELF32_LOG("\ne_phentsize:%d", ehdr->e_phentsize);
    ELF32_LOG("\ne_phnum:%d",     ehdr->e_phnum);
    ELF32_LOG("\ne_shentsize:%d", ehdr->e_shentsize);
    ELF32_LOG("\ne_shnum:%d",     ehdr->e_shnum);
    ELF32_LOG("\ne_shstrndx:%d",  ehdr->e_shstrndx);
    ELF32_LOG("\n");
}

void elf32_phdr(Elf32_Phdr *phdr, ELF32_1BYTE2 pidx)
{
    ELF32_0BYTE4 i = 0;
    for (i = 0; i < pidx; i++) {
        ELF32_LOG("\np_type:%d",     phdr[i].p_type);
        ELF32_LOG("\np_offset:0x%x", phdr[i].p_offset);
        ELF32_LOG("\np_vaddr:0x%x",  phdr[i].p_vaddr);
        ELF32_LOG("\np_paddr:0x%x",  phdr[i].p_paddr);
        ELF32_LOG("\np_filesz:0x%x", phdr[i].p_filesz);
        ELF32_LOG("\np_memsz:0x%x",  phdr[i].p_memsz);
        ELF32_LOG("\np_flags:%d",    phdr[i].p_flags);
        ELF32_LOG("\np_align:%d",    phdr[i].p_align);
        ELF32_LOG("\n");
    }
}

void elf32_shdr(Elf32_Shdr *shdr, ELF32_1BYTE2 sidx)
{
    ELF32_0BYTE4 i = 0;
    for (i = 0; i < sidx; i++) {
        ELF32_LOG("\nsh_name:%d",      shdr[i].sh_name);
        ELF32_LOG("\nsh_type:%d",      shdr[i].sh_type);
        ELF32_LOG("\nsh_flags:%d",     shdr[i].sh_flags);
        ELF32_LOG("\nsh_addr:0x%x",    shdr[i].sh_addr);
        ELF32_LOG("\nsh_offset:0x%x",  shdr[i].sh_offset);
        ELF32_LOG("\nsh_size:%d",      shdr[i].sh_size);
        ELF32_LOG("\nsh_link:%d",      shdr[i].sh_link);
        ELF32_LOG("\nsh_info:%d",      shdr[i].sh_info);
        ELF32_LOG("\nsh_addralign:%d", shdr[i].sh_addralign);
        ELF32_LOG("\nsh_entsize:%d",   shdr[i].sh_entsize);
        ELF32_LOG("\n");
    }
}

//PC端解析elf32各个字段
void elf32_parse(ELF32_1BYTE1 *elf)
{
    //获得ELF头表,并以此获得:段(节)头表,程序头表,对应项
    Elf32_Ehdr  *ehdr = ELF32_EHDR(elf);
    Elf32_Shdr  *shdr = ELF32_SHDR(elf, ehdr);
    Elf32_Phdr  *phdr = ELF32_PHDR(elf, ehdr);
    ELF32_1BYTE2 sidx = ehdr->e_shnum; //段(节)头表项
    ELF32_1BYTE2 pidx = ehdr->e_phnum; //程序头表项
    //
    elf32_ehdr(ehdr);
    elf32_phdr(phdr, pidx);
    elf32_shdr(shdr, sidx);
    //寻找到重修改位点
    
    
    
}

#endif

#define ELF32_LOAD_FLOW
#ifdef  ELF32_LOAD_FLOW

//PC端尝试导出内核符号表
void elf32_export_ksymtab(ELF32_1BYTE1 *elf, Elf32_KSymtab *ksymtab)
{
    ELF32_1BYTE2 index = 0;
    //获得ELF头表,并以此获得:段(节)头表,程序头表,对应项
    Elf32_Ehdr  *ehdr = ELF32_EHDR(elf);
    Elf32_Shdr  *shdr = ELF32_SHDR(elf, ehdr);
    Elf32_Phdr  *phdr = ELF32_PHDR(elf, ehdr);
    ELF32_1BYTE2 sidx = ehdr->e_shnum; //段(节)头表项
    ELF32_1BYTE2 pidx = ehdr->e_phnum; //程序头表项
    ELF32_1BYTE2 strn = ehdr->e_shstrndx; //段头字符串表
    //获取段头字符串表
    ELF32_1BYTE1 *strtab = NULL;
    strtab = (ELF32_1BYTE1 *)(elf + shdr[strn].sh_offset);
    //搜索指定段名的段
    for (index = 0; index < sidx; index++)
        if (strcmp(strtab + shdr[index].sh_name, ELF32_SYMTAB_SYM) == 0)
            break;
    if (index >= sidx)
        return;
    //确认内核符号表存储位置
    Elf32_KSym   *ksym    = elf + shdr[index].sh_offset;
    ELF32_1BYTE4  ksymidx = shdr[index].sh_size / sizeof(Elf32_KSym);  
    //获取符号表集合
    if (ksymidx == 0)
        return;
    //解析完布局后
    //统计内核符号表中符号数量
    ksymtab->ksym    = calloc(ksymidx, sizeof(Elf32_KSym));
    ksymtab->ksymidx = ksymidx;

    //提取内核符号表中符号
    for (index = 0; index < ksymidx; index++) {
        ELF32_0BYTE1 *name_posi  = NULL;
        ELF32_1BYTE4  name_size = 0;
        ELF32_0BYTE1 *name      = NULL;
        //获得符号存储位置,计算符号长度
        name_posi = (ELF32_0BYTE1 *)(elf + ksym[index].name);
        name_size = strlen(name_posi);
        name = malloc(name_size);
        memcpy(name, name_posi, name_size);
        //保存到内存中
        ksymtab->ksym[index].addr = ksym[index].addr;
        ksymtab->ksym[index].name = name;
    }
}

//PC端尝试完成内核符号表的静态导入
void elf32_load_ksymtab(ELF32_1BYTE1 *elf, Elf32_KSymtab *ksymtab)
{
    ELF32_1BYTE2 index = 0;
    //获得ELF头表,并以此获得:段(节)头表,程序头表,对应项
    Elf32_Ehdr  *ehdr = ELF32_EHDR(elf);
    Elf32_Shdr  *shdr = ELF32_SHDR(elf, ehdr);
    Elf32_Phdr  *phdr = ELF32_PHDR(elf, ehdr);
    ELF32_1BYTE2 sidx = ehdr->e_shnum; //段(节)头表项
    ELF32_1BYTE2 pidx = ehdr->e_phnum; //程序头表项
    //处理可重定向段
    for (index = 0; index < pidx; index++) {
        //检查1:跳过非可重定向段
        if (!IS_REL(shdr[index]))
            continue;
        ELF32_1BYTE4 i       = 0;
        ELF32_1BYTE4 rel_n   = 0;
        ELF32_1BYTE2 link    = index;
        ELF32_1BYTE1 *strtab = NULL;
        Elf32_Sym    *symtab = NULL;
        Elf32_Rel    *rel    = NULL;
        //加载可重定位项(首项)
        rel = (Elf32_Rel *)(elf + shdr[link].sh_offset);
        //(从可重定位项进行一次链接到达符号表段)
        link = shdr[link].sh_link;
        //加载符号表段
        symtab = (Elf32_Sym *)(elf + shdr[link].sh_offset);
        //(从符号表段进行一次链接到达字符串表段)
        link = shdr[link].sh_link;
        //加载字符串表段
        strtab = elf + shdr[link].sh_offset;
        //计算可重定位项的数目
        rel_n  = shdr[index].sh_size / sizeof(Elf32_Rel);
        //重定位与内核符号表交织的项
        Elf32_Sym    *sym = NULL;
        ELF32_1BYTE1 *str = NULL;
        ELF32_1BYTE4 addr = 0;
        for (i = 0; i < rel_n; i++, rel++) {
            //通过重定位项的内部信息获取符号表项
            link = ELF32_R_SYM(rel->r_info);
            //动态加载时会额外做加载的部分,无需关心
            sym = &symtab[link];
            //如果该符号存在节绑定(该符号在对应节内使用)
            if (sym->st_shndx != SHT_NULL)
                continue;
            //如果该符号是本地符号
            if (ELF_ST_BIND(sym->st_info) == STB_LOCAL)
                continue;
            //重定位该符号,使用内核的符号表
            //原:dlmodule_symbol_find,通过符号名称获得
            //对应符号的地址,此处复刻该做法
            
            //1.通过索引获得字符串表中的字符串(符号名称)
            str = strtab + sym->st_name;
            //2.通过该符号名称获得内核中对应的地址
            ELF32_1BYTE4 addr = 0;
            ELF32_1BYTE4 i = 0;
            for (i = 0; i < ksymtab->ksymidx; i++)
                if (strcmp(ksymtab->ksym[i].name, str) == 0) {
                    addr = ksymtab->ksym[i].addr;
                    break;
                }
            if (addr == 0)
                return;
            //3.将该地址导入到ELF中
            //复写可重定位的偏移量
            ELF32_1BYTE4 *taddr = NULL;
            taddr = (ELF32_1BYTE4 *)(elf + rel->r_offset);
            
            switch (ELF32_R_TYPE(rel->r_info)) {
            case R_ARM_NONE:
                break;
            case R_ARM_ABS32:   //复写32位绝对地址为相对0地址做累加
                *taddr += (ELF32_1BYTE4)addr;
                break;
            case R_ARM_PC24:
            case R_ARM_PLT32:
            case R_ARM_CALL:
            case R_ARM_JUMP24: {//复写24位绝对地址为相对0地址做累加
                ELF32_1BYTE4 addend = 0;
                addend = *taddr & 0x00ffffff;
                if (addend  & 0x00800000)
                    addend |= 0xff000000;
                ELF32_1BYTE4 temp = 0;
                temp = (addr - (ELF32_1BYTE4)taddr + (addend << 2)) >> 2;
                *taddr = (*taddr & 0xff000000) | (temp & 0x00ffffff);
                break;
            }
            case R_ARM_REL32:
                *taddr += (ELF32_1BYTE4)addr - (ELF32_1BYTE4)taddr;
                break;
            case R_ARM_V4BX:
                *taddr &= 0xf000000f;
                *taddr |= 0x01a0f000;
                break;
            case R_ARM_GLOB_DAT:
            case R_ARM_JUMP_SLOT:
                *taddr = (ELF32_1BYTE4)addr;
                break;
#if 0
            case R_ARM_GOT_BREL: {           // To do
                ELF32_1BYTE4 temp = 0;
                temp   = (ELF32_1BYTE4)addr;
                *taddr = (ELF32_1BYTE4)&temp;
                break;
            }
#endif
            case R_ARM_RELATIVE:
                *taddr += (ELF32_1BYTE4)addr;
                break;
            case R_ARM_THM_CALL:
            case R_ARM_THM_JUMP24: {
                ELF32_1BYTE4 offset = 0;
                ELF32_1BYTE4 upper, lower;
                ELF32_1BYTE4 sign, j1, j2;
                //
                upper  = *(ELF32_1BYTE2 *)taddr;
                lower  = *(ELF32_1BYTE2 *)((ELF32_1BYTE4)taddr + 2);
                //
                sign   = (upper >> 10) & 1;
                j1     = (lower >> 13) & 1;
                j2     = (lower >> 11) & 1;
                //
                offset |= ((upper & 0x03ff) << 12);
                offset |= ((lower & 0x07ff) << 1);
                offset |= (sign << 24);
                offset |= ((~(j1 ^ sign) & 1) << 23);
                offset |= ((~(j2 ^ sign) & 1) << 22);
                //
                if (offset  & 0x01000000)
                    offset -= 0x02000000;
                //
                offset += addr - (ELF32_1BYTE4)taddr;
                //
                //模块:只允许Thumb地址
                if (!(offset & 1))
                    return;
                if (offset <= (ELF32_1BYTE4)0xff000000)
                    return;
                if (offset >= (ELF32_1BYTE4)0x01000000)
                    return;


                sign = (offset >> 24) & 1;
                j1   = sign ^ (~(offset >> 23) & 1);
                j2   = sign ^ (~(offset >> 22) & 1);
                
                ELF32_1BYTE2 temp0 = 0;
                temp0 |= (ELF32_1BYTE2)((offset >> 12) & 0x03ff);
                temp0 |= (ELF32_1BYTE2)((upper) &  0xf800);
                temp0 |= (ELF32_1BYTE2)((sign)  << 10);
                ELF32_1BYTE2 temp1 = 0;
                temp1 |= (ELF32_1BYTE2)((offset >>  1) & 0x07ff);
                temp1 |= (ELF32_1BYTE2)((lower) &  0xd000);
                temp1 |= (ELF32_1BYTE2)((j1)    << 13);
                temp1 |= (ELF32_1BYTE2)((j2)    << 11);
                //
                *(ELF32_1BYTE2 *)(taddr)     = temp0;
                *(ELF32_1BYTE2 *)(taddr + 2) = temp1;
                //
                upper = *(ELF32_1BYTE2 *)taddr;
                lower = *(ELF32_1BYTE2 *)((ELF32_1BYTE4)taddr + 2);
                break;
            }
            }
        }
    }
}

#endif

