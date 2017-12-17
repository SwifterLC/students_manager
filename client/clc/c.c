#include <stdio.h>
#include <p_net.h>
#include <signal.h>
#include "../p_trans.h"
int main(){
    int fd = p_connect(7878,"176.201.118.18");
    if (-1 == fd) {
        perror("p_connect");
        return -1;
    }
    p_c_trans(fd);
    return 0;
}
