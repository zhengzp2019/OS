#include <cstdio>
#include <cstring>
#include <cstdlib>
#define MAXN 10 //就绪队列的最大进程数
#define NUM 5   //实际进程数

typedef struct process
{
    char name[10]; //进程名
    int AT;        //到达时间
    int ST;        //服务时间
    int CT;        //执行时间
    int FT;        //完成时间
    int state;     //标志进程状态，2为完成，0为未进入就绪队列，1表示在就绪队列中
} process;

int PR;        //时间片
int num = NUM; //记录未完成的进程
int times = 0; //记录总的时间
FILE *fp_in = fopen("PR_in", "r");
FILE *fp_out = fopen("PR_out", "w");

void Input(process pcs[], int n /*==NUM*/);                                                 //输入各进程信息
void Init(process pcs[], process pq[], int *first, int *rear, int *n /*就绪队列的进程数*/); //每次输入一个PR后用于初始化就绪队列
void Work(process pcs[], process pq[], int first, int rear, int *n);                        //时间片下处理机工作
void Search(process pcs[], process p);                                                      //在pcs中查找进程p
void Insert(process pcs[], process pq[], int *first, int *rear, int *n);                    //每个时间片结束后使到达进程入队
inline void Print_run(const process pq[], int first, int rear, int *n);                     //输出每个时间片完成后的进程信息
inline void Print_finish(const process pcs[]);                                              //所有进程完成后输出结果

int main()
{
    process pcs[MAXN + 5]; //存储进程信息
    int n = 1;             //就绪队列进程数
    Input(pcs, NUM);
    while (fscanf(fp_in, "PR=%d", &PR) != EOF)
    {
        fprintf(fp_out, "********************\n********************\n");
        fprintf(fp_out, "PR=%d时进程执行信息如下\n\n", PR);
        process pq[MAXN + 5];        //进程就绪队列
        int first = 1, rear = first; //first指向头节点，rear指向尾结点的下一位
        Init(pcs, pq, &first, &rear, &n);
        while (num)
        {
            fprintf(fp_out, "CPU开始时间：%d\n", times);
            Work(pcs, pq, first, rear, &n);
            Print_run(pq, first, rear, &n);
            Insert(pcs, pq, &first, &rear, &n);
            //出队
            pq[0] = pq[first];
            pq[first].AT = 0;
            pq[first].CT = 0;
            pq[first].FT = false;
            for (int i = 1; i < 10; i++)
            {
                pq[first].name[i] = 0;
            }
            pq[first].ST = 0;
            pq[first].state = 0;
            do
            {
                first = first % MAXN + 1;
            } while (pq[first].state == 2);
            pq[rear] = pq[0]; //入队
            do
            {
                rear = rear % MAXN + 1;
            } while (pq[rear].state == 2);
        }
        Print_finish(pcs);
        fgetc(fp_in);
    }
    fclose(fp_in);
    fclose(fp_out);
    return 0;
}

void Input(process pcs[], int n /*==NUM*/)
{
    for (int i = 1; i <= n; i++)
    {
        pcs[i].FT = 0;
        pcs[i].state = 0;
        pcs[i].CT = 0;
        fscanf(fp_in, "name=%s", pcs[i].name);
        fgetc(fp_in);
        fscanf(fp_in, "AT=%d", &pcs[i].AT);
        fgetc(fp_in);
        fscanf(fp_in, "ST=%d", &pcs[i].ST);
        fgetc(fp_in);
        fgetc(fp_in);
    }
}

void Init(process pcs[], process pq[], int *first, int *rear, int *n /*就绪队列的进程数*/)
{
    num = NUM;
    times = 0;
    int p = *first;
    //初始化pq
    for (int i = 0; i <= MAXN; i++)
    {
        pq[i].AT = 0;
        pq[i].CT = 0;
        pq[i].FT = 0;
        strcpy(pq[i].name, "\0");
        pq[i].ST = 0;
        pq[i].state = 0;
    }
    //初始化pcs
    for (int i = 1; i <= NUM; i++)
    {
        //初始化pcs
        pcs[i].CT = 0;
        pcs[i].FT = 0;
        pcs[i].state = 0;
        if (pcs[i].AT <= times && pcs[i].state == 0)
        {
            ++(*n);
            pcs[i].state = 1;
            pq[p] = pcs[i];
            *rear = *rear % MAXN + 1;
        }
    }
}

void Work(process pcs[], process pq[], int first, int rear, int *n)
{
    pq[first].AT = times;
    int ppr = PR;
    //将时间片按照一个时间点来进行
    while (ppr--)
    {
        ++pq[first].CT;
        ++times;
        if (pq[first].CT >= pq[first].ST)
        {
            pq[first].FT = times;
            pq[first].state = 2;
            Search(pcs, pq[first]);
            --num;
            return;
        }
    }
}

void Search(process pcs[], process p)
{
    for (int i = 1; i <= NUM; i++)
    {
        if (strcmp(pcs[i].name, p.name) == 0)
        {
            pcs[i].CT = p.CT;
            pcs[i].FT = p.FT;
            pcs[i].state = p.state;
        }
    }
}

void Print_run(const process pq[], int first, int rear, int *n)
{
    fprintf(fp_out, "当前运行的进程信息如下：\n");
    fprintf(fp_out, "进程名：%s\nCPU执行时间：%d\n服务时间：%d\n\n", pq[first].name, pq[first].CT, pq[first].ST);
    fprintf(fp_out, "就绪队列信息如下：\n");
    int p = first;
    while (p != rear)
    {
        if (pq[p].state != 2)
        {
            fprintf(fp_out, "进程名：%s\n到达时间：%d\n服务时间：%d\nCPU执行时间：%d\n", pq[p].name, pq[p].AT, pq[p].ST, pq[p].CT);
            switch (pq[p].state)
            {
            case 1:
                fprintf(fp_out, "当前状态：未完成\n");
                break;
            case 2:
                fprintf(fp_out, "当前状态：已完成\n完成时间：%d\n", pq[p].FT);
                break;
            default:
                fprintf(fp_out, "当前进程状态出错\n");
                break;
            }
        }
        p = p % MAXN + 1;
    }
    fprintf(fp_out, "CPU结束时间：%d\n", times);
    fprintf(fp_out, "***********************************\n");
}

void Insert(process pcs[], process pq[], int *first, int *rear, int *n)
{
    int number = 0; //存储每个时间片后应该入队的进程数；
    for (int i = 1; i <= NUM; i++)
    {
        if (pcs[i].state == 0 && pcs[i].AT <= times)
            ++number;
    }
    //将满足就绪条件的进程插入队列
    int q = (*rear - 2 + MAXN) % MAXN + 1; //q指向移动前队尾元素
    *rear = (*rear - 1 + number) % MAXN + 1;
    int p = (*rear - 2 + MAXN) % MAXN + 1; //p指向移动后队尾元素
    //移动
    while (q != *first)
    {
        pq[p] = pq[q];
        p = (p - 2 + MAXN) % MAXN + 1;
        q = (q - 2 + MAXN) % MAXN + 1;
    }
    //进程入队
    for (int i = 1; i <= NUM; i++)
    {
        if (pcs[i].state == 0 && pcs[i].AT <= times)
        {
            q = q % MAXN + 1;
            pcs[i].state = 1;
            pq[q] = pcs[i];
        }
    }
    *n = *n + number;
}

void Print_finish(const process pcs[])
{
    float w = 0;   //记录每个带权周转时间
    float sum = 0; //带权周转时间之和
    for (int i = 1; i <= NUM; i++)
    {
        w = (pcs[i].FT - pcs[i].AT) / (float)pcs[i].ST;
        fprintf(fp_out, "进程名：%s\t完成时间：%3d\t周转时间：%3d\t带权周转时间：%.2f\t\n", pcs[i].name, pcs[i].FT, pcs[i].FT - pcs[i].AT, w);
        sum += w;
    }
    fprintf(fp_out, "平均带权周转时间为：%.2f\n", sum / NUM);
}