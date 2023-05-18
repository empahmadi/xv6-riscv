//
// Created by empahmadi on 4/8/23.
//

#ifndef XV6_RISCV_STRUCTS_H
#define XV6_RISCV_STRUCTS_H

#define MAXNAMELEN 64

struct sys_info {
    long uptime;
    unsigned long totalram;
    unsigned long freeram;
    unsigned short procs;
};


enum scheduler_algorithm_mode {DEFAULT, FCFS};


#endif //XV6_RISCV_STRUCTS_H
