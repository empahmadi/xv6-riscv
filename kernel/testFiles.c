#include "types.h"
#include "stat.h"
#include "user.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    int array1[10000];
    int array2[10000];

    for (int i = 0; i < 10000; i++) {
        array1[i] = rand();
        array2[i] = rand();
    }
    int avgTurnaround[2];
    int avgWaiting[2];
    int avgCBT[2];

    for (int count = 0; count<2;count++){
        changePolicy(count); // set the algorithm
        int children = 0;
        int retime;
        int rutime;
        int stime;
        int res[10000];
        for (int i = 0; i <32 ; i++) {//each child
            int pid = fork();
            if ( pid == 0) {
                for (int p = 0;p<100000;p++){
                    res[p] = array1[p] + array2[p];
                }
                sleep(2000);
                exit();
            }
            else children= children +1;
        }

        int sumTurnaround = 0;
        int sumWaiting = 0;
        int sumBurst = 0;

        for (int i = 0; i <32 ; i++) {
            int pid = wait2(&retime, &rutime, &stime);
            int waitingTime = stime;
            int sleeping = retime;
            int cpuBurst = rutime;
            int turnAround = waitingTime + sleeping+cpuBurst;
            sumTurnaround += turnAround;
            sumWaiting += waitingTime;
            sumBurst += cpuBurst;
            printf(1, "pid: %d, ", pid);
            printf(1, "turnaround time = %d, ", turnAround);
            printf(1, "waiting time = %d, ", waitingTime);
            printf(1, "sleeping time = %d, ", sleeping);
            printf(1, "cpu burst = %d, ", cpuBurst);
            printf(1,"\n");
        }   
        avgTurnaround[count] =  sumTurnaround / 32;
        avgWaiting[count] =  sumWaiting / 32;
        avgCBT[count] = sumBurst / 32;
        exit();

        }
    printf(1, "\naverage turnaround times: %d  --  %d\n",avgTurnaround[0],avgTurnaround[1]);
    printf(1, "average waiting times: %d  --  %d\n",avgWaiting[0],avgWaiting[1]);
    printf(1, "average cpu burst times: %d  --  %d\n",avgCBT[0],avgCBT[1]);

    
}