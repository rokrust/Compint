#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define learning_rate 0.2


#include "neuron.h"

#include <stdio.h>
#include <math.h>


struct NeuralNetwork {
	int N_LAYERS;
	Neuron** _neurons;
	int* _n_layer_inputs;
};
typedef struct NeuralNetwork NeuralNetwork;


void copy_array(double* arr1, double* arr2, int length);
void update_network_weights(NeuralNetwork* network, double out, int data_class);
double network_output(NeuralNetwork* network, double* start_input);


void train_network_from_data(NeuralNetwork* network) {
	double input[2];
	int data_class;

	FILE* fp = fopen("testInput10A.txt", 'w+');

	fscanf(fp, "%d,%d,%d", &input[0], &input[1], &data_class);
	while (data_class != 0) { //end of training data
		double out = network_output(network, input);
		update_network_weights(network, out, data_class);

	}

	fscanf(fp, "%d,%d,%d", &input[0], &input[1], &data_class);
}


void update_network_weights(NeuralNetwork* network, double out, int data_class) {

	//misclassification
	if (_convert_output_to_class(out) != data_class) {
		//Gotta fix this function
		//update_neuron_weights();
	}
}


double network_output(NeuralNetwork* network, double* start_input) {
	double* input = malloc(sizeof(double)*network->_n_layer_inputs[0]);
	copy_array(input, start_input, network->_n_layer_inputs[0]);

	for (int layer = 0; layer < network->N_LAYERS; layer++) {
		double *out = malloc(sizeof(double)*network->_n_layer_inputs[layer+1]);
			
		int i = 0;
		for (Neuron* neuron = &(network->_neurons[layer][i]);
			i < network->_n_layer_inputs[layer + 1];
			neuron = &(network->_neurons[layer][++i])) {

			out[i] = output(neuron, input);
		}

		//output of neurons is input in the next
		copy_array(input, out, network->_n_layer_inputs[layer+1]);
	}
	return input[0];
}

//iz good
NeuralNetwork* initialize_network(int* n_layer_inputs, int n_layers) {
	NeuralNetwork* network = malloc(sizeof(NeuralNetwork));
	network->N_LAYERS = n_layers;
	network->_neurons = malloc(sizeof(Neuron *)*n_layers);

	for (int layer = 0; layer < n_layers; layer++) {
		network->_n_layer_inputs[layer] = n_layer_inputs[layer];
		network->_neurons[layer] = malloc(sizeof(Neuron)*n_layer_inputs[layer + 1]);

		int i = 0;
		for (Neuron* neuron = &(network->_neurons[layer][i]);
		i < network->_n_layer_inputs[layer + 1]; //Number of neurons in layer
			neuron = &(network->_neurons[layer][++i])) {

			initialize_neuron(neuron, n_layer_inputs[layer]);
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
