//标准程序
#include<cstdio>
#define NUM 10

int main()
{
    int A, B;
    for (int i = 0; i < NUM; i++)
    {
        scanf("%d %d", &A, &B);
        printf("%d\n", A + B);
    }
    return 0;
}