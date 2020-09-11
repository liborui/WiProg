#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "network_profiler.h"
#include "resource_profiler.h"
#include "centrol_profiler.h"
#include <sqlite3.h>

#define IP_CLOUD "47.97.201.66"

static int sql_callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

void print_row(int n_values,char** values)
{
    int i;
    for (i = 0; i < n_values; ++i) 
    {
        if (i > 0) 
        {
            printf("\t");
        }
        printf("%s", values[i]); 
    }
    printf("\n");
}

int print_result_cb(void* data,int n_columns,char** column_values,char** column_names)
{
    static int column_names_printed = 0;
    int i; 
    if (!column_names_printed) //首次执行结果是列名
    {
        print_row(n_columns, column_names);
        column_names_printed = 1;
    }

    print_row(n_columns, column_values);
    return 0;
}

int centrol_profiler::read_config(string &str_destAdd, int &count_getLatency, int &count_getLossRate, int &second_getBandwidth, int &second_Period)
{
	fstream fin;
	fin.open("../assets/scheduler.conf", ios::in);

	if(!fin)
	{
		cout << "open conf file failed!" << endl;
		return -1;
	}

	if(!fin.eof())
	{
		fin >> str_nodeID >> str_destAdd >> count_getLatency >> count_getLossRate >> second_getBandwidth >> second_Period;
	}
	
	fin.close();
	
	this->str_nodeID = str_nodeID;
	this->str_destAdd = str_destAdd;
	this->count_getLatency = count_getLatency;
	this->count_getLossRate = count_getLossRate;
	this->second_getBandwidth = second_getBandwidth;
	this->second_Period = second_Period;
	
	return 0;
}


int centrol_profiler::get_all_performance()
{
	network_profiler network_pro;
	resource_profiler resource_pro;
	string str_destAdd = "";    //Ping 目标主机IP地址
	int count_getLatency = 0;
	int count_getLossRate = 0;
	int second_getBandwidth = 0;
	int second_Period = 0;    //运行周期
	long timeStamp = 0;
	double latency = 0;
	double lossRate = 0;
	double bandWidth = 0;
    double process_ct_1min = 0;
    double process_ct_5min = 0;
    double process_ct_15min = 0;
	double free_mem_in_kb = 0;
    double free_swap_in_kb = 0;
	double max_workload = 0;
	double max_mem = 0;
	fstream fout;

    sqlite3 *pdb;
    char *errMsg;
    int rc;
    char sql[500];

    rc = sqlite3_open("../assets/profile.db", &pdb);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(pdb));
        exit(0);
    }else{
        fprintf(stderr, "Opened database successfully\n");
    }
    /* Create SQL statement */
    sprintf(sql, "CREATE TABLE IF NOT EXISTS resource_profile("  \
            "id         INTEGER     PRIMARY KEY AUTOINCREMENT," \
            "profile_timestamp      TIMESTAMP   NOT NULL    DEFAULT (datetime('now','localtime'))," \
            "latency    REAL        NOT NULL," \
            "loss_rate  REAL        NOT NULL," \
            "bandwidth  REAL        NOT NULL," \
            "cpu_load   REAL        NOT NULL," \
            "free_mem   REAL        NOT NULL);");

    /* Execute SQL statement */
    rc = sqlite3_exec(pdb, sql, sql_callback, 0, &errMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }else{
        fprintf(stdout, "Table created successfully\n");
    }
	/*
	if(daemon(0,0) == -1)
	{
		printf("daemon error\n");
		return -1;
	}*/

	read_config(str_destAdd, count_getLatency, count_getLossRate, second_getBandwidth, second_Period);

    while(true){
        latency = network_pro.getLatency(str_destAdd, count_getLatency);
        lossRate = network_pro.getLossRate(str_destAdd, count_getLossRate);
        bandWidth = network_pro.getBandwidth(str_destAdd, second_getBandwidth);
        resource_pro.getWorkload(process_ct_1min, process_ct_5min, process_ct_15min);
        resource_pro.getFreeMemory(free_mem_in_kb, free_swap_in_kb);
        sprintf(sql, "INSERT INTO resource_profile(latency, loss_rate, bandwidth, cpu_load, free_mem) VALUES(%f, %f, %f, %f, %f)", latency, lossRate, bandWidth, process_ct_1min, free_mem_in_kb);
        rc = sqlite3_exec(pdb, sql, print_result_cb, 0, &errMsg);
        sprintf(sql, "SELECT * FROM resource_profile");
        rc = sqlite3_exec(pdb, sql, print_result_cb, 0, &errMsg);
        sleep(second_Period);
    }
	// while(true)
	// {

	// 	fout.open("../testresults.txt", ios::out | ios::app);
	// 	if(!fout)
	// 	{
	// 	cout << "open 'testresult.txt' failed!" << endl;
	// 	return -1;
	// 	}

	// 	fout << "Latency: " << network_pro.getLatency(str_destAdd, count_getLatency) << endl;
	// 	fout << "Loss Rate: " << network_pro.getLossRate(str_destAdd, count_getLossRate) << endl;
	// 	fout << "Bandwitdh: " << network_pro.getBandwidth(str_destAdd, second_getBandwidth) << endl;

	// 	resource_pro.getWorkload(o, f, fn);
	// 	fout << "CPU Load: " << o << ", " << f << ", " << fn << endl;
	// 	resource_pro.getFreeMemory(fm, fc);
	// 	fout << "Free Mem: " << fm << ", " << fc << endl;

	// 	latency = network_pro.getLatency(str_destAdd, count_getLatency);
	// 	lossRate = network_pro.getLossRate(str_destAdd, count_getLossRate);
	// 	bandWidth = network_pro.getBandwidth(str_destAdd, second_getBandwidth);
	// 	resource_pro.getWorkload(o, f, fn);
	// 	resource_pro.getFreeMemory(fm, fc);
	// 	resource_pro.getMaxWorkloadAndMemory(max_workload, max_mem);
	// 	timeStamp = getTimeStamp();

	// 	LInt2String(timeStamp, str_timeStamp);
	// 	Double2String(latency, str_latency);
	// 	Double2String(lossRate, str_lossRate);
	// 	Double2String(bandWidth, str_bandwidth);
	// 	Double2String(o, str_workload);
	// 	Double2String(fm, str_freeMemory);
	// 	Double2String(max_workload, str_maxWorkload);
	// 	Double2String(max_mem, str_maxMemory);


	// 	fout.close();

	// 	// send_performance_info();

	// 	sleep(second_Period);
	// }
}

// 针对cloud 端,采集设备运行资源信息（不采集网络信息）
int centrol_profiler::get_resource_performance_cloud()
{
	resource_profiler resource_pro;
	string str_destAdd = "";    //Ping 目标主机IP地址
	int count_getLatency = 0;
	int count_getLossRate = 0;
	int second_getBandwidth = 0;
	int second_Period = 0;    //运行周期
	long timeStamp = 0;
	double o, f, fn, fm, fc;
	double max_workload = 0;
	double max_mem = 0;

	/*
	if(daemon(0,0) == -1)
	{
		printf("daemon error\n");
		return -1;
	}*/

	read_config(str_destAdd, count_getLatency, count_getLossRate, second_getBandwidth, second_Period);
	
	resource_pro.getWorkload(o, f, fn);
	resource_pro.getFreeMemory(fm, fc);
	resource_pro.getMaxWorkloadAndMemory(max_workload, max_mem);
	timeStamp = getTimeStamp();

	LInt2String(timeStamp, str_timeStamp);
	Double2String(o, str_workload);
	Double2String(fm, str_freeMemory);
	Double2String(max_workload, str_maxWorkload);
	Double2String(max_mem, str_maxMemory);

	// send_performance_info();

	// sleep(second_Period);
	return 0;
}


// 针对cloud 端,采集网络资源信息（不采集设备资源信息）
int centrol_profiler::get_network_performance_cloud()
{
	network_profiler network_pro;
	string str_destAdd = "";    //Ping 目标主机IP地址
	int count_getLatency = 0;
	int count_getLossRate = 0;
	int second_getBandwidth = 0;
	int second_Period = 0;    //运行周期
	double latency = 0;
	double lossRate = 0;
	double bandWidth = 0;

	/*
	if(daemon(0,0) == -1)
	{
		printf("daemon error\n");
		return -1;
	}*/

	read_config(str_destAdd, count_getLatency, count_getLossRate, second_getBandwidth, second_Period);
	
	latency = network_pro.getLatency(IP_CLOUD, count_getLatency);
	lossRate = network_pro.getLossRate(IP_CLOUD, count_getLossRate);
	// bandWidth = network_pro.getBandwidth(IP_CLOUD, second_getBandwidth);
	bandWidth = 5 - 1;

	Double2String(latency, str_latency);
	Double2String(lossRate, str_lossRate);
	Double2String(bandWidth, str_bandwidth);

	// send_performance_info();

	// sleep(second_Period);
	return 0;
}



void centrol_profiler::Int2String(int data, string &str_data)
{
	stringstream ss;
	ss << data;
	str_data = ss.str();
}

void centrol_profiler::LInt2String(long data, string &str_data)
{
	stringstream ss;
	ss << data;
	str_data = ss.str();
}

void centrol_profiler::Double2String(double data, string &str_data)
{
	stringstream ss;
	ss << data;
	str_data = ss.str();
}

long int centrol_profiler::getTimeStamp()
{
	struct timeval timeStamp;
	gettimeofday(&timeStamp, NULL);
	return timeStamp.tv_sec;
}

// int centrol_profiler::send_performance_info()
// {
// 	// Verify that the version of the library that we linked against is
// 	// compatible with the version of the headers we compiled against.
// 	GOOGLE_PROTOBUF_VERIFY_VERSION;

// 	offloading::Beacon *beacon = new offloading::Beacon();
// 	string str_Info;	//保存序列化后的数据
// 	char* resp[100];
// 	fstream output("performInfo.bin", ios::out | ios::trunc | ios::binary);

// 	beacon->mutable_bea_nodeinfo()->set_node_id(str_nodeID);
// 	beacon->mutable_bea_nodeinfo()->set_max_workload(str_maxWorkload);
// 	beacon->mutable_bea_nodeinfo()->set_max_memory(str_maxMemory);
// 	beacon->mutable_bea_nodeinfo()->set_workload(str_workload);
// 	beacon->mutable_bea_nodeinfo()->set_memory(str_freeMemory);
// 	beacon->mutable_bea_nodeinfo()->set_latency(str_latency);
// 	beacon->mutable_bea_nodeinfo()->set_bandwidth(str_bandwidth);
// 	beacon->mutable_bea_nodeinfo()->set_lossrate(str_lossRate);
// 	beacon->mutable_bea_nodeinfo()->set_timestamp(str_timeStamp);


// 	// Write the new address book back to disk.
// 	if (!beacon->SerializeToOstream(&output))
// 	{
// 		cerr << "Failed to write info." << endl;
// 		return -1;
// 	}

// 	// Optional:  Delete all global objects allocated by libprotobuf.
// 	google::protobuf::ShutdownProtobufLibrary();
// 	output.close();

// 	char *strf;
// 	int length;
// 	fstream input("performInfo.bin", ios::in | ios::binary);
// 	input.seekg(0, ios::end);
// 	length = input.tellg();
// 	input.seekg(0,std::ios::beg);
// 	strf = new char[length];
// 	input.read(strf, length);
// 	input.close();
// 	// cout << strf << endl;

// 	char buffer[4096];
// 	tcp_client cli;

// 	int res = cli.establish_connection("192.168.1.1", 12345);
// 	res = cli.send_message(strf, length);
// 	res = cli.wait_for_response(buffer);
// 	printf("Received: %d\n", res);
// 	cli.end_connection();
// 	delete[] strf;

// 	return res;
// }
