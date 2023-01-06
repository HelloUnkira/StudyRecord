#ifndef APP_ARCH_ADAPTOR_H
#define APP_ARCH_ADAPTOR_H

/* APP architecture adaptor: */

#define APP_ARCH_IS_PC      1

#if 0
#elif APP_ARCH_IS_PC
#include "app_arch_adaptor_pc.h"
#else
#error "unknown arch"
#endif

#endif
