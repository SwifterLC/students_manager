#include <stdio.h>
#include "../addtions/p_net.h"
#include "p_trans.h"
#include <signal.h>
int main(){
    int fd = p_connect(7878,"176.201.118.18");
    if (-1 == fd) {
        perror("p_connect");
        return -1;
    }
    p_c_trans(fd);
    return 0;
}
