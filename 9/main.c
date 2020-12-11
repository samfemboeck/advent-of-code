#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

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


int main()
{
    FILE* file = openFile("../input.txt");

    #define MaxNums 1000
    long long nums[MaxNums];
    char buf[200];
    for (int i = 0; fgets(buf, sizeof buf, file) != NULL; nums[i] = atoll(buf), ++i);

    long long target = 57195069;
    for (int i = 0 ; i < MaxNums - 1; ++i)
    {
        long long sum = nums[i];
        for (int a = i + 1; a < MaxNums; ++a)
        {
            sum += nums[a];
            if (sum == target)
            {
                long long max = 0;
                long long min = LLONG_MAX;

                for (int j = i; j <= a; ++j)
                {
                    if (nums[j] < min)
                        min = nums[j];
                    if (nums[j] > max)
                        max = nums[j];
                }

                printf("%lld\n", min + max);
            }
            else if (sum > target)
                break;
        }
    }
}
