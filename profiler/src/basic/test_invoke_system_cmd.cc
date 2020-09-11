#include <iostream>
#include "invoke_system_cmd.h"

using namespace std;

int main()
{
    char *cmd = "ping www.baidu.com -c 10";
    char *buff = new char[1024];
    int n = invoke_system_call(cmd, buff, 1024);
    cout << "Recv: " << n << " chars\n" << buff << endl;
    return 0;
}