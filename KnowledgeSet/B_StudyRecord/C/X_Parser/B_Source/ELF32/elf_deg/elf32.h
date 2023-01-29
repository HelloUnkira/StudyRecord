
#ifndef ELF32_H
#define ELF32_H

/* 平台无关项 */
#define ELF_SECTION_NAME
#ifdef  ELF_SECTION_NAME
/* 段名 */
#define ELF_BSS           ".bss"         /* 未初始化数据段 */
#define ELF_DATA          ".data"        /* 初始化数据段 */
#define ELF_DEBUG         ".debug"       /* 调试段 */
#define ELF_DYNAMIC       ".dynamic"     /* 动态链接信息段 */
#define ELF_DYNSTR        ".dynstr"      /* 动态字符串表段 */
#define ELF_DYNSYM        ".dynsym"      /* 动态符号表段 */
#define ELF_FINI          ".fini"        /* 终止代码段 */
#define ELF_GOT           ".got"         /* 全局偏移量表段 */
#define ELF_HASH          ".hash"        /* 符号哈希表段 */
#define ELF_INIT          ".init"        /* 初始化代码段 */
#define ELF_REL_DATA      ".rel.data"    /* 可重定位数据段 */
#define ELF_REL_FINI      ".rel.fini"    /* 可重定位终止代码段 */
#define ELF_REL_INIT      ".rel.init"    /* 可重定位初始化代码段 */
#define ELF_REL_DYN       ".rel.dyn"     /* 可重定位动态链接信息段 */
#define ELF_REL_RODATA    ".rel.rodata"  /* 可重定位只读数据段 */
#define ELF_REL_TEXT      ".rel.text"    /* 可重定位终止代码段 */
#define ELF_RODATA        ".rodata"      /* 只读数据段 */
#define ELF_SHSTRTAB      ".shstrtab"    /* 段头字符串表段 */
#define ELF_STRTAB        ".strtab"      /* 字符串表段 */
#define ELF_SYMTAB        ".symtab"      /* 符号表段 */
#define ELF_TEXT          ".text"        /* 代码段 */
#endif

#define ELF_SECTION_TYPE
#ifdef  ELF_SECTION_TYPE
/* e_ident[]文件类型 */
#define ELFCLASSNONE    0     /* 非法类型 */
#define ELFCLASS32      1     /* 32位类型 */
#define ELFCLASS64      2     /* 64位类型 */
#define ELFCLASSNUM     3     /* 类数量 */
/* e_ident[]数据编码 */
#define ELFDATANONE     0     /* 非法类型 */
#define ELFDATA2LSB     1     /* 小端序列 */
#define ELFDATA2MSB     2     /* 大端序列 */
#define ELFDATANUM      3     /* 数据编码定义数量 */
/* 绑定字段 */
#define STB_LOCAL       0     /* 本地标号 */
#define STB_GLOBAL      1     /* 全局标号 */
#define STB_WEAK        2     /* 可覆盖标号 */
#define STB_NUM         3
#define STB_LOPROC      13    /* 处理器 - 特定的范围 */
#define STB_HIPROC      15    /* 处理器 - 特定的范围 */
/*  */
#define STT_NOTYPE      0     /* 符号类型未指定 */
#define STT_OBJECT      1     /* 数据对象 */
#define STT_FUNC        2     /* 代码对象 */
#define STT_SECTION     3     /* 符号标识一个ELF段 */
#define STT_FILE        4     /* 符号的名称是文件名 */
#define STT_COMMON      5     /* 公共的数据对象 */
#define STT_TLS         6     /* 线程局部数据对象 */
#define STT_NUM         7     /* #泛型范围内定义的类型 */
#define STT_LOOS        10    /* 操作系统特定的范围 */
#define STT_HIOS        12    /* 操作系统特定的范围 */
#define STT_LOPROC      13    /* 处理器 - 特定的范围 */
#define STT_HIPROC      15    /* 处理器 - 特定的范围 */
/*  */
#define STN_UNDEF       0     /* 未定义 */
/* p_type */
#define PT_NULL         0             /* 没有被使用 */
#define PT_LOAD         1             /* 可装载的内存段 */
#define PT_DYNAMIC      2             /* 该段描述的是动态链接信息 */
#define PT_INTERP       3
#define PT_NOTE         4
#define PT_SHLIB        5
#define PT_PHDR         6
#define PT_TLS          7
#define PT_NUM          8
#define PT_LOOS         0x60000000    /* 操作系统特定的范围 */
#define PT_HIOS         0x6fffffff    /* 操作系统特定的范围 */
#define PT_LOPROC       0x70000000    /* 处理器 - 特定的范围 */
#define PT_HIPROC       0x7fffffff    /* 处理器 - 特定的范围 */
/* p_flags */
#define PF_X            1
#define PF_W            2
#define PF_R            4
/* sh_type */
#define SHT_NULL        0             /* 闲置段 */
#define SHT_PROGBITS    1             /* 程序定义信息段 */
#define SHT_SYMTAB      2             /* 符号表段 */
#define SHT_STRTAB      3             /* 字符串表段 */
#define SHT_RELA        4             /* 带有追加的可重定义段 */
#define SHT_HASH        5             /* 符号哈希表段 */
#define SHT_DYNAMIC     6             /* 动态段 */
#define SHT_NOTE        7             /* 记录段 */
#define SHT_NOBITS      8             /* 无空间段 */
#define SHT_REL         9             /* 不带追加的可重定义段 */
#define SHT_SHLIB       10            /* 保留-用途未知 */
#define SHT_DYNSYM      11            /* 动态符号表段 */
#define SHT_NUM         12            /* 段类型数量 */
#define SHT_LOPROC      0x70000000    /* 为处理器保留范围 */
#define SHT_HIPROC      0x7fffffff    /* 特定段头类型 */
#define SHT_LOUSER      0x80000000    /* 为应用保留范围 */
#define SHT_HIUSER      0xffffffff    /* 特殊索引 */
/* sh_flags */
#define SHF_WRITE       0x1           /* 可写 */
#define SHF_ALLOC       0x2           /* 占用内存 */
#define SHF_EXECINSTR   0x4           /* 可执行 */
#define SHF_MASKPROC    0xf0000000    /* 为处理器保留位 */
/*  */
#define ET_NONE         0              /* 无文件类型 */
#define ET_REL          1              /* 可重定位文件类型 */
#define ET_EXEC         2              /* 可执行文件类型 */
#define ET_DYN          3              /* 共享对象类型 */
#define ET_CORE         4              /* 核心文件 */
#endif

#define ELF_SECTION_PORT
#ifdef  ELF_SECTION_PORT
/* ARM可重定位类型 */
#define R_ARM_NONE          0
#define R_ARM_PC24          1
#define R_ARM_ABS32         2
#define R_ARM_REL32         3
#define R_ARM_THM_CALL      10
#define R_ARM_GLOB_DAT      21
#define R_ARM_JUMP_SLOT     22
#define R_ARM_RELATIVE      23
#define R_ARM_GOT_BREL      26
#define R_ARM_PLT32         27
#define R_ARM_CALL          28
#define R_ARM_JUMP24        29
#define R_ARM_THM_JUMP24    30
#define R_ARM_V4BX          40
/* X86可重定位类型 */
#define R_386_NONE          0
#define R_386_32            1
#define R_386_PC32          2
#define R_386_GOT32         3
#define R_386_PLT32         4
#define R_386_COPY          5
#define R_386_GLOB_DAT      6
#define R_386_JMP_SLOT      7
#define R_386_RELATIVE      8
#define R_386_GOTOFF        9
#define R_386_GOTPC         10
#endif

#define ELF_FILE_HEAD
#ifdef  ELF_FILE_HEAD
//设定的e_ident[]魔数
#define ELFMAG0         0x7f           /* e_ident[EI_MAG0] */
#define ELFMAG1         'E'            /* e_ident[EI_MAG1] */
#define ELFMAG2         'L'            /* e_ident[EI_MAG2] */
#define ELFMAG3         'F'            /* e_ident[EI_MAG3] */
#define SELFMAG         4              /* 魔数大小 */
#define ELFMAG          "\177ELF"      /* 魔数:ELF */
/*  */
#define EI_MAG0         0
#define EI_MAG1         1
#define EI_MAG2         2
#define EI_MAG3         3
#define EI_CLASS        4              /* 文件类型 */
#define EI_NIDENT       16             /* e_ident[]占用空间 */
#endif

//平台依赖:此处是32位平台,
//对应各个段头字节长度是带有依赖的
#define ELF32_PORT
#ifdef  ELF32_PORT
#define ELF32_0BYTE1   signed char
#define ELF32_0BYTE2   signed short
#define ELF32_0BYTE4   signed int
#define ELF32_1BYTE1 unsigned char
#define ELF32_1BYTE2 unsigned short
#define ELF32_1BYTE4 unsigned int
#endif

#define ELF32_HEADER
#ifdef  ELF32_HEADER
/* ELF头部 */
typedef struct elfhdr {
    ELF32_1BYTE1 e_ident[EI_NIDENT];    /* ELF识别 */
    ELF32_1BYTE2 e_type;                /* 对象文件类型 */
    ELF32_1BYTE2 e_machine;             /* 机器 */
    ELF32_1BYTE4 e_version;             /* 对象文件版本 */
    ELF32_1BYTE4 e_entry;               /* 虚拟的入口点 */
    ELF32_1BYTE4 e_phoff;               /* 程序头表偏移量 */
    ELF32_1BYTE4 e_shoff;               /* 段头表偏移量 */
    ELF32_1BYTE4 e_flags;               /* 处理器 - 特定的标志 */
    ELF32_1BYTE2 e_ehsize;              /* ELF头尺寸 */
    ELF32_1BYTE2 e_phentsize;           /* 程序头条目大小 */
    ELF32_1BYTE2 e_phnum;               /* 程序头条目数量 */
    ELF32_1BYTE2 e_shentsize;           /* 段头条目大小 */
    ELF32_1BYTE2 e_shnum;               /* 段头条目数量 */
    ELF32_1BYTE2 e_shstrndx;            /* 段头表的 段头字符表 条目偏移量 */
} Elf32_Ehdr;

/* ELF的段头部 */
typedef struct elf32_shdr {
    ELF32_1BYTE4 sh_name;         /* 名字 - 索引到段头 字符串表段 */
    ELF32_1BYTE4 sh_type;         /* 类型 */
    ELF32_1BYTE4 sh_flags;        /* 标志 */
    ELF32_1BYTE4 sh_addr;         /* 地址 */
    ELF32_1BYTE4 sh_offset;       /* 文件偏移量 */
    ELF32_1BYTE4 sh_size;         /* 段尺寸 */
    ELF32_1BYTE4 sh_link;         /* 段头表索引链接 */
    ELF32_1BYTE4 sh_info;         /* 附加信息 */
    ELF32_1BYTE4 sh_addralign;    /* 地址对齐 */
    ELF32_1BYTE4 sh_entsize;      /* 段条目大小 */
} Elf32_Shdr;

/* 程序头部 */
typedef struct elf32_phdr {
    ELF32_1BYTE4 p_type;      /* 分段类型 */
    ELF32_1BYTE4 p_offset;    /* 分段偏移量 */
    ELF32_1BYTE4 p_vaddr;     /* 段虚拟地址 */
    ELF32_1BYTE4 p_paddr;     /* 物理地址-忽略? */
    ELF32_1BYTE4 p_filesz;    /* 文件中seg.的字节数 */
    ELF32_1BYTE4 p_memsz;     /* mem.中seg.的字节数 */
    ELF32_1BYTE4 p_flags;     /* 标记 */
    ELF32_1BYTE4 p_align;     /* 内存地址对齐 */
} Elf32_Phdr;

/* 符号表项 */
typedef struct elf32_sym {
    ELF32_1BYTE4 st_name;     /* 名字 - 索引到字符串表 */
    ELF32_1BYTE4 st_value;    /* 符号值 */
    ELF32_1BYTE4 st_size;     /* 符号尺寸 */
    ELF32_1BYTE1 st_info;     /* 类型和绑定 */
    ELF32_1BYTE1 st_other;    /* 0 - 无定义 */
    ELF32_1BYTE2 st_shndx;    /* 段头索引 */
} Elf32_Sym;

/* 带有隐式追加的重定位项 */
typedef struct elf32_rel {
    ELF32_1BYTE4 r_offset;    /* 可重定位的偏移量 */
    ELF32_1BYTE4 r_info;      /* 符号表索引和类型 */
} Elf32_Rel;

/* 带有显式追加的重定位项 */
typedef struct elf32_rela {
    ELF32_1BYTE4 r_offset;    /* 可重定位的偏移量 */
    ELF32_1BYTE4 r_info;      /* 符号表索引和类型 */
    ELF32_0BYTE4 r_addend;    /* 带符号的追加 */
} Elf32_Rela;
#endif

#define ELF32_HELP
#ifdef  ELF32_HELP
/* e_ident检查 */
#define IS_ELF(ehdr)    ((ehdr).e_ident[EI_MAG0] == ELFMAG0 && \
                         (ehdr).e_ident[EI_MAG1] == ELFMAG1 && \
                         (ehdr).e_ident[EI_MAG2] == ELFMAG2 && \
                         (ehdr).e_ident[EI_MAG3] == ELFMAG3)

#define ELF_ST_BIND(info)       ((info) >> 4)
#define ELF_ST_TYPE(info)       ((info) & 0xf)
#define ELF_ST_INFO(bind, type) (((bind) << 4) + ((type) & 0xf))
/* 提取重定位信息 - r_info */
#define ELF32_R_SYM(i)          ((ELF32_1BYTE1)((i) >> 8))
#define ELF32_R_TYPE(i)         ((ELF32_1BYTE1)((i)))
#define ELF32_R_INFO(s,t)       ((ELF32_1BYTE1)((t)) + ((s) << 8))
/* 段特定属性 */
#define IS_PROG(s)        (s.sh_type == SHT_PROGBITS)
#define IS_NOPROG(s)      (s.sh_type == SHT_NOBITS)
#define IS_REL(s)         (s.sh_type == SHT_REL)
#define IS_RELA(s)        (s.sh_type == SHT_RELA)
#define IS_ALLOC(s)       (s.sh_flags == SHF_ALLOC)
#define IS_AX(s)          ((s.sh_flags & SHF_ALLOC) && (s.sh_flags & SHF_EXECINSTR))
#define IS_AW(s)          ((s.sh_flags & SHF_ALLOC) && (s.sh_flags & SHF_WRITE))

/* 获取头表 */
#define ELF32_EHDR(pointer)       ((Elf32_Ehdr *)pointer)
#define ELF32_SHDR(pointer, ehdr) ((Elf32_Shdr *)(pointer + ehdr->e_shoff))
#define ELF32_PHDR(pointer, ehdr) ((Elf32_Shdr *)(pointer + ehdr->e_phoff))

/* 重要Abbr(abbreviation): */
/* ehdr:---e:ELF,     hdr:header */
/* shdr:---s:Section, hdr:header */
/* phdr:---p:Program, hdr:header */
/* sidx:---s:Section, idx:index */
/* pidx:---p:Program, idx:index */
#endif

#define ELF32_EXTERN_SYMTAB
#ifdef  ELF32_EXTERN_SYMTAB

typedef struct elf32_ksym {
    ELF32_1BYTE4 addr;  //修改,32位地址
    ELF32_1BYTE4 name;  //修改,它是.rodata.name中的偏移量
} Elf32_KSym;

//从内核导出的符号表会被保存至此SECTION
//注意平台依赖,这里是32位,并且统一,符号表段名字
#define ELF32_SYMTAB_NAME       ".rodata.name"
#define ELF32_SYMTAB_SYM        ".elf32symtab"

#define KSYM_EXPORT(symbol)                                                     \
const ELF32_0BYTE1 ksym_##symbol##_name[] SECTION(ELF32_SYMTAB_NAME) = #symbol; \
const Elf32_KSym   ksym_##symbol          SECTION(ELF32_SYMTAB_SYM)  =          \
{                                                                               \
    (ELF32_1BYTE4)(&symbol),                                                    \
    (ELF32_1BYTE4)(ksym_##symbol##_name)                                        \
};

typedef struct elf32_ksymtab {
    ELF32_1BYTE4 ksymidx;
    Elf32_KSym  *ksym;
} Elf32_KSymtab;

#endif

#endif

