#ifndef CENTROL_PROFILER_H
#define CENTROL_PROFILER_H

#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string>

#include "network_profiler.h"
#include "resource_profiler.h"
// #include "RPCMessage.pb.h"
#include "tcp_client.h"

using namespace std;

class centrol_profiler {

public:
	
	string str_nodeID;		//节点号
	string str_latency;		//延迟
	string str_lossRate;	//丢包率
	string str_bandwidth;	//带宽
	string str_maxWorkload;	//最大workload
	string str_maxMemory;	//最大内存
	string str_workload;	//CPU workload
	string str_freeMemory;	//free memory
	string str_timeStamp;	//时间戳
	
	// 配置文件数据
	string str_destAdd;
	int count_getLatency;
	int count_getLossRate;
	int second_getBandwidth;
	int second_Period;
	
    int get_all_performance();
	int get_resource_performance_cloud();
	int get_network_performance_cloud();
	// int send_performance_info();

private:
	
	int read_config(string &str_destAdd, int &count_getLatency, int &count_getLossRate, int &second_getBandwidth, int &second_Period);
	void Int2String(int data, string &str_data);
	void LInt2String(long data, string &str_data);
	void Double2String(double data, string &str_data);
	long getTimeStamp();

};

#endif