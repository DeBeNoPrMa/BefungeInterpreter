#include <vector>
#include <stack>
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#include "BefungeInterpreter.h"

struct Index {
    int x;
    int y;
};

int main()
{
    BefungeInterpreter interpreter;

    if(interpreter.loadFromFile("program.bfg"))
        interpreter.run(FLAG_STEPBYSTEP | FLAG_VISUAL);
}