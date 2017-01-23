#include "fuzzy.h"

int main(){
    Data data;
    int n_samples = fuzzy_read_data(&data);

    for(int i = 0; i < n_samples; i++){
        Wheel_speed wheel_speed = fuzzy_determine_wheel_speeds(data, i);
        //printf("%lf,", wheel_speed.left_wheel_speed);
        //printf("%lf\n", wheel_speed.right_wheel_speed);
        //printf("%lf,%lf\n", wheel_speed.left_wheel_speed, wheel_speed.right_wheel_speed);
        fuzzy_print_wheel_speeds(wheel_speed);
    }

    return 0;
}
