/*link with -lp_net*/
  /*TCP/IP v4*/
  #ifndef     __P_NET_H__
  #define     __P_NET_H__
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <unistd.h>
  #include <sys/wait.h>
  #include <string.h>
  #include <stdlib.h>

  typedef struct sockaddr SA;
  typedef struct sockaddr_in  SA4;

  //·监听
  int p_listen(int port,int backlog);
  //开启多进程服务
  void p_start_server(int port,\
      int backlog,\
      void (*p_trans)(int),\
     void (*p_showClient)(SA4 *));
  //客户端连接服务器
  int p_connect(int port,const char *ip);
#endif      //__P_NET_H__           