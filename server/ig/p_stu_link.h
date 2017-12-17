#ifndef		__P_STU_LINK__
#define		__P_STU_LINK__

#include <stdlib.h>		/*free(void *p) malloc()*/
#include <string.h>		/*memcpy(,,)*/
#include <pthread.h>
/*
 *	学生信息
 */
typedef struct student{
    char name[24];	/*姓名*/
    int age;
    short score;	/*成绩*/
} student_t;

//学生信息管理系统，完成学生信息的增c 删d 查r 改u
//查询学生信息(全部信息显示，用户指定学生编号显示)

typedef struct node{
    int num;	/*对应学生学号,作为学生的唯一标识*/
    student_t student;	/*学生信息*/
    struct node *p_next;
} node_t;

typedef struct link{
    node_t head	/*头节点*/,tail	/*尾节点*/;
    pthread_rwlock_t rwlock;	/*读写锁*/
    node_t *p_cur;
} link_t;

extern link_t lk;

/*链表初始化*/
void link_init(link_t *p_link);

/*链表析构*/
void link_deinit(link_t *p_link);

/*链表有效节点个数*/
int link_size(const link_t *p_link);

/*链表是否为空*/
int link_empty(const link_t *p_link);

/*链表是否满*/
int link_full(const link_t *p_link);

/*查询链表制定编号的节点,返回值为空表示未找到*/
node_t *link_search(link_t *p_link,int num);

/*链表插入数据,程序在插入前以进行判定,插定点必唯一，函数仍然进行了判断*/
int link_insert(link_t *p_link,const node_t *p_node);

/*链表删除节点*/
int link_delete(link_t *p_link,int num);

/*链表开始遍历*/
void link_began(link_t *p_link);

/*链表遍历*/
const node_t *link_next(link_t *p_link);

#endif		//__P_STU_LINK__
