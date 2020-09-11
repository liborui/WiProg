#include "centrol_profiler.h"

int main()
{
    centrol_profiler centrol_pro;
	
	pid_t pid;
	fstream fout;
	
	pid=fork();              //父进程返回的pid是大于零的，而创建的子进程返回的pid是 等于0的，这个机制正好用来区分 父进程和子进程
	if(pid==0)//子进程
	{
		cout << "child" << endl;
		centrol_pro.get_all_performance();
	}
	else      //父进程
	{
		wait(NULL);   //等待子进程结束
		cout << "parent" << endl;
		fout.open("../testresults.txt", ios::in);
		if(!fout)
		{
			cout << "open 'testresult.txt' failed!" << endl;
			return -1;
		}
		else
		{
			cout << "open file success" << endl;
			fout.close();
		}
	}
    return 0;
}