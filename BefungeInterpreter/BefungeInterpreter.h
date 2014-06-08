#ifndef __BEFUNGEINTERPRETER__
#define __BEFUNGEINTERPRETER__

#include <string>
#include <fstream>
#include <iostream>
#include <stack>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

typedef enum DIRECTION {
    UP,
    DOWN,
    RIGHT,
    LEFT
} Direction;

typedef enum FLAGS {
    FLAG_NONE       = 0x01,
    FLAG_STEPBYSTEP = 0x02,
    FLAG_VISUAL     = 0x04
} Flags;

class BefungeInterpreter
{
private:
    bool visualMode;
    bool stepByStepMode;
    bool stringMode;
    bool endOfProgram;

    std::vector<std::string>    program;
    std::stack<__int8>          programStack;

    sf::Vector2u instructionPointer;

    Direction direction;

    void parseFlags(int flags);
    void resetExecution();
    void parse();
    void draw();

    bool popA();
    bool popAB();
    bool popABV();

    void arithmetic(const char& in);
    void directionOperator(const char& in);
    void branching(const char& in);
    void output(const char& in);
    void input(const char& in);

    void updateInstructionPointer();

    int a;
    int b;

    int v;
public:
    BefungeInterpreter();
    ~BefungeInterpreter();

    bool loadFromFile(std::string filename);
    void run(int flags);
};

#endif