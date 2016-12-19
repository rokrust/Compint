#include "network.h"
#include <stdio.h>

int main() {

	static double inputList[1000][2];
	int classList[1000];
	FILE* fp = fopen("testInput10A.txt", "r"); //Static to continue reading test data
	
	//Don't count the input layer. Must however count the outputs.
	const int n_layers = 2;
	int n_layer_inputs[3] = { 2, 3, 1};
	NeuralNetwork *network = initialize_network(n_layer_inputs, n_layers);

	//Learning/training phase
	read_training_data(fp, inputList, classList);
	train_network_from_data(network, inputList, classList);

	//Running phase
	int n_inputs = read_running_data(fp, inputList);
	run_network_from_data(network, inputList, n_inputs);

	fclose(fp);
	return 0;
}
