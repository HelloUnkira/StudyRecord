#include <stdio.h>
#include "TestModule.h"

FA_Process_Define(Module, ModuleStatus1, ModuleStatus2)
{
    printf("Process 1 -> 2\n");
}

FA_Process_Define(Module, ModuleStatus2, ModuleStatus3)
{
    printf("Process 2 -> 3\n");
}

FA_Process_Define(Module, ModuleStatus3, ModuleStatus1)
{
    printf("Process 3 -> 1\n");
}

FA_Process_Define(Module, ModuleStatus1, ModuleStatus3)
{
    FA_Process_Execute(Module, ModuleStatus1, ModuleStatus2);
    FA_Process_Execute(Module, ModuleStatus2, ModuleStatus3);
}

FA_Process_Define(Module, ModuleStatus2, ModuleStatus1)
{
    FA_Process_Execute(Module, ModuleStatus2, ModuleStatus3);
    FA_Process_Execute(Module, ModuleStatus3, ModuleStatus1);
}

FA_Process_Define(Module, ModuleStatus3, ModuleStatus2)
{
    FA_Process_Execute(Module, ModuleStatus3, ModuleStatus1);
    FA_Process_Execute(Module, ModuleStatus1, ModuleStatus2);
}
