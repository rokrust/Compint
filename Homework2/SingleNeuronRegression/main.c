#include "network.h"
#include <stdio.h>
#include <time.h>

int main() {
	srand(time(NULL));
	double inputList[1000][1];
	double classList[1000];
	FILE* fp = fopen("testInput12A.txt", "r"); //Static to continue reading test data
	
	//Don't count the input layer. Must however count the outputs.
	const int n_layers = 1;
	int n_layer_inputs[2] = { 1, 1}; //Include bias in number of inputs
	NeuralNetwork *network = initialize_network(n_layer_inputs, n_layers);

	//Learning/training phase
	double previous_error = 100000.0;
	double largest_value = read_training_data(fp, inputList, classList);
	printf("largest_value: %lf\n", largest_value);
	for(int i = 0; i < 10000; i++){
		train_network_from_data(network, inputList, classList, largest_value);
		if((fabs(previous_error) < fabs(summed_error) && i > 300) || summed_error < 0.0000001){
			printf("i1: %d\n", i);
			break;
		}
		
		//printf("lowest error: %lf-%d\n", previous_error, i);
		previous_error = summed_error;
		summed_error = 0;
	}


	//printf("lowest error: %lf\n", previous_error);
	//print_network_weights(network);

	//Running phase
	int n_inputs = read_running_data(fp, inputList);
	run_network_from_data(network, inputList, n_inputs, largest_value);

	//printf("Summed error: %lf\n", summed_error);
	//printf("\n");
	//print_network_weights(network);

	fclose(fp);
	return 0;
}
