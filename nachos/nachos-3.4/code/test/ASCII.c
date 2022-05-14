#include "syscall.h"

void printASCII()
{
    int i;
    for (i=32; i < 127; i++)
    {
	PrintString("||\t ");PrintChar(i);PrintString(" \t|\t ");
	PrintNum(i);PrintString(" \t||");
	PrintChar('\n');
    }
}

int main()
{
	PrintString("----------------------------------\n\n");
	PrintString("\t    BANG MA ASCII\n\n");
	PrintString("----------------------------------\n");
	PrintString("||    Character \tValue  ||\n");
	printASCII();
	PrintString("-------------------------------\n");
	Halt();
}

