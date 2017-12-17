#include "p_trans.h"
#include <p_file.h>
#include <stdio.h>
#include <string.h>
#include "clc/p_stu_link.h"
#include "clc/p_sndrcv.h" 
//清理缓存区
static void clean(void){
	scanf("%*[^\n]");
	scanf("%*c");
}
static void m_search(int fd){
	printf("输入1查询全部学生信息,输入2按学号查询,输入3返回");
	int num;
	while (1){
		scanf("%d",&num);
		clean();
		if (num > 0 && num < 4)
			break;
		printf("请重新输入:");
	}
	char buf[1024] = {0};
	int size;
	if (num == 1){
		p_send_once(fd,2000,"");
	}else if (num == 2){
		printf("请输入要查询学生学号:");
		while(1){
			scanf("%d",&num);
			clean();
			if (num > 0)
				break;
			printf("输入学号格式错误,请重新输入:");
		}
		char str[10] = {0};
		sprintf(str,"%d",num);
		p_send_once(fd,2001,str);
	}
	/*size = read(fd,buf,1024);
	if (2 == size)
		return;
	write(1,buf + 2,size - 2);
	if (size < 1024)
		return;
	while (size = read(fd,buf,1024))
		write(1,buf,size);*/
        p_recv_output(fd,1);
}
static void m_add(int fd){
	int num;
        int code;
	char str[10] = {0};
	char ch;
	char buf[512] = {0};
	while (1){
		printf("请输入要增加的学生学号:");
		scanf("%d",&num);
		clean();
		if (num < 0)
			continue;
		sprintf(buf,"%d",num);
		p_send_once(fd,3000,buf);
		//read(fd,&ch,1);
                memset(buf,0,512);
                p_recv_once(fd,&code,buf);
		if (!strcmp(buf,"1"))
			printf("要增加的学生已经存在\n");
		else
			break;
	}
	node_t node = {0};
	node.num = num;
	printf("请输入学生姓名,年龄,成绩:");
	scanf("%s%d%hd",node.student.name,&node.student.age,&node.student.score);
	sprintf(buf,"%d %s %d %hd",num,node.student.name,node.student.age,node.student.score);
	p_send_once(fd,3001,buf);
}
static void m_update(int fd){
	int num;
        int code;
        char buf[512] = {0};
	char str[10] = {0};
	char ch;
	printf("请输入要修改的学生学号(0返回):");
	while (1){
		while (1){
			scanf("%d",&num);
			clean();
			if (num >= 0)
				break;
			printf("学号输入错误,请重新输入:");
		}
		if (!num)
			break;
		sprintf(str,"%d",num);
		p_send_once(fd,4000,str);
		//read(fd,&ch,1);
                memset(buf,0,512);
                p_recv_once(fd,&code,buf);
		if (strcmp(buf,"1"))
			printf("要修改的学生不存在，请重新输入(0返回)");
		else{
			char buf[512] = {0};
			node_t node = {0};
			node.num = num;
			printf("请输入学生姓名,年龄,成绩:");
			scanf("%s%d%hd",node.student.name,&node.student.age,&node.student.score);
			sprintf(buf,"%d %s %d %hd",num,node.student.name,node.student.age,node.student.score);
			p_send_once(fd,4001,buf);
			break;
		}
	}
}
static void m_delete(int fd){
	int num;
        int code;
        char buf[512];
	char str[10] = {0};
	printf("请输入要删除学生的学号:");
	while (1){
	scanf("%d",&num);
	clean();
	if (num > 0)
		break;
	printf("输入学生学号格式错误,请重新输入:");
	}
	sprintf(str,"%d",num);
	p_send_once(fd,5000,str);
        p_recv_once(fd,&code,buf);
        printf("删除完毕\n");
}
static void m_download(int fd){
	p_send_once(fd,6000,"");
	int file_d = open("student.txt",O_WRONLY|O_CREAT|O_TRUNC,0664);
	if (-1 == file_d){
		perror("open");
		exit(-1);
	}
        p_recv_output(fd,file_d);
        close(file_d);
}
static int op(int fd){
	int num = 0;
	while(1){
		scanf("%d",&num);
		clean();
		if (num > 0 && num < 7)
			break;
		printf("请重新输入:");
	}
	switch(num){
		case 6:
			p_send_once(fd,1000,"");
			return 0;
		case 1:
			m_search(fd);
			break;
		case 2:
			m_add(fd);
			break;
		case 3:
			m_update(fd);
			break;
		case 4:
			m_delete(fd);
			break;
		case 5:
			m_download(fd);
			break;
	}
	return 1;
}
// 客户端处理业务
void p_c_trans(int fd){
	//欢迎
	printf("欢迎使用学生管理系统\n");
	while(1){
		printf("输入(1查询,2增加,3修改,4删除,5下载,6退出)");
		if (!op(fd))
			break;
	}
	printf("欢迎下次使用\n");
	close(fd);
}

