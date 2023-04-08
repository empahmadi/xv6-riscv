//
// Created by empahmadi on 4/5/23.
//

#include "../include/structs.h"
#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"


int main() {
    struct sys_info info;
    printf("addr = %d\n", &info);

    sysinfo(&info);

    printf("addr = %d\n", &info);
    printf("uptime: %d\n", info.uptime);
    printf("total ram: %d\n", info.totalram);
    printf("free ram: %d\n", info.freeram);
    printf("processes: %d\n", info.procs);

    return 0;
}
