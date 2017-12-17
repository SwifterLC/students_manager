#include <stdlib.h>
#include <p_file.h>
#include "ig/p_stu_link.h"
#include "p_trans.h"
#include "ig/p_sndrcv.h"
static void syn(void){
	link_began(&lk);
	int fd = open("ig/student.bin",O_WRONLY|O_CREAT|O_TRUNC,0664);
	if (-1 == fd){
		perror("open");
		exit(-1);
	}
	const node_t *p_node;
	while (p_node = link_next(&lk))
		write(fd,p_node,sizeof(node_t));
	close(fd);
}
static void m_search_all(fd){
	pthread_rwlock_rdlock(&lk.rwlock);
        //sleep(1);
	link_began(&lk);
	const node_t* p_node = NULL;
	char buf[512] = {0};
	//write(fd,"0\n",2);	//状态码的占位
        p_send(fd,0,"",0);
	while(p_node = link_next(&lk)){
		sprintf(buf,"学号:%4d,姓名:%10s,年龄:%2d,成绩:%3hd\n",\
				p_node->num,\
				p_node->student.name,\
				p_node->student.age,\
				p_node->student.score
			   );
		printf("%s",buf);
		//write(fd,buf,strlen(buf));
                p_send(fd,0,buf,1);
	}
        p_send(fd,0,"",2);
	pthread_rwlock_unlock(&lk.rwlock);
}
static void m_serch_num(int fd,int num){
	pthread_rwlock_rdlock(&lk.rwlock);
	char buf[512] = {0};
	node_t *p_node = link_search(&lk,num);
	if (p_node){
	sprintf(buf,"学号:%4d,姓名:%10s,年龄:%2d,成绩:%3hd\n",\
			p_node->num,\
			p_node->student.name,\
			p_node->student.age,\
			p_node->student.score
		   );
	}
	p_send_once(fd,0,buf);
	pthread_rwlock_unlock(&lk.rwlock);
}
static void m_exist(int fd,int num){
	pthread_rwlock_rdlock(&lk.rwlock);
	if (link_search(&lk,num))
		//write(fd,"1",1);
                p_send_once(fd,1234,"1");
	else
		p_send_once(fd,1234,"0");
	pthread_rwlock_unlock(&lk.rwlock);
}
static void m_insert(int fd,const char *buf){
	pthread_rwlock_wrlock(&lk.rwlock);
        //sleep(1);
	node_t node;
	sscanf(buf,"%d%s%d%hd",&node.num,node.student.name,&node.student.age,&node.student.score);
	link_insert(&lk,&node);
	syn();
	pthread_rwlock_unlock(&lk.rwlock);
}
static void m_update(int fd,const char *buf){
	pthread_rwlock_wrlock(&lk.rwlock);
	int num;
	sscanf(buf,"%d",&num);
	node_t *p_node = link_search(&lk,num);
        if (!p_node){
            printf("文件已经不存在------------\n");
            return;
        }
	sscanf(buf,"%d%s%d%hd",&p_node->num,p_node->student.name,&p_node->student.age,&p_node->student.score);
	syn();
	pthread_rwlock_unlock(&lk.rwlock);
}
static void m_delete(int fd,const char *buf){
	pthread_rwlock_wrlock(&lk.rwlock);
	link_delete(&lk,atoi(buf));
	syn();
	//write(fd,"0\n",2);	//占位，删除结束的标志
        p_send_once(fd,0,"");
	pthread_rwlock_unlock(&lk.rwlock);
}
//状态码处理
static void m_trans(int fd,int code,char *buf){
	switch(code){
		case 6000:
		case 2000:     /*全部查找*/
			m_search_all(fd);
			break;
		case 2001:     /*按学号查找*/
			m_serch_num(fd,atoi(buf));
			break;
		case 3000:		/*查看是否存在*/
		case 4000:
			m_exist(fd,atoi(buf));
			break;
		case 3001:
			m_insert(fd,buf);
			break;
		case 4001:
			m_update(fd,buf);
			break;
		case 5000:
			m_delete(fd,buf);
	}
}

// 服务器处理业务
void p_s_trans(int fd){
	while (1){
		int code;
		char buf[512] = {0};
		p_recv_once(fd,&code,buf);	//buf------>  查询2000+ 增加3000+ 修改4000+ 删除5000+ 下载6000+ 退出1000
                printf("code = %d\tbuf = %s\n",code,buf);
		if (code == 1000)
			break;
		m_trans(fd,code,buf);
	}
	close(fd);
}
