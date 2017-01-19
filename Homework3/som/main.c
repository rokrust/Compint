#include "som.h"

#include <stdlib.h>

int main(){
    Som_network* network = malloc(sizeof(Som_network));
    int data[1000];
    Position pos[1000];

    printf("Read data\n");
    int n_cities = som_read_data(data, pos);

    for(int i = 0; i < n_cities; i++){
        printf("%d: %lf, %lf\n", i + 1, pos[i].x, pos[i].y);
    }

    printf("\nInit\n");
    som_init_network(network);

    //printf("Run network\n");
    som_run_network(network, pos, n_cities);

    for(int i = 0; i < N_NODES; i++){
        printf("%d: %lf, %lf\n", i + 1, network->node[i].pos.x, network->node[i].pos.y);
    }

    //printf("Print path\n");
    //som_print_path(network);
}
