//
// Created by empahmadi on 5/23/2
//

#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"


int main() {
    switch_scheduler(1);
    int status = 0;
    int p = fork();
    if(p == 0){
        printf("child started\n");
        int i = 0;
        int y = 0;
        printf("loading ");
        for(int x = 0; x < 1000000; x++){
            i += (x %10);
            y += i + x % 100;
            y++;
            i++;
            if(x % 10000 == 0){
                printf("...\n");
            }
        }
        printf("\nchild %d ended\n", getpid());
        exit(0);
    }else {
        int pid ;
        int info[2][5];
        pid = wait_pid(&status, info[0]);
        printf("child pid = %d\n", pid);
        printf("%d - %d - %d - %d - %d\n", info[0][0], info[0][1], info[0][2], info[0][3], info[0][4]);
    }
    return 0;
}


