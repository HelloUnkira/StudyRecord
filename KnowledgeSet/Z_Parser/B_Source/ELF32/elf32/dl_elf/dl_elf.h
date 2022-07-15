
#ifndef DL_ELF_H
#define DL_ELF_H

typedef uint8_t  Elf_Byte;      //无符号字节
typedef uint32_t Elf32_Addr;    //无符号程序地址
typedef uint32_t Elf32_Off;     //无符号文件偏移量
typedef int32_t  Elf32_Sword;   //有符号大整数
typedef uint32_t Elf32_Word;    //无符号大整数
typedef uint16_t Elf32_Half;    //无符号中等整数

//设定的e_ident[]魔数
#define ELFMAG0                 0x7f           /* e_ident[EI_MAG0] */
#define ELFMAG1                 'E'            /* e_ident[EI_MAG1] */
#define ELFMAG2                 'L'            /* e_ident[EI_MAG2] */
#define ELFMAG3                 'F'            /* e_ident[EI_MAG3] */
#define SELFMAG                 4              //魔数大小
#define ELFMAG                  "\177ELF"      //魔数:ELF
#define RTMMAG                  "\177RTM"      //魔数:RTM

#define EI_CLASS                4              //文件类型
#define EI_NIDENT               16             //e_ident[]占用空间

//e_ident[]文件类型
#define ELFCLASSNONE            0              //非法类型
#define ELFCLASS32              1              //32位类型
#define ELFCLASS64              2              //64位类型
#define ELFCLASSNUM             3              //类数量

//e_ident[]数据编码
#define ELFDATANONE             0              //非法类型
#define ELFDATA2LSB             1              //小端序列
#define ELFDATA2MSB             2              //大端序列
#define ELFDATANUM              3              //数据编码定义数量

//e_ident检查:
#define IS_ELF(ehdr)            ((ehdr).e_ident[EI_MAG0] == ELFMAG0 && \
                                 (ehdr).e_ident[EI_MAG1] == ELFMAG1 && \
                                 (ehdr).e_ident[EI_MAG2] == ELFMAG2 && \
                                 (ehdr).e_ident[EI_MAG3] == ELFMAG3)

#define ET_NONE                 0              //无文件类型
#define ET_REL                  1              //可重定位文件类型
#define ET_EXEC                 2              //可执行文件类型
#define ET_DYN                  3              //共享对象类型
#define ET_CORE                 4              //核心文件

//ELF头部
typedef struct elfhdr {
    Elf_Byte      e_ident[EI_NIDENT];          //ELF识别
    Elf32_Half    e_type;                      //对象文件类型
    Elf32_Half    e_machine;                   //机器
    Elf32_Word    e_version;                   //对象文件版本
    Elf32_Addr    e_entry;                     //虚拟的入口点
    Elf32_Off     e_phoff;                     //程序头表偏移量
    Elf32_Off     e_shoff;                     //段头表偏移量
    Elf32_Word    e_flags;                     //处理器 - 特定的标志
    Elf32_Half    e_ehsize;                    //ELF头尺寸
    Elf32_Half    e_phentsize;                 //程序头条目大小
    Elf32_Half    e_phnum;                     //程序头条目数量
    Elf32_Half    e_shentsize;                 //段头条目大小
    Elf32_Half    e_shnum;                     //段头条目数量
    Elf32_Half    e_shstrndx;                  //段头表的 段头字符表 条目偏移量
} Elf32_Ehdr;

//ELF的段头部
typedef struct {
    Elf32_Word sh_name;                        //名字 - 索引到段头 字符串表段
    Elf32_Word sh_type;                        //类型
    Elf32_Word sh_flags;                       //标志
    Elf32_Addr sh_addr;                        //地址
    Elf32_Off  sh_offset;                      //文件偏移量
    Elf32_Word sh_size;                        //段尺寸
    Elf32_Word sh_link;                        //段头表索引链接
    Elf32_Word sh_info;                        //附加信息
    Elf32_Word sh_addralign;                   //地址对齐
    Elf32_Word sh_entsize;                     //段条目大小
} Elf32_Shdr;

//段名
#define ELF_BSS                 ".bss"         //未初始化数据段
#define ELF_DATA                ".data"        //初始化数据段
#define ELF_DEBUG               ".debug"       //调试段
#define ELF_DYNAMIC             ".dynamic"     //动态链接信息段
#define ELF_DYNSTR              ".dynstr"      //动态字符串表段
#define ELF_DYNSYM              ".dynsym"      //动态符号表段
#define ELF_FINI                ".fini"        //终止代码段
#define ELF_GOT                 ".got"         //全局偏移量表段
#define ELF_HASH                ".hash"        //符号哈希表段
#define ELF_INIT                ".init"        //初始化代码段
#define ELF_REL_DATA            ".rel.data"    //可重定位数据段
#define ELF_REL_FINI            ".rel.fini"    //可重定位终止代码段
#define ELF_REL_INIT            ".rel.init"    //可重定位初始化代码段
#define ELF_REL_DYN             ".rel.dyn"     //可重定位动态链接信息段
#define ELF_REL_RODATA          ".rel.rodata"  //可重定位只读数据段
#define ELF_REL_TEXT            ".rel.text"    //可重定位终止代码段
#define ELF_RODATA              ".rodata"      //只读数据段
#define ELF_SHSTRTAB            ".shstrtab"    //段头字符串表段
#define ELF_STRTAB              ".strtab"      //字符串表段
#define ELF_SYMTAB              ".symtab"      //符号表段
#define ELF_TEXT                ".text"        //代码段
#define ELF_RTMSYMTAB           "RTMSymTab"

//符号表项
typedef struct elf32_sym {
    Elf32_Word    st_name;                     //名字 - 索引到字符串表
    Elf32_Addr    st_value;                    //符号值
    Elf32_Word    st_size;                     //符号尺寸
    Elf_Byte      st_info;                     //类型和绑定
    Elf_Byte      st_other;                    //0 - 无定义
    Elf32_Half    st_shndx;                    //段头索引
} Elf32_Sym;

#define STB_LOCAL               0              //捆绑
#define STB_GLOBAL              1
#define STB_WEAK                2
#define STB_NUM                 3

#define STB_LOPROC              13             //处理器 - 特定的范围
#define STB_HIPROC              15

#define STT_NOTYPE              0              //符号类型未指定
#define STT_OBJECT              1              //数据对象
#define STT_FUNC                2              //代码对象
#define STT_SECTION             3              //符号标识一个ELF段
#define STT_FILE                4              //符号的名称是文件名
#define STT_COMMON              5              //公共的数据对象
#define STT_TLS                 6              //线程局部数据对象
#define STT_NUM                 7              //#泛型范围内定义的类型
#define STT_LOOS                10             //操作系统特定的范围
#define STT_HIOS                12
#define STT_LOPROC              13             //处理器 - 特定的范围
#define STT_HIPROC              15

#define STN_UNDEF               0              //未定义

#define ELF_ST_BIND(info)       ((info) >> 4)
#define ELF_ST_TYPE(info)       ((info) & 0xf)
#define ELF_ST_INFO(bind, type) (((bind)<<4)+((type)&0xf))

//带有隐式追加的重定位项
typedef struct {
    Elf32_Addr r_offset;                       //可重定位的偏移量
    Elf32_Word r_info;                         //符号表索引和类型
} Elf32_Rel;

//带有显式追加的重定位项
typedef struct {
    Elf32_Addr  r_offset;                      //可重定位的偏移量
    Elf32_Word  r_info;                        //符号表索引和类型
    Elf32_Sword r_addend;
} Elf32_Rela;

//提取重定位信息 - r_info
#define ELF32_R_SYM(i)          ((i) >> 8)
#define ELF32_R_TYPE(i)         ((unsigned char) (i))
#define ELF32_R_INFO(s,t)       (((s) << 8) + (unsigned char)(t))

//ARM可重定位类型
#define R_ARM_NONE              0
#define R_ARM_PC24              1
#define R_ARM_ABS32             2
#define R_ARM_REL32             3
#define R_ARM_THM_CALL          10
#define R_ARM_GLOB_DAT          21
#define R_ARM_JUMP_SLOT         22
#define R_ARM_RELATIVE          23
#define R_ARM_GOT_BREL          26
#define R_ARM_PLT32             27
#define R_ARM_CALL              28
#define R_ARM_JUMP24            29
#define R_ARM_THM_JUMP24        30
#define R_ARM_V4BX              40

//X86可重定位类型
#define R_386_NONE              0
#define R_386_32                1
#define R_386_PC32              2
#define R_386_GOT32             3
#define R_386_PLT32             4
#define R_386_COPY              5
#define R_386_GLOB_DAT          6
#define R_386_JMP_SLOT          7
#define R_386_RELATIVE          8
#define R_386_GOTOFF            9
#define R_386_GOTPC             10

//程序头部
typedef struct {
    Elf32_Word p_type;                         //分段类型
    Elf32_Off  p_offset;                       //分段偏移量
    Elf32_Addr p_vaddr;                        //段虚拟地址
    Elf32_Addr p_paddr;                        //物理地址-忽略?
    Elf32_Word p_filesz;                       //文件中seg.的字节数
    Elf32_Word p_memsz;                        //mem.中seg.的字节数
    Elf32_Word p_flags;                        //标记
    Elf32_Word p_align;                        //内存地址对齐
} Elf32_Phdr;

//p_type
#define PT_NULL                 0
#define PT_LOAD                 1
#define PT_DYNAMIC              2
#define PT_INTERP               3
#define PT_NOTE                 4
#define PT_SHLIB                5
#define PT_PHDR                 6
#define PT_TLS                  7
#define PT_NUM                  8
#define PT_LOOS                 0x60000000
#define PT_HIOS                 0x6fffffff
#define PT_LOPROC               0x70000000
#define PT_HIPROC               0x7fffffff

//p_flags
#define PF_X                    1
#define PF_W                    2
#define PF_R                    4

//sh_type
#define SHT_NULL                0              //闲置段
#define SHT_PROGBITS            1              //程序定义信息段
#define SHT_SYMTAB              2              //符号表段
#define SHT_STRTAB              3              //字符串表段
#define SHT_RELA                4              //带有追加的可重定义段
#define SHT_HASH                5              //符号哈希表段
#define SHT_DYNAMIC             6              //动态段
#define SHT_NOTE                7              //记录段
#define SHT_NOBITS              8              //无空间段
#define SHT_REL                 9              //不带追加的可重定义段
#define SHT_SHLIB               10             //保留-用途未知
#define SHT_DYNSYM              11             //动态符号表段
#define SHT_NUM                 12             //段类型数量
#define SHT_LOPROC              0x70000000     //为处理器保留范围
#define SHT_HIPROC              0x7fffffff     //特定段头类型
#define SHT_LOUSER              0x80000000     //为应用保留范围
#define SHT_HIUSER              0xffffffff     //特殊索引

//段属性标记sh_flags
#define SHF_WRITE               0x1            //可写
#define SHF_ALLOC               0x2            //占用内存
#define SHF_EXECINSTR           0x4            //可执行
#define SHF_MASKPROC            0xf0000000     //为处理器保留位
//段特定属性

#define IS_PROG(s)        (s.sh_type == SHT_PROGBITS)
#define IS_NOPROG(s)      (s.sh_type == SHT_NOBITS)
#define IS_REL(s)         (s.sh_type == SHT_REL)
#define IS_RELA(s)        (s.sh_type == SHT_RELA)
#define IS_ALLOC(s)       (s.sh_flags == SHF_ALLOC)
#define IS_AX(s)          ((s.sh_flags & SHF_ALLOC) && (s.sh_flags & SHF_EXECINSTR))
#define IS_AW(s)          ((s.sh_flags & SHF_ALLOC) && (s.sh_flags & SHF_WRITE))

#define elf_module        ((Elf32_Ehdr *)module_ptr)
#define shdr              ((Elf32_Shdr *)((DL_BYTE1 *)module_ptr + elf_module->e_shoff))
#define phdr              ((Elf32_Phdr *)((DL_BYTE1 *)module_ptr + elf_module->e_phoff))

//加载共享对象
DL_CHAR4 dlmod_load_shared_object(dlmod_t *module, void *module_ptr);
//加载可重定向对象
DL_CHAR4 dlmod_load_relocated_object(dlmod_t *module, void *module_ptr);
//重定向
DL_CHAR4 dlmod_relocate(dlmod_t *module, Elf32_Rel *rel, Elf32_Addr sym_val);

#endif
