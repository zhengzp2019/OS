/*
 *该实验为进程调度算法
 *以五个进程为例描述进程调度的过程
 */
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <fstream>
using namespace std;

#define NUM 5   //就绪队列能够存储的最大进程数
#define TIMES 5 //最大运行时间
#define MAXN 5  //最大优先数
ifstream in("PCB_in.txt");
ofstream out("PCB_out.txt");

typedef enum state
{
    W, //wait
    F, //finish
    R  //run
} state;

typedef struct Pcb
{
    string name;
    int priority; //优先数
    int AT;       //到达时间
    int RT;       //需要运行时间
    int CT;       //已用CPU时间
    state s;      //进程状态
} Pcb;

void Input(vector<Pcb> &P);          //输入内容
void Search(vector<Pcb> &P, int &k); //找到优先级最高的PCB,并使它由就绪状态转换成执行状态
void Work(vector<Pcb> &P, int &k);   //完成执行状态
void Print(vector<Pcb> &P, int k);   //输出中间结果
int num = NUM;                       //记录未完成的进程
int times = 0;                       //总的时间，记录进程进入时间，从0开始

int main()
{
    int k = 1;              //优先级最大的进程下标
    vector<Pcb> P(NUM + 5); //存放进程
    Input(P);
    while (num)
    {
        Search(P, k);
        Work(P, k);
        Print(P, k);
    }
    in.close();
    out.close();
    return 0;
}

void Input(vector<Pcb> &P)
{
    srand(time(NULL)); //删除随机数种子，每次运行程序前最好使用，不用会使每次随机生成的数据一样
    for (int i = 1; i <= NUM; i++)
    {
        in >> P[i].name;
        P[i].priority = rand() % MAXN + 1;
        P[i].AT = 0;
        P[i].RT = rand() % TIMES + 1;
        P[i].CT = 0;
        P[i].s = W;
    }
}

//搜索
void Search(vector<Pcb> &P, int &k)
{
    int j = 1;
    for (; j <= NUM; j++)
    {
        if (P[j].s != F)
        {
            k = j;
            break;
        }
    }
    for (int i = j; i <= NUM; i++)
    {
        if (P[i].s != F && P[i].priority > P[k].priority)
            k = i;
    }
    P[k].s = R;
}

void Work(vector<Pcb> &P, int &k)
{
    ++P[k].CT;
    P[k].AT = times;
    if (P[k].CT >= P[k].RT)
    {
        P[k].s = F;
        --num;
    }
    else
    {
        P[k].s = W;
    }
    --P[k].priority;
    ++times;
}

//输出运行进程、就绪队列、以及各个进程的 Pcb
void Print(vector<Pcb> &P, int k)
{
    out << "*******第" << times << "次进程调度信息如下*******\n";
    out << "当前运行进程" << P[k].name << "信息如下:\n";
    out << "到达时间：" << P[k].AT << '\n'
        << "需要运行时间：" << P[k].RT << '\n'
        << "CPU占用时间：" << P[k].CT << '\n';
    out << "进程状态：Run\n"
        << endl;
    out << "就绪队列信息如下:\n";
    for (int i = 1; i <= NUM; i++)
    {
        if (P[i].s != 'F')
        {
            out << "进程名：" << P[i].name << '\n'
                << "到达时间：" << P[i].AT << '\n'
                << "需要运行时间：" << P[i].RT << '\n'
                << "CPU占用时间：" << P[i].CT << '\n';
            out << "进程状态：";
            switch (P[i].s)
            {
            case W:
                out << "Wait\n"
                    << endl;
                break;
            case R:
                out << "Run\n"
                    << endl;
                break;
            default:
                break;
            }
        }
    }
    out << "各个进程信息如下:\n";
    for (int i = 1; i <= NUM; i++)
    {
        out << "进程名：" << P[i].name << '\n'
            << "到达时间：" << P[i].AT << '\n'
            << "需要运行时间：" << P[i].RT << '\n'
            << "CPU占用时间：" << P[i].CT << '\n';
        out << "进程状态：";
        switch (P[i].s)
        {
        case W:
            out << "Wait\n"
                << endl;
            break;
        case R:
            out << "Run\n"
                << endl;
            break;
        case F:
            out << "Finish\n"
                << endl;
            break;
        default:
            break;
        }
    }
}