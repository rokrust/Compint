#include "vector_quantization.h"

int main(){
	srand(time(NULL));

	double data[1000][2];
	int k;
	double threshold;
	int n_points = k_means_read_data(&k, data, &threshold);

	Comp_network* network;

	while(1){
		network = k_means_init(data, k, n_points);
		
		for(int i = 0; i < n_iterations; i++){
			k_means_run(network, n_points, data);
			lower_eta(i);
		}
		
		if(!restart_required(network, k, threshold)){
			break;
		}	
	}

	print_cluster_centroids(network);

	return 0;
}
