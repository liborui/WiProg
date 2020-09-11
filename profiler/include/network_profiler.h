#ifndef NETWORK_PROFILER_H
#define NETWORK_PROFILER_H

#include <iostream>
#include <string>
#include "system_util.h"
#include "string_util.h"

using namespace std;

class network_profiler {

public:
    double getLatency(string address, int count);
    double getBandwidth(string address, int second);
    double getLossRate(string address, int count);

private:
    
};

#endif