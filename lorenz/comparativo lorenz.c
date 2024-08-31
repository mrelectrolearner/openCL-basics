#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#define SIGMA 10.0
#define RHO 28.0
#define BETA 8.0 / 3.0

typedef struct {
    double x, y, z;
} Point;

void lorenzSystem(Point *p, double dt) {
    double x_dot = SIGMA * (p->y - p->x);
    double y_dot = p->x * (RHO - p->z) - p->y;
    double z_dot = p->x * p->y - BETA * p->z;

    (p+1)->x =p->x+ x_dot * dt;
    (p+1)->y =p->y + y_dot * dt;
    (p+1)->z = p->z +z_dot * dt;
}




int main(){
    struct timeval start, end;
     Point initialPoint = {1.0, 0.0, 20.0}; // Initial conditions
    double dt = 0.01; // Time step
    int steps = 1000000; // Number of steps

    Point * listPoint;

    listPoint=malloc(sizeof(Point)*steps);
    listPoint[0]=initialPoint;

    printf("Time\tX\tY\tZ\n");

    
    gettimeofday(&start,NULL);
    //sleep(27);
    
    for (int i = 0; i <= steps; i++) {
       
            lorenzSystem(listPoint+i , dt);
        }
   
    gettimeofday(&end,NULL);
     /* for (int i = 0; i <= steps; i++) {
            printf("%d\t%lf\t%lf\t%lf\n", i, (listPoint+i)->x, (listPoint+i)->y, (listPoint+i)->z);
        } */

    long double time_taken;
     time_taken = (end.tv_sec - start.tv_sec) * 1e6;
    time_taken = (time_taken + (end.tv_usec - 
                              start.tv_usec)) * 1e-6;
    printf("f time= %Lf sec \n", time_taken);

    return 0;
}
