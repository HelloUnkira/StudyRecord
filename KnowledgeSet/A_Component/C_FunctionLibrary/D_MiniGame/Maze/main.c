#include <stdio.h>
#include <stdlib.h>

#include "Console.h"

#define MazeWidth      30
#define MazeHeight     20


uint32_t InstanceRandom(uint32_t Number)
{
    return rand() % Number;
}

int main(int argc, char *argv[])
{
    MazeBlock Matrix[MazeHeight][MazeWidth] = {0};
    
    Maze Instance = {0};
    Instance.Matrix   = Matrix;
    Instance.Elements = MazeWidth;
    Instance.Lines    = MazeHeight;
    Instance.Random   = InstanceRandom;
    Instance.Allocate = malloc;
    Instance.Free     = free;
    
    // printf("\n-----------------------\n");
    // for (uint32_t Y = 0; Y < MazeHeight; Y++) {
    // for (uint32_t X = 0; X < MazeWidth; X++)
        // printf("%x ", Matrix[Y][X].H_Status);
        // printf("\n");
    // }
    // printf("\n-----------------------\n");
    // for (uint32_t Y = 0; Y < MazeHeight; Y++) {
    // for (uint32_t X = 0; X < MazeWidth; X++)
        // printf("%x ", Matrix[Y][X].V_Status);
        // printf("\n");
    // }
    // printf("\n-----------------------\n");
    // printf("All_Count:%d", MazeHeight * MazeWidth);
    // printf("\n-----------------------\n");
    // uint32_t H_Count = 0;
    // for (uint32_t Y = 0; Y < MazeHeight; Y++)
    // for (uint32_t X = 0; X < MazeWidth; X++)
        // if (Matrix[Y][X].H_Status)
            // H_Count++;
        // printf("H_Count:%d", H_Count);
    // printf("\n-----------------------\n");
    // uint32_t V_Count = 0;
    // for (uint32_t Y = 0; Y < MazeHeight; Y++)
    // for (uint32_t X = 0; X < MazeWidth; X++)
        // if (Matrix[Y][X].V_Status)
            // V_Count++;
        // printf("V_Count:%d", V_Count);
    // printf("\n-----------------------\n");
    
    ConsoleExecuteMaze(&Instance);
    return 0;
}
