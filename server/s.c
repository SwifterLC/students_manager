#include <stdio.h>
#include "../addtions/p_net.h"
#include <stdlib.h>
#include <signal.h>
#include <p_file.h>
#include "p_trans.h"
#include "../addtions/p_stu_link.h"

link_t lk;

void *trans(void *arg){
	p_s_trans(*(int *)arg);
        free(arg);
	return (void *)0;
}
void fd_close(int num){
    pthread_exit(NULL);
}
void doit(void){
	link_deinit(&lk);
}
//读取文件节点，插入链表
void init_from_file(void){
	int fd = open("student.bin",O_RDONLY|O_CREAT,0664);
	if (-1 == fd){
		perror("open");
		exit(-1);
	}
	node_t node;
	while(read(fd,&node,sizeof(node)))
		link_insert(&lk,&node);
	close(fd);
}
int main(void){
	int fd,conn_fd;
	pthread_t tid;
	atexit(doit);
	link_init(&lk);
	init_from_file();
        signal(SIGPIPE,fd_close);
	fd = p_listen(7878,12);
	if (-1 == fd)
		exit(-1);
	while (1){
		conn_fd = accept(fd,NULL,NULL);
		if (-1 == conn_fd){
			perror("accept");
			return -1;
		}
                int *p = malloc(sizeof(int));
                if (!p)
                    exit(-1);
                *p = conn_fd;
		pthread_create(&tid,NULL,trans,(void *)p);
		pthread_detach(tid);
	}
}
