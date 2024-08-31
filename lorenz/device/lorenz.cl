#define SIGMA 10.0
#define RHO 28.0
#define BETA 8.0 / 3.0

typedef struct {
    double x, y, z;
} Point;

__kernel void lorenz(__global Point * p, const double dt, const  int steps) {

    for (int i = 0; i < steps; i++) {
        
                double x_dot = SIGMA * (p[i].y - p[i].x);
                double y_dot = p[i].x * (RHO - p[i].z) - p[i].y;
                double z_dot = p[i].x * p[i].y - BETA * p[i].z;

                p[i+1].x =p[i].x+ x_dot * dt;
                p[i+1].y =p[i].y + y_dot * dt;
                p[i+1].z = p[i].z +z_dot * dt;
            }
}


