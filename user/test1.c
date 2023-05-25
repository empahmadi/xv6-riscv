//
// Created by empahmadi on 5/23/2
//
#include "../kernel/types.h"
#include "../kernel/stat.h"
#include "user.h"


int main() {
    int status = 0;
    int avgTurnaround[2];
    int avgWaiting[2];
    int avgCBT[2];

    for (int count = 0; count<2;count++){
        switch_scheduler((count + 1) % 2); // set the algorithm
        int children = 0;
        // for 32 process:
        // creation time _ termination time _ running time _ ready time _ sleeping time.
        int times[32][5];
        for (int i = 0; i <32 ; i++) {//each child
            int pid = fork();
            if ( pid == 0) {
                int array1[10000];
                int array2[10000];
                int res[10000];

                for (int i = 0; i < 10000; i++) {
                    array1[i] = 100 + (i % 100);
                    array2[i] = 100 + (i % 100);
                }
                for (int p = 0;p<10000;p++){
                    res[p] = array1[p] + array2[p];
                    array1[p+1] = res[p] % 200;
                    if(p % 100 == 0)
                        printf("...\n");
                }
                exit(0);
            } else {
                children = children + 1;
            }
        }

        int sumTurnaround = 0;
        int sumWaiting = 0;
        int sumBurst = 0;

        for (int i = 0; i <32 ; i++) {
            int pid = wait_pid(&status, times[i]);
            int waitingTime = times[i][4] + times[i][3];
            int sleeping = times[i][4];
            int cpuBurst = times[i][2];
            int turnAround = times[i][1] - times[i][0];
            sumTurnaround += turnAround;
            sumWaiting += waitingTime;
            sumBurst += cpuBurst;
            printf("pid: %d, ", pid);
            printf("turnaround time = %d, ", turnAround);
            printf("waiting time = %d, ", waitingTime);
            printf("sleeping time = %d, ", sleeping);
            printf("cpu burst = %d, ", cpuBurst);
            printf("\n");
        }
        avgTurnaround[count] =  sumTurnaround / 32;
        avgWaiting[count] =  sumWaiting / 32;
        avgCBT[count] = sumBurst / 32;
    }
    printf("\naverage turnaround times: %d  --  %d\n",avgTurnaround[0],avgTurnaround[1]);
    printf("average waiting times: %d  --  %d\n",avgWaiting[0],avgWaiting[1]);
    printf("average cpu burst times: %d  --  %d\n",avgCBT[0],avgCBT[1]);
    return 0;
}
