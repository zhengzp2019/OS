#include "stack.cpp"
#include <string>
#include <iostream>
using namespace std;

int main()
{
    stack s;
    string str("zhengzhanpeng");
    s.empty();
    s.full();
    s.push(str);
    str = "0";
    s.top(str);
    str = "0";
    s.pop(str);
    cout << str;
    cout << s.size();
    s.~stack();
    return 0;
}