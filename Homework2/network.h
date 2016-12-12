#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define learning_rate 0.2


#include "neuron.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

struct NeuralNetwork {
	int N_LAYERS;
	Neuron** _neurons;
	int* _n_layer_inputs;
};
typedef struct NeuralNetwork NeuralNetwork;


void copy_array(double* arr1, double* arr2, int length);
void update_network_weights(NeuralNetwork* network, double out, int data_class);
double network_output(NeuralNetwork* network, double* start_input);
int _convert_output_to_class(double output);

//returns largest input for scaling purposes
//in case it should be needed
double read_training_data(double* inputs[1000], int data_class[1000]) {
	double largest_input = -INFINITY;

	FILE* fp = fopen("testInput10A.txt", "w+");
	
	int i = 0;
	fscanf(fp, "%f,%f,%d", &input[i][0], &input[i][1], &data_class[i]);
	while (data_class[i] != 0) { //end of training data
		if(input[i][0] > largest_input){
			printf("Larger input found\n");
			largest_input = input[i][0];		
		}

		if(input[i][1] > largest_input){
			largest_input = input[i][1];		
		}
		
		i++;
	}
	data_class[i] = 0;

	fclose(fp);
	
	return largest_data
	//fscanf(fp, "%d,%d,%d", &input[0], &input[1], &data_class);
}

void train_network_from_data(NeuralNetwork *network, double* inputs[1000], int* data_class){
	
	double output;

	for(int i = 0; data_class[i] != 0; i++){
		output = network_ouput(network, inputs[i]);	

		update_network_weights_if_necessary(network, output, data_class[i]);
	}


}


void update_network_weights_if_necessary(NeuralNetwork* network, double out, int data_class) {

	//misclassification
	if (_convert_output_to_class(out) != data_class) {
		//Gotta fix this function
		//update_neuron_weights();
	}
}


double network_output(NeuralNetwork* network, double* start_input) {
	double input[100];
	copy_array(input, start_input, network->_n_layer_inputs[0]);

	for (int layer = 0; layer < network->N_LAYERS; layer++) {
		double out[network->_n_layer_inputs[layer+1]];
		
		int i = 0;
		for (Neuron* neuron = network->_neurons[layer];
			i < network->_n_layer_inputs[layer + 1];
			neuron++) {

			out[i++] = output(neuron, input);
		}
		copy_array(input, out, network->_n_layer_inputs[layer]);
	}
	printf("output: %f\n", input[0]);
	return input[0];
}

//iz good
NeuralNetwork* initialize_network(int* n_layer_inputs, int n_layers) {

	NeuralNetwork* network = malloc(sizeof(NeuralNetwork));
	network->_n_layer_inputs = malloc(sizeof(int)*n_layers);
	network->_neurons = malloc(sizeof(Neuron *)*n_layers);
	network->N_LAYERS = n_layers;

	//Initialize layers
	for (int layer = 0; layer < n_layers-1; layer++) {
		network->_neurons[layer] = malloc(sizeof(Neuron)*n_layer_inputs[layer + 1]);
		
		//Gotta allocate both inputs and outputs
		network->_n_layer_inputs[layer] = n_layer_inputs[layer];
		network->_n_layer_inputs[layer+1] = n_layer_inputs[layer+1];
		//printf("First for: %d-%d\n", layer, network->_n_layer_inputs[layer]);

		int neuron_index = 0;
		for (Neuron* neuron = network->_neurons[layer];
			neuron_index < network->_n_layer_inputs[layer + 1]; //Number of neurons in layer
			neuron++) {

			neuron = initialize_neuron(network->_n_layer_inputs[layer]);
			network->_neurons[layer][neuron_index++] = *neuron;
		}
	}
	
	return network;
}


//iz good
int _convert_output_to_class(double output) {
	if (output < 0) {
		return (int)floor(output);
	}

	else {
		return (int)ceil(output);
	}

}

void copy_array(double* arr1, double* arr2, int length) {
	for (int i = 0; i < length; i++) {
		arr1[i] = arr2[i];
	}
}

/*
void train_network_from_data(NeuralNetwork* network) {
double input[2];
int data_class;

scanf("%d,%d,%d", &input[0], &input[1], &data_class);
while (data_class != 0) { //end of training data

for (int layer = 0; layer < network->N_LAYERS; layer++) {
int i = 0;
for (Neuron* neuron = &(network->_neurons[layer][i]);
i < network->_n_layer_inputs[layer + 1];
neuron = &(network->_neurons[layer][++i])) {


double out = output(neuron, input);

//misclassification
if (_convert_output_to_class(out) != data_class) {
_update_neuron_weights(neuron, data_class, out, input);
}
}
}

}

scanf("%d,%d,%d", input[0], input[1], &data_class);
}
*/

/*
void run(){
double* input[];
int data_class;

while(scanf("%d,%d,%d", input[0], input[1], &data_class) != EOF){

//Gotta pipe this into other neurons
for(int i = 0; i < N_NEURONS; i++){
double output = _neurons[i].output();
}

printf("%+d\n", _convert_output_to_class(output));
}

}
*/
