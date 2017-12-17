#include "p_sndrcv.h"

//给远端发送消息，fd 通讯描述符,code 发送状态码,body 消息体 
void p_send(int fd,int code,const char *body){
    char buf[512] = {0}; 
    sprintf(buf,"%d\n%s",code,body);
    write(fd,buf,strlen(buf));
}
//给远端接收消息，fd 通讯描述符,code 发送状态码,body 消息体 
void p_recv(int fd,int *code,char *body){
    char buf[512] = {0};
    read(fd,buf,512);
    //提取状态码
    sscanf(buf,"%d",code);
    //提取消息体
    char *p = buf;
    while(*p++ != '\n');
    strcpy(body,p);
}

