#include <vector>
#include <stack>
#include <stdio.h>
#include <Windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <time.h>

#define POP programStack.top(); programStack.pop()

typedef enum DIRECTION {
    UP,
    DOWN, 
    RIGHT,
    LEFT
} direction;

struct Index {
    int x;
    int y;
};

int main()
{
    srand(time(NULL));

    std::vector<std::string> program;
    std::stack<int> programStack;
    Index executionPointer;

    std::ifstream myfile("program.bfg");
    std::string line;

    if (myfile.is_open())
    {
        while (std::getline(myfile, line))
        {
            line.resize(80, ' ');
            program.push_back(line);
        }
        myfile.close();
    }
    
    direction executionDir  = RIGHT;
    bool endOfProgram       = false;
    bool stringMode         = false;
    executionPointer.x      = 0;
    executionPointer.y      = 0;

    int a, b, x, y, v;

    while (!endOfProgram)
    {
        char in = program[executionPointer.y][executionPointer.x];

        if (!stringMode)
        {
            switch (in)
            {
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                programStack.push(in - '0');
                break;
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '`':
                a = POP;
                b = POP;
                programStack.push(in == '+' ? a + b : in == '-' ? b - a : in == '*' ? a * b : in == '/' ? b / a : in == '%' ? b % a : b > a ? 1 : 0);
                break;
            case '!':
                a = POP;
                programStack.push(a == 0 ? 1 : 0);
                break;
            case '>':
                executionDir = RIGHT;
                break;
            case '<':
                executionDir = LEFT;
                break;
            case '^':
                executionDir = UP;
                break;
            case 'v':
                executionDir = DOWN;
                break;
            case '?':
                int r = rand() % 4;
                executionDir = r == 0 ? RIGHT : r == 1 ? UP : r == 2 ? DOWN : LEFT;
                break;
            case '_':
                a = POP;
                executionDir = a == 0 ? RIGHT : LEFT;
                break;
            case '|':
                a = POP;
                executionDir = a == 0 ? DOWN : UP;
                break;
            case '"':
                stringMode = true;
                break;
            case ':':
                programStack.push(programStack.top());
                break;
            case '\\':
                a = POP;
                b = POP;
                programStack.push(a);
                programStack.push(b);
                break;
            case '$':
                programStack.pop();
                break;
            case '.':
                a = POP;
                printf("%i", a);
                break;
            case ',':
                a = POP;
                printf("%c", a);
                break;
            case '#':
                executionPointer.x += executionDir == RIGHT ? 1 : executionDir == LEFT ? -1 : 0;
                executionPointer.y += executionDir == DOWN ? 1 : executionDir == UP ? -1 : 0;

                if (executionPointer.y < 0) executionPointer.x = program.size() - 1;
                else if (executionPointer.y == program.size()) executionPointer.x = 0;
                if (executionPointer.x < 0) executionPointer.x = program[executionPointer.y].size() - 1;
                else if (executionPointer.x == program[executionPointer.y].size()) executionPointer.x = 0;
                break;
            case 'p':
                y = POP;
                x = POP;
                v = POP;
                program[x][y] = (char)v;
                break;
            case 'g':
                y = POP;
                x = POP;
                printf("%c", program[x][y]);
                break;
            case '&':
                scanf("%i", &a);
                programStack.push(a);
                break;
            case '~':
                scanf("%c", &a);
                programStack.push(a);
                break;
            case '@':
                endOfProgram = true;
            }
        }
        else
        {
            int a = program[executionPointer.y][executionPointer.x];

            if (a == '"')
                stringMode = false;
            else
                programStack.push(a);
        }
        
        executionPointer.x += executionDir == RIGHT ? 1 : executionDir == LEFT ? -1 : 0;
        executionPointer.y += executionDir == DOWN ? 1 : executionDir == UP ? -1 : 0;

        if (executionPointer.y < 0) executionPointer.y = program.size() - 1;
        else if (executionPointer.y == program.size()) executionPointer.y = 0;
        if (executionPointer.x < 0) executionPointer.x = program[executionPointer.y].size() - 1;
        else if (executionPointer.x == program[executionPointer.y].size()) executionPointer.x = 0;
    }

    printf("\n");
    system("PAUSE");
}