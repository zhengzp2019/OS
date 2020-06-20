#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int FIFO(const vector<int> &vec); //先入先出置换算法,返回缺页次数
int LRU(const vector<int> &vec);  //最近最久未使用算法，返回缺页次数

int main()
{
    int N; //待访问进程序列大小
    cout << "请输入待访问进程序列大小\n";
    cin >> N;
    vector<int> vec(N); //存储待访问的页面序列
    cout << "请输入待访问进程页面序列\n";
    for (int i = 0; i < N; i++)
    {
        cin >> vec[i];
    }
    cout << "先入先出置换算法执行如下：\n";
    FIFO(vec);
    cout << "最近最久未使用算法结果如下：\n";
    LRU(vec);
    return 0;
}

int FIFO(const vector<int> &vec)
{
    unsigned int n; //内存块大小
    cout << "请输入内存块大小\n";
    cin >> n;
    queue<int> q;     //记录每个页面进入内存的时间
    vector<int> v(n); //模拟内存
    int cnt = 1;      //记录缺次数
    for (unsigned int i = 0; i < vec.size(); i++)
    {
        if (i < n)
        { //在内存块未满时，页面都缺页
            cout << "第" << cnt++ << "次缺页:" << vec[i] << endl;
            v[i] = (vec[i]);
            q.push(vec[i]);
        }
        else
        {
            if (find(v.begin(), v.end(), vec[i]) == v.end())
            { //内存块中能找不到页号，缺页
                int page = q.front();
                q.pop();
                *find(v.begin(), v.end(), page) = vec[i];
                q.push(vec[i]);
                cout << "第" << cnt++ << "次缺页" << vec[i] << endl;
            }
        }
    }
    cout << "先入先出算法缺页率为：" << (--cnt) / (float)vec.size() << endl;
    return cnt;
}

int LRU(const vector<int> &vec)
{
    int cnt = 1;    //记录缺页次数
    unsigned int n; //内存块大小
    cout << "请输入内存块大小\n";
    cin >> n;
    vector<int> v(n); //模拟内存
    int time = 0;     //模拟页面访问时间
    vector<int> t(n); //记录每个页面最近进入内存的时间
    for (unsigned int i = 0; i < vec.size(); i++)
    {
        time++;
        if (i < n) //内存未满时，均缺页
        {
            v[i] = vec[i];
            t[i] = time;
            cout << "第" << cnt++ << "次缺页:" << vec[i] << endl;
        }
        else
        {
            if (find(v.begin(), v.end(), vec[i]) == v.end())
            {
                cout << "第" << cnt++ << "次缺页:" << vec[i] << endl;
                //内存中未找到叶浩即缺页,查找最近最久未使用的页面，即t[]中的最小值
                int k = 0; //记录最近最久未使用页面的位置
                int min = 100;
                for (unsigned int j = 0; j < n; j++)
                {
                    if (t[j] < min)
                    {
                        min = t[j];
                        k = j;
                    }
                }
                //找到最近最久未使用的页面后更新内存v[]和最近进入内存的时间t[]
                v[k] = vec[i];
                t[k] = time;
            }
            else
            { //未缺页,找到页面在内存中的位置，更新最久进入内存的时间t[]
                t[find(v.begin(), v.end(), vec[i]) - v.begin()] = time;
            }
        }
    }
    cout << "最近最久未使用算法缺页率为：" << (--cnt) / (float)vec.size() << endl;
    return cnt;
}
