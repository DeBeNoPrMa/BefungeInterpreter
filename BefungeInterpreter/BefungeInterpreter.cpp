#include "BefungeInterpreter.h"

BefungeInterpreter::BefungeInterpreter()
{
    resetExecution();
}

BefungeInterpreter::~BefungeInterpreter()
{
}

bool BefungeInterpreter::loadFromFile(std::string filename)
{
    std::ifstream myfile(filename);
    std::string line;

    if (myfile.is_open())
    {
        while (std::getline(myfile, line))
        {
            line.resize(80, ' ');
            program.push_back(line);
        }
        myfile.close();

        return true;
    }
    return false;
}

void BefungeInterpreter::resetExecution()
{
    srand(time(NULL));

    visualMode = false;
    stepByStepMode = false;
    stringMode = false;
    endOfProgram = false;

    direction = RIGHT;

    instructionPointer = sf::Vector2u(0, 0);
}

void BefungeInterpreter::run(int flags)
{
    parseFlags(flags);

    while (!endOfProgram)
    {
        parse();

        if (visualMode)
            //draw();

        if (stepByStepMode)
            __noop;
    }

    system("PAUSE");
}

bool BefungeInterpreter::popA()
{
    if (programStack.size() >= 1)
    {
        a = programStack.top();
        programStack.pop();
        return true;
    }
    return false;
}

bool BefungeInterpreter::popAB()
{
    if (programStack.size() >= 2)
    {
        a = programStack.top();
        programStack.pop();
        b = programStack.top();
        programStack.pop();
        return true;
    }
    return false;
}

bool BefungeInterpreter::popABV()
{
    if (programStack.size() >= 3)
    {
        a = programStack.top();
        programStack.pop();
        b = programStack.top();
        programStack.pop();
        v = programStack.top();
        programStack.pop();
        return true;
    }
    return false;
}

void BefungeInterpreter::arithmetic(const char& in)
{
    if (popAB())
    {
        programStack.push(in == '+' ? a + b :
            in == '-' ? b - a :
            in == '*' ? a * b :
            in == '/' ? b / a :
            in == '%' ? b % a :
            in == 'g' ? program[a][b] :
            b > a ? 1 : 0);
    }
}

void BefungeInterpreter::directionOperator(const char& in)
{
    switch (in)
    {
    case '<':
        direction = LEFT;
        break;
    case '>':
        direction = RIGHT;
        break;
    case 'v':
        direction = DOWN;
        break;
    case '^':
        direction = UP;
        break;
    case '?':
        int r = rand() % 4;
        direction = r == 0 ? LEFT : r == 1 ? RIGHT : r == 2 ? UP : DOWN;
    }
}

void BefungeInterpreter::branching(const char& in)
{
    if (popA())
    {
        switch (in)
        {
        case '_':
            direction = a == 0 ? RIGHT : LEFT;
            break;
        case '|':
            direction = a == 0 ? DOWN : UP;
            break;
        }
    }
}

void BefungeInterpreter::output(const char& in)
{
    if (popA())
    {
        switch (in)
        {
        case '.':
            printf("%i", a);
            break;
        case ',':
            printf("%c", a);
            break;
        }
    }
}

void BefungeInterpreter::input(const char& in)
{
    switch (in)
    {
    case '&':
        scanf("%i", &a);
        programStack.push(a);
        break;
    case '~':
        scanf("%c", &a);
        programStack.push(a);
        break;
    }
}

void BefungeInterpreter::parse()
{
    char in = program[instructionPointer.y][instructionPointer.x];

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
            arithmetic(in);
            break;
        case '<':
        case '>':
        case 'v':
        case '^':
        case '?':
            directionOperator(in);
            break;
        case '_':
        case '|':
            branching(in);
            break;
        case '"':
            stringMode = true;
            break;
        case '!':
            if(popA())
                programStack.push(a == 0 ? 1 : 0);
            break;
        case ':':
            if (!programStack.empty())
                programStack.push(programStack.top());
            break;
        case '.':
        case ',':
            output(in);
            break;
        case '$':
            if (!programStack.empty())
                programStack.pop();
            break;
        case '\\':
            if (popAB())
            {
                programStack.push(b);
                programStack.push(a);
            }
            break;
        case '&':
        case '~':
            input(in);
            break;
        case 'g':
            if (popAB())
                programStack.push(program[a][b]);
            break;
        case 'p':
            if (popABV())
                program[a][b] = v;
            break;
        case ' ':
            break;
        case '#':
            updateInstructionPointer();
            break;
        case '@':
            endOfProgram = true;
            break;
        }
    }
    else
    {
        if (in == '"')
            stringMode = false;
        else
            programStack.push(in);
    }

    updateInstructionPointer();
}

void BefungeInterpreter::parseFlags(int flags)
{
    if (flags & FLAG_NONE)
        return;
    if (flags & FLAG_STEPBYSTEP)
        stepByStepMode = true;
    if (flags & FLAG_VISUAL)
        visualMode = true;
}

void BefungeInterpreter::updateInstructionPointer()
{
    instructionPointer.x += direction == RIGHT ? 1 : direction == LEFT ? -1 : 0;
    instructionPointer.y += direction == DOWN ? 1 : direction == UP ? -1 : 0;

    if (instructionPointer.x == 80) instructionPointer.x = 0;
    else if (instructionPointer.x == -1) instructionPointer.x = 80;
    if (instructionPointer.y == program.size()) instructionPointer.y = 0;
    else if (instructionPointer.y == -1) instructionPointer.y = program.size() - 1;
}