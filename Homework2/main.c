#include "network.h"
#include <stdio.h>

int main() {
	double inputList[1000][3];
	int classList[1000];
	FILE* fp = fopen("testInput10A.txt", "r"); //Static to continue reading test data
	
	//Don't count the input layer. Must however count the outputs.
	const int n_layers = 1;
	int n_layer_inputs[2] = { 3, 1}; //Include bias in number of inputs
	NeuralNetwork *network = initialize_network(n_layer_inputs, n_layers);

	//Learning/training phase
	double largest_value = read_training_data(fp, inputList, classList);
	for(int i = 0; i < 1000; i++){
		train_network_from_data(network, inputList, classList, largest_value);
		printf("Summed error: %lf\n", summed_error);
		summed_error = 0;
	}

	//Running phase
//	int n_inputs = read_running_data(fp, inputList);
//	run_network_from_data(network, inputList, n_inputs, largest_value);

//	printf("Summed error: %lf\n", summed_error);

	fclose(fp);
	return 0;
}
