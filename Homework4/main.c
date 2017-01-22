#include "fuzzy.h"

int main(){
    Data data;
    int n_samples = fuzzy_read_data(&data);

    fuzzy_run(data, n_samples);

    return 0;
}
