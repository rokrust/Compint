#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#define N_RULES 4
#define N_INPUT_VARIABLES 2
#define WHEEL_DISTANCE 0.16


typedef struct{
    double left_wheel_speed;
    double right_wheel_speed;
} Wheel_speed;

typedef struct{
    double x;
    double y;

} Position;

typedef struct{
    double angle;
    Position pos;
    Wheel_speed wheel_speed;

} Robot_state;

typedef struct{
    Robot_state state[1000];

} Data;

typedef struct{
    Robot_state ref_state[1000];
    Robot_state current_state[1000];
} Old_data;

typedef enum{
    SMALL = 0,
    LARGE
} ERR_EXP;

double de_gain[N_RULES] = {0.25, 0.25, 0.50, 0.50};
double thetae_gain[N_RULES] = {0.12, 0.25, 0.12, 0.25};


double distance(Position from, Position to);
double min(double a, double b);
double max(double a, double b);
double fuzzy_de_membership_func(double d_error, ERR_EXP error_expression);
double fuzzy_thetae_membership_func(double h_error, ERR_EXP error_expression);
void fuzzy_membership_function_output(double de_error, double dh_error, double mu[N_RULES][N_INPUT_VARIABLES]);
Wheel_speed fuzzy_determine_rule_output(double de, double thetae, int rule_nbr);
double fuzzy_robot_velocity(Robot_state current_state);
double fuzzy_robot_angular_velocity(Robot_state current_state);
Wheel_speed fuzzy_determine_wheel_speeds(Robot_state ref_state, Robot_state current_state);


int fuzzy_read_data(FILE* fp, Data* ref_data){
    char input_string[100];

    int i = 0;

    while(fgets(input_string, 100, fp) != NULL){
        sscanf(input_string, "%lf,%lf,%lf\n",
               &(ref_data->state[i].pos.x), &(ref_data->state[i].pos.y), &(ref_data->state[i].angle));

        i++;
    }

    ref_data->state[0].wheel_speed.left_wheel_speed = 0;
    ref_data->state[0].wheel_speed.right_wheel_speed = 0;


    return i + 1;
}


Robot_state fuzzy_calculate_next_state(Robot_state ref_state, Robot_state current_state){
    Robot_state next_state;

    double robot_mean_velocity = fuzzy_robot_velocity(current_state);
    double robot_angular_velocity = fuzzy_robot_angular_velocity(current_state);
    //printf("%f,%f\n", robot_mean_velocity, robot_angular_velocity);

    next_state.angle = current_state.angle + robot_angular_velocity;
    next_state.pos.x = current_state.pos.x + robot_mean_velocity*cos(current_state.angle);
    next_state.pos.y = current_state.pos.y + robot_mean_velocity*sin(current_state.angle);
    next_state.wheel_speed = fuzzy_determine_wheel_speeds(ref_state, next_state);

    return next_state;
}

double fuzzy_robot_velocity(Robot_state current_state){
    return ((current_state.wheel_speed.left_wheel_speed + current_state.wheel_speed.right_wheel_speed) / 2);
}

double fuzzy_robot_angular_velocity(Robot_state current_state){
    return ((current_state.wheel_speed.left_wheel_speed - current_state.wheel_speed.right_wheel_speed) / WHEEL_DISTANCE);
}


Wheel_speed fuzzy_determine_wheel_speeds(Robot_state ref_state, Robot_state current_state){
    Wheel_speed wheel_speed;
    wheel_speed.left_wheel_speed = 0;
    wheel_speed.right_wheel_speed = 0;

    //printf("Ref: %f,%f\nCur: %f,%f\n", ref_state.pos.x, ref_state.pos.y, current_state.pos.x, current_state.pos.y);
    double de_error = distance(ref_state.pos, current_state.pos);
    double dh_error = (ref_state.angle - current_state.angle);

    double mu[N_RULES][N_INPUT_VARIABLES];
    fuzzy_membership_function_output(de_error, fabs(dh_error), mu);

    double w_i_sum = 0;
    for(int i = 0; i < N_RULES; i++){
        double w_i = min(mu[i][0], mu[i][1]);
        w_i_sum += w_i;

        Wheel_speed rule_speed_output = fuzzy_determine_rule_output(de_error, dh_error, i);

        wheel_speed.left_wheel_speed += (rule_speed_output.left_wheel_speed*w_i);
        wheel_speed.right_wheel_speed += (rule_speed_output.right_wheel_speed*w_i);
    }

    wheel_speed.left_wheel_speed /= w_i_sum;
    wheel_speed.right_wheel_speed /= w_i_sum;

    //printf("Wheel: %f,%f\n\n", wheel_speed.left_wheel_speed, wheel_speed.right_wheel_speed);

    return wheel_speed;
}


Wheel_speed fuzzy_determine_rule_output(double de, double thetae, int rule_nbr){
    Wheel_speed u_i;

    u_i.left_wheel_speed = de_gain[rule_nbr]*de + thetae_gain[rule_nbr]*thetae;
    u_i.right_wheel_speed = de_gain[rule_nbr]*de - thetae_gain[rule_nbr]*thetae;

    return u_i;
}



void fuzzy_print_wheel_speeds(Wheel_speed wheel_speed){
    printf("%lf,%lf\n", wheel_speed.left_wheel_speed, wheel_speed.right_wheel_speed);
}

//Puts all mu into mu_array
void fuzzy_membership_function_output(double de_error, double dh_error, double mu[N_RULES][N_INPUT_VARIABLES]){
    mu[0][0] = fuzzy_de_membership_func(de_error, SMALL);
    mu[1][0] = mu[0][0];

    mu[2][0] = fuzzy_de_membership_func(de_error, LARGE);
    mu[3][0] = mu[2][0];

    mu[0][1] = fuzzy_thetae_membership_func(dh_error, SMALL);
    mu[2][1] = mu[0][1];

    mu[1][1] = fuzzy_thetae_membership_func(dh_error, LARGE);
    mu[3][1] = mu[1][1];

}


double fuzzy_de_membership_func(double d_error, ERR_EXP error_expression){
    double mu_de = min( max(0, (d_error - 80) / -160 ), 1);

    if(error_expression == SMALL){
        return mu_de;
    }

    else if(error_expression == LARGE){
        return 1 - mu_de;
    }

    else{
        printf("Undefined error expression\n");
        return -1;
    }

}

double fuzzy_thetae_membership_func(double h_error, ERR_EXP error_expression){
    double mu_thetae = (-h_error + 180)/180;

    if(error_expression == SMALL){
        return mu_thetae;
    }

    else if(error_expression == LARGE){
        return 1 - mu_thetae;
    }

    else{
        printf("Undefined error expression\n");
        return -1;
    }

}


double distance(Position from, Position to){
    double x = pow(to.x - from.x, 2);
    double y = pow(to.y - from.y, 2);

    return sqrt(x + y);
}



double min(double a, double b){
    if(a < b){
        return a;
    }

    else{
        return b;
    }
}

double max(double a, double b){
    if(a > b){
        return a;
    }

    else{
        return b;
    }

}


int fuzzy_read_old_data(FILE* fp, Old_data* data){
    char input_string[100];

    int i = 0;

    while(fgets(input_string, 100, fp) != NULL){
        sscanf(input_string, "%lf,%lf,%lf,%lf,%lf,%lf\n",
               &(data->ref_state[i].pos.x), &(data->ref_state[i].pos.y), &(data->ref_state[i].angle),
               &(data->current_state[i].pos.x), &(data->current_state[i].pos.y), &(data->current_state[i].angle));

        i++;
    }


    return i;
}
