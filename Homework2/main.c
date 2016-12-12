#include "network.h"
#include <stdio.h>

int main() {

	const int n_layers = 5;
	int n_layer_inputs[5] =< { 2, 5, 3, 2, 1};

	double* inputList[1000];
	int classList[1000];
	int largest_value = read_training_data(inputList, data_class);

	NeuralNetwork *network = initialize_network(n_layer_inputs, n_layers);
	train_network_from_data(network, inputList, data_class);
	
	return 0;
}
