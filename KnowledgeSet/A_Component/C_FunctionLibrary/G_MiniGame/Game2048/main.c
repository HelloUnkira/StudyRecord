#include <stdio.h>
#include <stdlib.h>

#include "Console.h"

uint32_t Game2048Random(uint32_t Number)
{
    return rand() % Number;
}

#define Length  4

int main(int argc, char *argv[])
{
    uint8_t Matrix[Length * Length] = {0};
    uint8_t Buffer[Length] = {0};
    
    Game2048 Instance = {0};
    Instance.Matrix = Matrix;
    Instance.Scale  = Length;
    Instance.Buffer = Buffer;
    Instance.Random = Game2048Random;  
    
    ConsoleExecuteGame2048(&Instance, 2);
    return 0;
}

