#pragma once
#define _CRT_SECURE_NO_WARNINGS
#define learning_rate 0.2


#include "neuron.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#define _LEARNING_RATE 0.05

struct NeuralNetwork {
	int N_LAYERS;
	Neuron** _neurons;
	int* _n_layer_inputs;
};
typedef struct NeuralNetwork NeuralNetwork;


void copy_array(double* arr1, double* arr2, int length);
void update_network_weights_if_necessary(NeuralNetwork* network, double out, int data_class, double* input);
double network_output(NeuralNetwork* network, double* start_input);
int _convert_output_to_class(double output);

//returns largest input for scaling purposes
//in case it should be needed
double read_training_data(FILE* fp, double inputs[][2], int data_class[]) {
	double largest_input = -INFINITY;

	int i = 0;
	char inputString[200];
	
	fgets(inputString, 200, fp);
	sscanf(inputString, "%lf,%lf,%d\n", &(inputs[i][0]), &(inputs[i][1]), &(data_class[i]));

	while (data_class[i] != 0) { //end of training data
		if(inputs[i][0] > largest_input){
			//printf("Larger input found\n");
			largest_input = inputs[i][0];		
		}

		if(inputs[i][1] > largest_input){
			largest_input = inputs[i][1];		
		}
		
		i++;
		fgets(inputString, 200, fp);	
		sscanf(inputString, "%lf,%lf,%d\n", &(inputs[i][0]), &(inputs[i][1]), &(data_class[i]));
	}

	return largest_input;
}

void train_network_from_data(NeuralNetwork *network, double inputs[1000][2], int* data_class){
	
	double output;
	for(int i = 0; data_class[i] != 0; i++){
		output = network_output(network, inputs[i]);	
		update_network_weights_if_necessary(network, output, data_class[i], inputs[i]);
	}

}


void update_network_weights_if_necessary(NeuralNetwork* network, double out, int data_class, double* input) {
	//misclassification
	//if (_convert_output_to_class(out) != data_class) {
		//printf("Misclassification\n");
		double delta_j[50]; //error gradient of all neurons in the next layer
		double delta_i; //error gradient of current neuron
		double error = (double)(data_class) - out;

		for(int layer = network->N_LAYERS-1; layer > 0; layer--){
			//printf("Layer: %d\n", layer);
			//printf(" First for\n");

			for(int neuron = 0; neuron < network->_n_layer_inputs[layer-1]; neuron++){
				//printf("  Second for\n");
				
				//Output neuron
				if(layer == network->N_LAYERS-1){
					//printf("   Weight: %lf\n", network->_neurons[0][0]._weights[0]);
					//printf("   Output neuron found\n");
					delta_i = 2*error;
				}
				
				//Hidden neuron
				else{
					
					printf("   Hidden layer neuron found\n");
					double net_delta = 0;
					for(int j = 0; j < network->_n_layer_inputs[layer+1]; j++){
						printf("    Finding net_delta_j\n");
						net_delta += network->_neurons[layer+1][j]._weights[neuron]*delta_j[j];
					}

					delta_i = _diff_action_function(network->_neurons[layer][neuron].net_input)*net_delta;
				}

				//Iterate through all weights and update them.				
				
				for(int i = 0; i < network->_neurons[layer][neuron].N_INPUTS; i++){
					//printf("   Iterating through neuron weights\n");
					double delta_w;

					if(layer != 1){
						delta_w = delta_i*_action_function(network->_neurons[layer-1][i].net_input);
					}
					else{
						delta_w = delta_i*input[i];
					}
					network->_neurons[layer][neuron]._weights[i] += _LEARNING_RATE*delta_w;
				}
								
						
				delta_j[neuron] = delta_i;
			}
		}
		//Gotta fix this function
		//update_neuron_weights();
	//}
}


double network_output(NeuralNetwork* network, double* start_input) {
	//printf("input: %lf, %lf\n", start_input[0], start_input[1]);
	double input[100];
	copy_array(input, start_input, network->_n_layer_inputs[0]);

	for (int layer = 0; layer < network->N_LAYERS; layer++) {
		double out[network->_n_layer_inputs[layer+1]];
		
		int i = 0;
		for (Neuron* neuron = network->_neurons[layer];
			i < network->_n_layer_inputs[layer + 1];
			neuron++) {
			//printf("%f\n", neuron->_weights[0]);
			out[i++] = output(neuron, input);
		}
		copy_array(input, out, network->_n_layer_inputs[layer]);
	}
	//printf("Output: %lf\n", input[0]);
	return input[0];
}

//iz good
NeuralNetwork* initialize_network(int* n_layer_inputs, int n_layers) {

	NeuralNetwork* network;
	network = malloc(sizeof(NeuralNetwork));
	network->_n_layer_inputs = malloc(sizeof(int)*(n_layers+1)); //+1 To include number of system inputs
	network->_neurons = malloc(sizeof(Neuron *)*n_layers);
	network->N_LAYERS = n_layers;
	
	network->_n_layer_inputs[0] = n_layer_inputs[0];

	//Initialize layers
	for (int layer = 0; layer < n_layers; layer++) {
		
		//Don't make neurons for the inputs
		network->_neurons[layer] = malloc(sizeof(Neuron)*n_layer_inputs[layer + 1]);
		network->_n_layer_inputs[layer+1] = n_layer_inputs[layer+1];
		
		for(int neuron = 0; neuron < n_layer_inputs[layer+1]; neuron++){
			Neuron* temp = initialize_neuron(n_layer_inputs[layer]);
			network->_neurons[layer][neuron] = *temp;

		}
	}

	return network;
}


int _convert_output_to_class(double output) {
	if (output < 0) {
		return -1;
	}

	else {
		return 1;
	}

}

void copy_array(double* arr1, double* arr2, int length) {
	for (int i = 0; i < length; i++) {
		arr1[i] = arr2[i];
	}
}

int read_running_data(FILE* fp, double inputs[][2]) {

	char inputString[200];
	
	int i = 0;
	while (fgets(inputString, 200, fp)) { //end of training data
		sscanf(inputString, "%lf,%lf\n", &(inputs[i][0]), &(inputs[i][1]));
		i++;
	}
	return i;
}

void run_network_from_data(NeuralNetwork* network, double inputs[][2], int n_inputs){

	for(int i = 0; i < n_inputs; i++){
		printf("%+d\n", _convert_output_to_class(network_output(network, inputs[i])));			
	}
}
