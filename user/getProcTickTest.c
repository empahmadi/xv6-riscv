//
// Created by empahmadi on 4/4/23.
//
#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"

int main(void){
    printf("This is the start of test for getProcTick system call !!!\n");
    int x = 998 + 809;
    printf("this is the result: %d\n", x);
    getProcTick(getpid());
    printf("This is the end of test for getProcTick system call !!!\n");
    exit(0);
}
