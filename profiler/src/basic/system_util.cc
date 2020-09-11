#include "system_util.h"

int invoke_system_call(const char* call, char* buff, int buff_len)
{
    int rtn = 0;
    FILE *stream;

    memset( buff, '\0', buff_len );//初始化buf,以免后面写如乱码到文件中
    stream = popen( call, "r" ); //将“ls －l”命令的输出 通过管道读取（“r”参数）到FILE* stream
    if (stream == NULL)
    {
        fprintf(stderr, "Cannot execute the command\n");
        return -1;
    }
    rtn = fread( buff, sizeof(char), buff_len,  stream);  //将刚刚FILE* stream的数据流读取到buf中
    pclose( stream );

    return rtn;
}

int getProcessNumber()
{
    char buff[16];
    invoke_system_call("cat /proc/cpuinfo |grep processor | wc | awk {'print $1'}", buff, 16);
    return atoi(buff);
}