#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <stdlib.h>
#include <stdbool.h>

char *rgxAssemblyLine = "([[:lower:]]{3}) ([+-][[:digit:]]{1,3})";
#define maxGroups 4
regmatch_t groupArray[maxGroups];

int instrCount = 0;
#define instrSize 625 * sizeof(char*)
char* instructions[625];

char* getMatch(int idx, char* p)
{
    regmatch_t target = groupArray[idx];
    int charCount = target.rm_eo - target.rm_so;
    char* ret = malloc(charCount + 1);
    ret[charCount] = 0;
    strncpy(ret, p + target.rm_so, charCount);
    return ret;
}

FILE* openFile(char* name)
{
    FILE* ret = fopen(name, "r");

    if (!ret)
    {
        perror("File opening failed");
        return EXIT_FAILURE;
    }

    return ret;
}

int execRegex(char *rgx, char *target)
{
    regex_t rgxCompiled;

    if (regcomp(&rgxCompiled, rgx, REG_EXTENDED))
    {
        printf("Error compiling regex.\n");
        return 1;
    };

    int error = regexec(&rgxCompiled, target, maxGroups, groupArray, 0);

    regfree(&rgxCompiled);
    return !error;
}

int executeProgram(int nopJmpSwapIdx)
{
    int processedLines[625] = {0};
    int acc = 0;
    int curNopJmpSwapIdx = 0;

    for (int i = 0, iOff = 1; i < instrCount; i += iOff, iOff = 1)
    {
        if (processedLines[i] == 1)
            break;

        processedLines[i] = 1;
        execRegex(rgxAssemblyLine, instructions[i]);
        char* instr = getMatch(1, instructions[i]);

        if (strcmp(instr, "nop") == 0 || strcmp(instr, "jmp") == 0)
        {
            if (curNopJmpSwapIdx++ == nopJmpSwapIdx)
            {
                if (strcmp(instr, "nop") == 0)
                    instr = "jmp";
                else
                    instr = "nop";
            }
        }

        if (strcmp(instr, "nop") == 0)
            continue;
        else if (strcmp(instr, "acc") == 0)
        {
            int add = atoi(getMatch(2, instructions[i]));
            acc += add;
        }
        else if (strcmp(instr, "jmp") == 0)
            iOff = atoi(getMatch(2, instructions[i]));

        if (i == instrCount - 1)
            return acc;
    }

    return 0;
}

int main ()
{
    FILE* file = openFile("../input.txt");

    char buf[200];
    while (fgets(buf, sizeof buf, file) != NULL)
        instructions[instrCount++] = strdup(buf);

    for (int i = 0; i < 1000; ++i)
    {
        int acc = executeProgram(i);
        if (acc != 0)
        {
            printf("%i\n", acc);
            break;
        }
    }

    return 0;
}
