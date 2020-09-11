#ifndef RESOURCE_PROFILER_H
#define RESOURCE_PROFILER_H

#include <iostream>
#include <string>
#include <sstream>
#include "system_util.h"
#include "string_util.h"

using namespace std;

class resource_profiler {

public:
    int getWorkload(double &one, double &five, double &fifteen);
    int getFreeMemory(double &free_mem, double &free_swap);
	int getMaxWorkloadAndMemory(double &max_workload, double &max_mem);
    double getNetworkStatus();

private:

};

#endif