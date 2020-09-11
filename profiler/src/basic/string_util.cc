#include "string_util.h"

string getstring(const int n)
{
    std::stringstream newstr;
    newstr<<n;
    return newstr.str();
}