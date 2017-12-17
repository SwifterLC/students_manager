#ifndef		__P_TRANS_H__
#define		__P_TRANS_H__
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
 * 
 */
// 服务器处理业务
void p_s_trans(int fd);

// 客户端处理业务
void p_c_trans(int fd);

#endif		//__P_TRANS_H__
