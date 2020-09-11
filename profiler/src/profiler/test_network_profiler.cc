#include "network_profiler.h"

int main()
{
    network_profiler pro;
    string site = "127.0.0.1";//10.192.21.84
    cout << "Latency: " << pro.getLatency(site, 10) << endl;
    cout << "Loss Rate: " << pro.getLossRate(site, 10) << endl;
    cout << "Bandwitdh: " << pro.getBandwidth(site, 5) << endl;
    return 0;
}
