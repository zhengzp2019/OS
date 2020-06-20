#include <iostream>
#define M 10 //最多M个进程
#define N 20 //最多N个资源
using namespace std;

int m, n;                //m个进程，n类资源
int name;                //进程号
int P[M];                //存储进程名
int Request[M][N];       //存储每个进程申请的资源数
int Available[N];        //可利用资源向量
int Max[M][N];           //最大需求矩阵，表示系统中每个进程对n类资源的最大需求
int Allocation[M][N];    //分配矩阵，定义了系统中每类资源当前已分配给进程的资源数
int Need[M][N];          //需求矩阵，表示每个进程尚需要的各类进程数
bool Finish[M];          //表示系统是否有足够的资源分配给进程，使之运行完成
bool Safe();             //安全性算法，检验系统是否安全,
                         //即将资源分配给请求进程后，正在进行的进程任能完成工作
bool Rank();             //银行家算法
void Input();            //输入初始化数据

int main()
{
    Input();
    while (!Safe())
    {
        cout << "初始系统不安全，请重新输入初始值" << endl;
        Input();
    }
    while (1)
    {
        do
        {
            cout << "请输入要申请资源的进程号：（第一个进程号为1，第二个进程号为2，依此类推）" << endl;
            cin >> name;
            cout << "请输入各类资源数目：" << endl;
            for (int i = 1; i <= n; i++)
            {
                cin >> Request[name][i];
            }
        } while (!Rank());
        if (Safe())
        {
            cout << "请求成功，同意资源分配\n";
        }
        else
        {
            cout << "系统不安全，请求失败，拒绝分配资源\n";
            for (int i = 1; i <= n; i++)
            { //分配失败，本次试探分配作废，恢复原来分配状态，让进程name等待
                Available[i] += Request[name][i];
                Allocation[name][i] -= Request[name][i];
                Need[name][i] += Request[name][i];
            }
        }
        char flag;
        cout << "是否再次请求分配？是请按Y/y,否请按N/n\n";
        while (1)
        {
            cin >> flag;
            if (flag == 'Y' || flag == 'y' || flag == 'N' || flag == 'n')
                break;
            else
            {
                cout << "输入错误，请按要求重新输入" << endl;
            }
        }
        if (flag == 'Y' || flag == 'y')
            continue;
        else
        {
            break;
        }
    }
    return 0;
}

void Input()
{
    cout << "输入进程的数目" << endl;
    cin >> m;
    cout << "输入资源数目" << endl;
    cin >> n;
    cout << "请输入每个进程最多所需的各资源数，按照"
         << m << "x" << n << "矩阵输入" << endl;
    //输入Max矩阵
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
            cin >> Max[i][j];
    cout << "请输入每个进程已分配的各资源数,也按照"
         << m << "x" << n << "矩阵输入" << endl;
    //输入Allocation矩阵,并求得Need矩阵
    for (int i = 1; i <= m; i++)
        for (int j = 1; j <= n; j++)
        {
            cin >> Allocation[i][j];
            Need[i][j] = Max[i][j] - Allocation[i][j];
            if (Need[i][j] < 0)
            {
                cout << "您输入的第" << i + 1
                     << "个进程所拥有的第" << j + 1
                     << "个资源数 错误,请重新输入:" << endl;
                j--;
                continue;
            }
        }
    cout << "请输入各个资源现有的数目:" << endl;
    //输入Available矩阵
    for (int i = 1; i <= n; i++)
    {
        cin >> Available[i];
    }
}

bool Rank()
{
    for (int i = 1; i <= n; i++)
    {
        if (Request[name][i] > Need[name][i])
        {
            cout << "所请求资源数超过进程的需求量！请重新输入" << endl;
            return false;
        }
        if (Request[name][i] > Available[i])
        {
            cout << "所请求资源数超过系统所有的资源数！请重新输入" << endl;
            return false;
        }
    }
    for (int i = 1; i <= n; i++)
    { //满足前上面两个要求，试探分配
        Available[i] -= Request[name][i];
        Allocation[name][i] += Request[name][i];
        Need[name][i] -= Request[name][i];
    }
    return true;
}

bool Safe()
{
    int num = 0; //记录Finish[i]==true的进程数
    int Work[N]; //工作向量，表示系统可提供给进程继续运行所需要的各类资源数目
    for (int i = 1; i <= n; i++)
    { //work的初始值等于Available
        Work[i] = Available[i];
    }
    for (int i = 1; i <= m; i++)
    {
        Finish[i] = false;
    }
    int circle = 1; //记录循环次数
    //每次至少有一个满足条件Finish[i]==true，Need[i][j]<Work[i]的进程，最多m次结束
    while (circle <= m && num != m)
    {
        for (int i = 1; i <= m; i++)
        { //查找Finish[i]==false的进程
            if (Finish[i] == true)
                continue;
            else
            {
                int j = 0;
                for (j = 1; j <= n; j++)
                {
                    if (Need[i][j] > Work[j]) //第i进程的第j个资源数是否大于第j类已有的资源数
                        break;
                }
                if (j > n) //i进程的每个需要的资源都比提供的少
                {
                    for (int k = 1; k <= n; k++)
                    { //将分配给进程i的资源释放
                        Work[k] += Allocation[i][k];
                    }
                    Finish[i] = true;
                    P[++num] = i;
                }
            }
        }
        circle++;
    }
    if (num == m) //若所有的进程都满足Finish[i]==ture
    {
        cout << "系统是安全的" << endl;
        cout << "系统安全序列是：" << endl;
        for (int i = 1; i <= m; i++)
        {
            cout << P[i];
            if (i != m)
            {
                cout << "-->";
            }
        }
        cout << '\n';
        return true;
    }
    else
    {
        return false;
    }
}