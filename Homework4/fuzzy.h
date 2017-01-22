#include <math.h>
#include <stdio.h>
#include <stdlib.h>

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

typedef enum{
    SMALL = 0,
    LARGE
} ERR_EXP;

double distance(Position from, Position to);
double min(double a, double b);
double max(double a, double b);
double fuzzy_de_membership_func(double d_error, ERR_EXP error_expression);
double fuzzy_thetae_membership_func(double h_error, ERR_EXP error_expression);


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

void fuzzy_run(Data data, int n_elements){
    for(int i = 0; i < n_elements; i++){
        double de_error = distance(data.cur_pos[i], data.ref_pos[i]);
        double dh_error = fabs(data.ref_angle[i] - data.cur_angle[i]);

        double mu_small_de = fuzzy_de_membership_func(de_error, SMALL);
        double mu_large_de = fuzzy_de_membership_func(de_error, LARGE);

        double mu_small_thetae = fuzzy_thetae_membership_func(dh_error, SMALL);
        double mu_large_thetae = fuzzy_thetae_membership_func(dh_error, LARGE);

        printf("%lf,%lf,%lf,%lf\n", mu_small_de, mu_large_de, mu_small_thetae, mu_large_thetae);
    }
}


void fuzzy_determine_wheel_speeds(Data data, int n_elements){



    for(int i = 0; i < n_elements; i++){
        double de_error = distance(data.cur_pos[i], data.ref_pos[i]);
        double dh_error = fabs(data.ref_angle[i] - data.cur_angle[i]);

        double mu_small_de = fuzzy_de_membership_func(de_error, SMALL);
        double mu_large_de = fuzzy_de_membership_func(de_error, LARGE);

        double mu_small_thetae = fuzzy_thetae_membership_func(dh_error, SMALL);
        double mu_large_thetae = fuzzy_thetae_membership_func(dh_error, LARGE);

    }
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
