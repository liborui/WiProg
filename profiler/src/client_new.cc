#include <iostream>
#include <fstream>
#include <string>

#include "tcp_client.h"
#include "stdio.h"
// #include "RPCMessage.pb.h"
#include "centrol_profiler.h"

using namespace std;

int main()
{
    centrol_profiler profiler;
	cout << "test1" << endl;
	profiler.get_all_performance(); 
	cout << "test2" << endl;
	//profiler.send_performance_info();
	cout << "test3" << endl;
	
    return 0;
}