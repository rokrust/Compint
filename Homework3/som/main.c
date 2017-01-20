#include "som.h"

#include <stdlib.h>

int main(){
    Som_network* network = malloc(sizeof(Som_network));
    int data[1000];
    Position pos[1000];
    Ellipse ellipse;

    int n_cities = som_read_data(data, pos, &ellipse);

    som_init_network(network, ellipse);

    som_run_network(network, pos, n_cities);

    som_print_path(network);
}
