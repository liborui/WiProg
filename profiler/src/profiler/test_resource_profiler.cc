#include "resource_profiler.h"

int main()
{
    double o, f, fn, fm, fc;
    resource_profiler rp;
    rp.getWorkload(o, f, fn);
    cout << "CPU Load: " << o << ", " << f << ", " << fn << endl;
    rp.getFreeMemory(fm, fc);
    cout << "Free Mem: " << fm << ", " << fc << endl;
    return 0;
}