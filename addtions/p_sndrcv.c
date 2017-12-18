#include "p_sndrcv.h"

/*
   消息发送
   */
//短消息一次发送完毕
void p_send_once(int fd,\
        		 int code,\
        		 const char *body 	/*本程序不会传递不可读数据*/){
    char buf[10];
    sprintf(buf,"%d\n",code);
    write(fd,buf,strlen(buf));
    write(fd,body,strlen(body));
    write(fd,"\r\n\r\n",4);	
}
//长消息持续发送
void p_send(int fd,\
        	int code,\
        	const char *body,\
        	int state){

    char buf[10];
    switch(state){
        case P_SND_STATE_HEAD:
            sprintf(buf,"%d\n",code);
            write(fd,buf,strlen(buf));
            write(fd,body,strlen(body));
            break;
        case P_SND_STATE_MIDDLE:
            write(fd,body,strlen(body));
            break;
        case P_SND_STATE_TAIL:
            write(fd,body,strlen(body));
            write(fd,"\r\n\r\n",4);
            break;
    }
}
//从远端接收消息,输出到文件或者屏幕上
void p_recv_output(int fd,int out){
    char buf[512] = {0};
    char cache[512] = {0};
    char *p;
    int size;
    char ch;
    //去除状态码和\n；
    while(read(fd,&ch,1),ch != '\n');
    while(1){
        read(fd,&ch,1);
        buf[strlen(buf)] = ch;
        if (strlen(buf) == 3)
            break;
    }
    while(1){
        size = read(fd,cache,512-4);
        cache[size] = '\0';
        strcat(buf,cache);
        if (p = strstr(buf,"\r\n\r\n")){
            write(out,buf,p - buf);
            return;
        }
        write(out,buf,strlen(buf)-3);
        strcpy(buf,buf + strlen(buf) - 3);
    }
}
void p_recv_once(int fd,int *code,char *body){
    char cache[512] = {0};
    char *p = NULL;
    char ch;
    char ch_buf[10] = {0};	/*状态码不会超过9位,存放状态码*/
    while(1){
        read(fd,&ch,1);
        if (ch == '\n')
            break;
        ch_buf[strlen(ch_buf)] = ch;
    }
    *code = atoi(ch_buf);	/*提取状态码*/
    strcpy(body,"");	/*字符串清空*/
    int ret;
    while(1){
        ret = read(fd,cache,511);
        cache[ret] = '\0';
        strcat(body,cache);	/*短消息不会超过512*/
        p = strstr(body,"\r\n\r\n");
        if (p/*获取结束*/){
            *p = '\0';
            break;
        }
    }
}
