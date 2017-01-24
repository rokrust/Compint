#include "fuzzy.h"

int main(){
    FILE* fp = fopen("testInput41A.txt", "r");

    Data ref_data;
    int n_samples = fuzzy_read_data(fp, &ref_data);

    Robot_state current_state = ref_data.state[0];

    for(int i = 1; i < n_samples; i++){
        current_state = fuzzy_calculate_next_state(ref_data.state[i], current_state);
        printf("%f,%f,%f\n", current_state.pos.x, current_state.pos.y, current_state.angle);
    }

    fclose(fp);
    return 0;
}
