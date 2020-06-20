//我的程序
#include <cstdio>
#define NUM 10 //程序执行次数

int main()
{
    int A, B;
    for (int i = 1; i <= NUM; i++)
    {
        scanf("%d %d", &A, &B);
        printf("%d\n", A + B);
    }
    return 0;
}