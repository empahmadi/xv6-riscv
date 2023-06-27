//
// Created by empahmadi on 6/27/23.
//
#include "../include/structs.h"
#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"


int main() {
    struct sys_info info;
    sysinfo(&info);
    printf("total ram (before): %d\n", info.totalram);
    printf("free ram (before): %d\n", info.freeram);
    int new_size = (int)((2 * info.freeram) / 3);
    printf("requested size is: %d\n", new_size);
    sbrk(new_size);
    sysinfo(&info);
    printf("total ram (after): %d\n", info.totalram);
    printf("free ram (after): %d\n", info.freeram);

    int pid = fork();
    if (pid == 0){
        printf("hi it's child\n");
        exit(0);
    } else if (pid > 0){
        wait(0);
        printf("hi it's parent\n");
        exit(0);
    } else {
        printf("hi it's error :(\n");
    }
    printf("the end of test 3\n");
    return 0;
}

