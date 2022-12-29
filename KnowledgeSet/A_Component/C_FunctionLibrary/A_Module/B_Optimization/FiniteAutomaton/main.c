#include <stdio.h>
#include "TestModule.h"

int main(int argc, char *argv[]) {
    printf("------------------------------------------\n");
    FA_Process_Execute(Module, ModuleStatus1, ModuleStatus2);
    printf("------------------------------------------\n");
    FA_Process_Execute(Module, ModuleStatus1, ModuleStatus3);
    printf("------------------------------------------\n");
    FA_Process_Execute(Module, ModuleStatus2, ModuleStatus1);
    printf("------------------------------------------\n");
    FA_Process_Execute(Module, ModuleStatus2, ModuleStatus3);
    printf("------------------------------------------\n");
    FA_Process_Execute(Module, ModuleStatus3, ModuleStatus1);
    printf("------------------------------------------\n");
    FA_Process_Execute(Module, ModuleStatus3, ModuleStatus2);
    printf("------------------------------------------\n");
    return 0;
}
