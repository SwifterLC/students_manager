#include	"p_stu_link.h"

/*链表初始化*/
void link_init(link_t *p_link){
    p_link->head.p_next = &p_link->tail;
    p_link->tail.p_next = NULL;
    p_link->p_cur = NULL;
    pthread_rwlock_init(&p_link->rwlock,NULL);
}
/*链表析构*/
void link_deinit(link_t *p_link){
    node_t *p_node = NULL;
    while (p_link->head.p_next != &p_link->tail){
        p_node = p_link->head.p_next;
        p_link->head.p_next = p_node->p_next;
        free(p_node);
        p_node = NULL;
    }
    p_link->p_cur = NULL;
    pthread_rwlock_destroy(&p_link->rwlock);
}
/*链表有效节点个数*/
int link_size(const link_t *p_link){
    int count = 0;
    const node_t *p_node = NULL;
    const node_t *p_first,*p_mid,*p_last;
    for (p_node = &p_link->head;p_node != &p_link->tail;p_node = p_node->p_next){
        p_first = p_node;
        p_mid = p_first->p_next;
        p_last = p_mid->p_next;
        if (p_mid != &p_link->tail)
            count++;
    }
    return count;
}
/*链表是否为空*/
int link_empty(const link_t *p_link){
    return p_link->head.p_next == &p_link->tail;
}
/*链表是否满*/
int link_full(const link_t *p_link){
    return 0;
}

/*查询链表制定编号的节点,返回值为空表示未找到*/
node_t *link_search(link_t *p_link,int num){
    node_t *p_node = NULL;
    node_t *p_first,*p_mid,*p_last;
    for (p_node = &p_link->head;p_node != &p_link->tail;p_node = p_node->p_next){
        p_first = p_node;
        p_mid = p_first->p_next;
        p_last = p_mid->p_next;
        if (p_mid != &p_link->tail)
            if (p_mid->num == num)
                return p_mid;
    }
    return NULL;
}
/*链表插入数据,程序在插入前以进行判定,插定点必唯一，函数仍然进行了判断*/
int link_insert(link_t *p_link,const node_t *p_node){
    p_link->p_cur = NULL;	/*插入时应结束便利*/
    if (link_search(p_link,p_node->num))
        return 0;
    node_t *p_tmp = (node_t *)malloc(sizeof(node_t));
    if (NULL == p_tmp)
        return 0;
    memcpy(p_tmp,p_node,sizeof(node_t));
    node_t *p_first,*p_mid,*p_last,*p_tmp1;
    for (p_tmp1 = &p_link->head;p_tmp1 != &p_link->tail;p_tmp1 = p_tmp1->p_next){
        p_first = p_tmp1;
        p_mid = p_first->p_next;
        p_last = p_mid->p_next;
        if (p_mid == &p_link->tail || p_mid->num > p_tmp->num){
            p_first->p_next = p_tmp;
            p_tmp->p_next = p_mid;
        }
    }
    return 1;
}
/*链表删除节点*/
int link_delete(link_t *p_link,int num){
    p_link->p_cur = NULL;	/*删除节点时，结束遍历*/
    if (!link_search(p_link,num))
        return 0;
    node_t *p_node = NULL;
    node_t *p_first,*p_mid,*p_last;
    for (p_node = &p_link->head;p_node != &p_link->tail;p_node = p_node->p_next){
        p_first = p_node;
        p_mid = p_first->p_next;
        p_last = p_mid->p_next;
        if (p_mid != &p_link->tail && p_mid->num == num){
            p_first->p_next = p_last;
            free(p_mid);
            p_mid = NULL;
        }
    }
    return 1;
}

/*链表开始遍历*/
void link_began(link_t *p_link){
    p_link->p_cur = &p_link->head;
}

/*链表遍历*/
const node_t *link_next(link_t *p_link){
    if (!p_link->p_cur)
        return NULL;
    p_link->p_cur = p_link->p_cur->p_next;
    if (p_link->p_cur != &p_link->tail)
        return (const node_t *)p_link->p_cur;
    else
        p_link->p_cur = NULL;
    return NULL;
}
