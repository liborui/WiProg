#include "system_util.h"

int invoke_system_call(const char* call, char* buff, int buff_len)
{
    int rtn = 0;
    FILE *stream;

    memset( buff, '\0', buff_len );//��ʼ��buf,�������д�����뵽�ļ���
    stream = popen( call, "r" ); //����ls ��l���������� ͨ���ܵ���ȡ����r����������FILE* stream
    if (stream == NULL)
    {
        fprintf(stderr, "Cannot execute the command\n");
        return -1;
    }
    rtn = fread( buff, sizeof(char), buff_len,  stream);  //���ո�FILE* stream����������ȡ��buf��
    pclose( stream );

    return rtn;
}

int getProcessNumber()
{
    char buff[16];
    invoke_system_call("cat /proc/cpuinfo |grep processor | wc | awk {'print $1'}", buff, 16);
    return atoi(buff);
}