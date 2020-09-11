#include "network_profiler.h"

double network_profiler::getLatency(string address, int count)
{
    string cmd_ping_version = "ping 2>&1";
    char *buff = new char[1024];
    invoke_system_call(cmd_ping_version.c_str(), buff, 1024);
    string ping_version = buff;
    int ping_version_flag = 0; // 0 linux normal, 1 busybox
    string cmd_ping = string("ping ") + address + string(" -c ") + getstring(count);
    if (ping_version.find("BusyBox") != string::npos) // busybox ping
    {
        ping_version_flag = 1;
        cmd_ping.append(" | grep min/avg/max | awk -F/ {'print $4'}");
    }
    else // linux normal
    {
        cmd_ping.append(" -i 0.2 | grep min/avg/max | awk -F/ {'print $5'}");
    }
    cout << cmd_ping << endl;
    invoke_system_call(cmd_ping.c_str(), buff, 1024);
    double latency = atof(buff);
    delete[] buff;
    return latency;
}

double network_profiler::getBandwidth(string address, int second)
{
    char *buff = new char[1024];
    string cmd_iperf = string("iperf -c ") + address + string(" -t ") + getstring(second) + string(" -f M") + string(" | grep /sec | awk {'print $7'}");
    cout << cmd_iperf << endl;
    invoke_system_call(cmd_iperf.c_str(), buff, 1024);
    // MB/s
    double bandwidth = atof(buff);
    delete[] buff;
    return bandwidth;
}

double network_profiler::getLossRate(string address, int count)
{
    string cmd_ping_version = "ping 2>&1";
    char *buff = new char[1024];
    invoke_system_call(cmd_ping_version.c_str(), buff, 1024);
    string ping_version = buff;
    int ping_version_flag = 0; // 0 linux normal, 1 busybox
    string cmd_ping = string("ping ") + address + string(" -c ") + getstring(count);
    if (ping_version.find("BusyBox") != string::npos) // busybox ping
    {
        ping_version_flag = 1;
        cmd_ping.append(" | grep 'packet loss' | awk {'print $7'}");
    }
    else // linux normal
    {
        cmd_ping.append(" -i 0.2 | grep 'packet loss' | awk {'print $6'}");
    }
    cout << cmd_ping << endl;
    invoke_system_call(cmd_ping.c_str(), buff, 1024);
    double latency = atof(buff);
    delete[] buff;
    return latency;
}
