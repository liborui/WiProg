#include "tcp_client.h"

tcp_client::tcp_client()
{
}

int tcp_client::establish_connection(const char* addr, uint16_t port)
{
    if( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){  
        printf("create socket error: %s(errno: %d)\n", strerror(errno),errno);  
        return -1;
    }

    memset(&servaddr, 0, sizeof(servaddr));  
    servaddr.sin_family = AF_INET;  
    servaddr.sin_port = htons(port);  
    if( inet_pton(AF_INET, addr, &servaddr.sin_addr) <= 0){  
        printf("inet_pton error for %s\n", addr);  
        return -1;
    }  

    if( connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){  
        printf("connect error: %s(errno: %d)\n",strerror(errno),errno);  
        return -1;
    }
    
    return 0;
}

int tcp_client::end_connection()
{
    close(sockfd);
}

int tcp_client::send_message(const char* message, int length)  
{
    char str[4];
    str[0] = (length + 1) % 256;
    str[1] = (length + 1) / 256 % 256;
	str[2] = (length + 1) / 256 / 256 % 256;
    str[3] = CRC8_Table(str, message, length);
    // printf("Try to send: %d\n", length + 1);
    // printf("%02x%02x%02x", str[0], str[1], str[2]);
//    for (int i = 0; i < length; i++)
//        printf("%02x", message[i]);
    // printf("%02x\n", str[3]);
    if( send(sockfd, str, 3, 0) < 0)  
    {
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
        return -1;
    }
    while (length > 0)
    {
        if( send(sockfd, message, length > MAXLINE ? MAXLINE : length, 0) < 0)  
        {  
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
            return -1;
        }
        message += MAXLINE;
        length -= MAXLINE;
		// printf("test: %d\n", length);
    }
    if( send(sockfd, str + 3, 1, 0) < 0)  
    {  
		printf("test3\n");
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
        return -1;
    }
    return 0;
}


int tcp_client::send_message(const char* message, int length, unsigned int scheduler_id, unsigned int task_id)  
{
    char str[4];
    str[0] = (length + 1) % 256;
    str[1] = (length + 1) / 256 % 256;
	str[2] = (length + 1) / 256 / 256 % 256;
    str[3] = CRC8_Table(str, message, length);
    str[4] = task_id & 0xFF;            // get task_id lower 8 bit
    str[5] = scheduler_id & 0xFF;       // get scheduler_id lower 8 bit
    str[6] = (task_id>>8) & 0xFF;       // get scheduler_id 8-16 bit
    str[7] = (task_id>>16) & 0xFF;      // get scheduler_id 16-24 bit

    if( send(sockfd, str, 3, 0) < 0)  
    {
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
        return -1;
    }
    while (length > 0)
    {
        if( send(sockfd, message, length > MAXLINE ? MAXLINE : length, 0) < 0)  
        {  
            printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
            return -1;
        }
        message += MAXLINE;
        length -= MAXLINE;
		// printf("test: %d\n", length);
    }
    if( send(sockfd, str + 3, 1, 0) < 0)  
    {  
		printf("test3\n");
        printf("send msg error: %s(errno: %d)\n", strerror(errno), errno);  
        return -1;
    }
    return 0;
}

int tcp_client::wait_for_response(char* resp)
{
    int rec_len = 0;
    if((rec_len = recv(sockfd, resp, MAXLINE,0)) == -1) {  
       perror("recv error");  
       return -1;
    }
    // printf("response len: %d\n", rec_len);  
    resp[rec_len]  = '\0';  
    // printf("Received : %s ", resp);  
    return rec_len;
}

int tcp_client::wait_for_response_cloud(char* resp)
{
    int rec_len = 0;
    if((rec_len = recv(sockfd, resp, MAXLINE,0)) == -1) {  
       perror("recv error");  
       return -1;
    }  
    return rec_len;
}

char tcp_client::CRC8_Table(char *p, const char *q, int counter)  
{  
    unsigned char crc8 = 0;
    int i;
    
    crc8 = (unsigned char)CRC8Table[(unsigned char)(crc8^(unsigned char)*p)];
    // printf("%d %02x\n", 0, crc8);
    crc8 = (unsigned char)CRC8Table[(unsigned char)(crc8^(unsigned char)*(p+1))];
    // printf("%d %02x\n", 1, crc8);
	crc8 = (unsigned char)CRC8Table[(unsigned char)(crc8^(unsigned char)*(p+2))];

    for( i = 0; i < counter; i++){  
        crc8 = (unsigned char)CRC8Table[(unsigned char)(crc8^(unsigned char)*(q+i))];
        // printf("%d %02x\n", i + 2, crc8);
    }  
    return crc8;
  
}

char tcp_client::CRC8_Table(char *p, const char *q, int p_counter, int q_counter)
{
    unsigned char crc8 = 0;
    int i = 0;

    for (i = 0; i < p_counter; i++)
    {
        crc8 = (unsigned char)CRC8Table[(unsigned char)(crc8^(unsigned char)*(p+i))];
    }
    for (i = 0; i < q_counter; i++)
    {
        crc8 = (unsigned char)CRC8Table[(unsigned char)(crc8^(unsigned char)*(q+i))];
    }
}
