#include "resource_profiler.h"

int resource_profiler::getWorkload(double &one, double &five, double &fifteen)
{
    char buff[32];
    one = five = fifteen = 0;
    //命令输出的最后内容表示在过去的1、5、15分钟内运行队列中的平均进程数量。　　
    //如果一个进程满 足以下条件则其就会位于运行队列中：　　
    // - 它没有在等待I/O操作的结果　　
    // - 它没有主动进入等待状态(也就是没有调用'wait') 　　
    // - 没有被停止(例如：等待终止)
    //一般来说只要每个CPU的当前活动进程数不大于1那么系统的性能就是良好的，如果每个CPU的任务数大于1，那么就表示这台机器的性能有严重问题。
    //e.g. 10:38:20 up 115 days, 17:16,  4 users,  load average: 17.03, 17.04, 17.00
    //假设系统有4个CPU，那么其每个CPU的当前任务数为：17/4=4.25。这表示该系统的性能已经出现了问题的
    invoke_system_call("uptime | awk -F'load average: ' {'print $2'} | awk {'print $1,$2,$3'}", buff, 32);
    string str = buff;
    cout << "Direct output: " << str << endl;
    one = atof(str.c_str());
    str = str.substr(str.find(',') + 2);
    five = atof(str.c_str());
    str = str.substr(str.find(',') + 2);
    fifteen = atof(str.c_str());
    if (one != 0 && five != 0 && fifteen != 0)
    {
        return 0;
    }
    else 
    {
        return -1;
    }
}

int resource_profiler::getFreeMemory(double &free_mem, double &free_swap)
{
    int i = 0;
    char buff[128];
    free_mem = free_swap = 0;
    // NR: number of row
    invoke_system_call("free | awk 'NR==3||NR==4 {print $4}'", buff, 128);
    free_mem = atof(buff);
    while (buff[i] != '\n')
    {
        i++;
    }
    free_swap = atof(buff + i + 1);
    if (free_mem == 0 && free_swap == 0)
    {
        return -1;
    }
    return 0;
}

int resource_profiler::getMaxWorkloadAndMemory(double &max_workload, double &max_mem)
{
	char buff[16];
	
	max_workload = 0;
	max_mem = 0;
	invoke_system_call("cat /proc/cpuinfo| grep \"processor\"| wc -l", buff, 16);
	max_workload = atof(buff);
	memset(buff,0,16); 
	invoke_system_call("free | awk 'NR==2 {print $2}'", buff, 16);
	max_mem = atof(buff);
	if (max_workload == 0 && max_mem == 0)
    {
        return -1;
    }
    return 0;
}

double resource_profiler::getNetworkStatus()
{
    //netstat -n | awk '/^tcp/ {++S[$NF]} END {for(a in S) print a, S[a]}'
    return 0;
}
    
