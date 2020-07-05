#include <iostream>
#include <time.h>
#include <iomanip>
using namespace std;

//页表类
class page_table
{
public:
    friend class address;
    page_table(int page_all); //构造函数
    void show(int page_all);  //显示页表
private:
    int block_number[100]; //存储页表
};

//随机生成页表
page_table::page_table(int page_all)
{
    cout << "页表随机生成中..." << endl;
    srand((int)time(0)); //初始化随机种子
    for (int i = 0; i < page_all; i++)
    {
        if (i == 0)
        {
            block_number[i] = (rand() % 4) + 1;
        }
        else
        {
            block_number[i] = block_number[i - 1] + (rand() % 4) + 1;
        }
    }
    cout << "页表生成完毕" << endl;
}

//显示页表
void page_table::show(int page_all)
{
    cout << "***************" << endl;
    cout << " 页号    块号" << endl;
    for (int i = 0; i < page_all; i++)
    {
        //setw(3)格式化输出
        cout << setw(3) << i << "  -->  " << setw(2) << block_number[i] << endl;
    }
    cout << "***************" << endl;
}

//地址类
class address
{
public:
    friend class page_table;
    address(int a, int page_all, int page_size);                       //构造函数
    int check(int page_all, int page_size);                            //检查逻辑地址是否合法
    void position(page_table Page_table, int page_all, int page_size); //计算物理地址
    void show();

private:
    int Log_add; //逻辑地址
    int Mac_add; //物理地址
};

address::address(int a, int page_all, int page_size)
{
    Log_add = a;
}

//检查输入的逻辑地址是否合法
int address::check(int page_all, int page_size)
{
    if (Log_add > page_all * page_size)
    {
        cout << "输入非法，该地址越界！" << endl;
        return 0;
    }
    return 1;
}

//计算物理地址
void address::position(page_table Page_table, int page_all, int page_size)
{
    int page_num = Log_add / page_size;
    int shifting = Log_add % page_size;
    Mac_add = (Page_table.block_number[page_num]) * page_size + shifting;
}

//显示物理地址
void address::show()
{
    cout << "对应的物理地址为：" << Mac_add << endl;
}

int main()
{
    cout << "****************************" << endl;
    cout << "***    页面地址重定位    ***" << endl;
    cout << "*** 1.显示页表信息       ***" << endl;
    cout << "*** 2.计算物理地址       ***" << endl;
    cout << "*** 3.退出模拟器         ***" << endl;
    cout << "****************************\n"
         << endl;

    //输入
    int page_size, page_all;
    cout << "请输入页号的长度：";
    cin >> page_all;
    cout << "请输入页面的大小：";
    cin >> page_size;
    page_table Page_table(page_all);

    int flag;
    while (true)
    {
        cout << "\n请选择功能：";
        cin >> flag;
        if (flag == 1)
        {
            //显示页表信息
            Page_table.show(page_all);
        }
        else if (flag == 2)
        {
            //检查逻辑地址&计算物理地址
            int AD;
            cout << "请输入逻辑地址：";
            cin >> AD;
            address add(AD, page_all, page_size);
            if (add.check(page_all, page_size))
            {
                add.position(Page_table, page_all, page_size);
                add.show();
            }
        }
        else if (flag == 3)
        {
            //退出
            cout << "成功退出！\n"
                 << endl;
            break;
        }
    }
    return 0;
}