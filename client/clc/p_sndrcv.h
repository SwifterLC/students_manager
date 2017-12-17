#ifndef		__P_SNDRCV_H__
#define		__P_SNDRCV_H__

#include <stdio.h>
#include <string.h>
#include <unistd.h>

enum {P_SND_STATE_HEAD,P_SND_STATE_MIDDLE,P_SND_STATE_TAIL};
/*
   消息发送
   */
//短消息一次发送完毕
void p_send_once(int fd,\
                 int code,\
                 const char *body/*本程序不会传递不可读数据*/);
//长消息持续发送
void p_send(int fd,\
            int code /*/*state非零时无效，可随便填*/,\
            const char *body,\
            int state/*P_SND_STATE_HEAD,P_SND_STATE_MIDDLE,P_SND_STATE_TAIL*/);
//从远端接收消息,输出到文件或者屏幕上
void p_recv_output(int fd,int out);

void p_recv_once(int fd,int *code,char *body);

#endif		//__P_SNDRCV_H__
