#include <math.h>
#include <stdio.h>
#include <stdlib.h>


#define N_RULES 4
#define N_INPUT_VARIABLES 2

typedef struct{
    double x;
    double y;

} Position;

typedef struct{
    Position ref_pos[1000];
    double ref_angle[1000];
    Position cur_pos[1000];
    double cur_angle[1000];

} Data;

typedef struct{
    double mu_small_de;
    double mu_large_de;
    double mu_small_thetae;
    double mu_large_thetae;

} Mu;

typedef struct{
    double left_wheel_speed;
    double right_wheel_speed;
} Wheel_speed;

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


int fuzzy_read_data(Data* data){
    FILE* fp = fopen("testInput41A.txt", "r");

    char input_string[100];

    int i = 0;

    while(fgets(input_string, 100, fp) != NULL){

        sscanf(input_string, "%lf,%lf,%lf,%lf,%lf,%lf\n",
               &(data->ref_pos[i].x), &(data->ref_pos[i].y), &(data->ref_angle[i]),
               &(data->cur_pos[i].x), &(data->cur_pos[i].y), &(data->cur_angle[i]));

        i++;
    }

    return i;
}


Wheel_speed fuzzy_determine_wheel_speeds(Data data, int input_nbr){
    Wheel_speed wheel_speed;

    double de_error = distance(data.cur_pos[input_nbr], data.ref_pos[input_nbr]);
    double dh_error = fabs(data.ref_angle[input_nbr] - data.cur_angle[input_nbr]);

    double mu[N_RULES][N_INPUT_VARIABLES];
    fuzzy_membership_function_output(de_error, dh_error, mu);

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


    return wheel_speed;
}

Wheel_speed fuzzy_determine_rule_output(double de, double thetae, int rule_nbr){
    Wheel_speed u_i;

    u_i.left_wheel_speed = de_gain[rule_nbr]*de + thetae_gain[rule_nbr]*thetae;
    u_i.right_wheel_speed = de_gain[rule_nbr]*de - thetae_gain[rule_nbr]*thetae;

    return u_i;
}



//*****************************Ok************************************//

int fuzzy_print_wheel_speeds(Wheel_speed wheel_speed){
    return printf("%lf,%lf", wheel_speed.left_wheel_speed, wheel_speed.right_wheel_speed);
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
