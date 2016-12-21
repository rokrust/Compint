#pragma once
#define _CRT_SECURE_NO_WARNINGS



#include "neuron.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define _LEARNING_RATE 0.001
#define div_factor1 9.9798
#define div_factor2 9.9653

struct NeuralNetwork {
	int N_LAYERS;
	Neuron** _neurons;
	int* _n_layer_inputs;
};
typedef struct NeuralNetwork NeuralNetwork;

double summed_error = 0;
void copy_array(double* arr1, double* arr2, int length);
void update_network_weights(NeuralNetwork* network, double error, double* input);
double network_output(NeuralNetwork* network, double* start_input);
int _convert_output_to_class(double output);
void set_weights(NeuralNetwork* network, double inputs[]);
void print_network_weights(NeuralNetwork* network);

//returns largest input for scaling purposes
//in case it should be needed
double read_training_data(FILE* fp, double inputs[][3], int data_class[]) {
	double largest_input = -INFINITY;

	int i = 0;
	char inputString[200];
	
	fgets(inputString, 200, fp);
	sscanf(inputString, "%lf,%lf,%d\n", &(inputs[i][0]), &(inputs[i][1]), &(data_class[i]));
	inputs[i][2] = 1.0;
	while (data_class[i] != 0) { //end of training data
		if(inputs[i][0] > largest_input){
			//printf("Larger input found\n");
			largest_input = inputs[i][0];	
		}

		if(inputs[i][1] > largest_input){
			largest_input = inputs[i][1];
		}
		
		//printf("Largest input: %lf\n", largest_input);
		i++;

		inputs[i][2] = 1.0;

		fgets(inputString, 200, fp);	
		sscanf(inputString, "%lf,%lf,%d\n", &(inputs[i][0]), &(inputs[i][1]), &(data_class[i]));
	}
	
	return fabs(largest_input);
}

void train_network_from_data(NeuralNetwork *network, double inputs[][3], int* data_class, double largest_value){
	
	double output, error;
	for(int i = 0; data_class[i] != 0; i++){

		output = network_output(network, inputs[i]);
		error = data_class[i] - output;
		//printf("output: %lf\n", output);	
		update_network_weights(network, error, inputs[i]);
	}

}



void update_network_weights(NeuralNetwork* network, double error, double* input){
	double sqrd_error = pow(error, 2);	

	//output neuron
	network->_neurons[network->N_LAYERS-1][0].delta_i = 2*error;
	summed_error += error;
	//printf("delta_i: %lf\n", network->_neurons[2][0].delta_i);
	
	//Iterate backwards through the network. Determine delta_i for all neurons 
	for(int layer = network->N_LAYERS-2; layer >= 0; layer--){
		Neuron* next_layer = network->_neurons[layer+1];
		Neuron* current_layer = network->_neurons[layer];
		
		for(int neuron = 0; neuron < network->_n_layer_inputs[layer+1]; neuron++){			
			Neuron* current_neuron = &(current_layer[neuron]); //Simplify accessing
			
			//Iterate through previous layer and multiply errors and weights					
			double net_delta = 0;
			
			for(int i = 0; i < network->_n_layer_inputs[layer+2]; i++){
				
				net_delta += next_layer[i].delta_i*next_layer[i]._weights[neuron];	
			}
			
			current_neuron->delta_i = net_delta*_diff_action_function(current_neuron->output);
		}
	}
	//printf("\n");
	set_weights(network, input);
}

void set_weights(NeuralNetwork* network, double inputs[]){
	//Handle input layer apart from the rest
	for(int neuron = 0; neuron < network->_n_layer_inputs[1]; neuron++){
		Neuron* current_neuron = &(network->_neurons[0][neuron]);
		
		for(int weight = 0; weight < network->_n_layer_inputs[0]; weight++){
			current_neuron->_weights[weight] += _LEARNING_RATE*current_neuron->delta_i*inputs[weight];
		}

	}
	
	//Iterate through the network starting after the input layer
	for(int layer = 1; layer < network->N_LAYERS; layer++){
		Neuron* current_layer = network->_neurons[layer];
		Neuron* previous_layer = network->_neurons[layer-1];

		for(int neuron = 0; neuron < network->_n_layer_inputs[layer+1]; neuron++){
			Neuron* current_neuron = &(current_layer[neuron]); //Simplify accessing

			for(int i = 0; i < network->_n_layer_inputs[layer]; i++){
				current_neuron->_weights[i] += _LEARNING_RATE*current_neuron->delta_i*previous_layer[i].output;				
			}
		}
	}
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
	//printf("Weight: %lf\n", network->_neurons[0][1]._weights[1]);
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
	//print_network_weights(network);
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

int read_running_data(FILE* fp, double inputs[][3]) {

	char inputString[200];
	
	int i = 0;
	while (fgets(inputString, 200, fp)) { //end of training data
		sscanf(inputString, "%lf,%lf\n", &(inputs[i][0]), &(inputs[i][1]));
		inputs[i][2] = 1.0;
		//inputs[i][0] *= 10;
		//inputs[i][1] *= 10;
		i++;
	}
	//printf("%lf,%lf\n", (inputs[0][0]), (inputs[0][1]));
	return i;
}

void run_network_from_data(NeuralNetwork* network, double inputs[][3], int n_inputs, double largest_value){

	for(int i = 0; i < n_inputs; i++){
		inputs[i][0] /= div_factor2;
		inputs[i][1] /= div_factor2;
		printf("%+d\n", _convert_output_to_class(network_output(network, inputs[i])));			
	}
}


void print_network_weights(NeuralNetwork* network){
	for(int layer = 0; layer < network->N_LAYERS; layer++){
		Neuron* current_layer = network->_neurons[layer];

		for(int neuron = 0; neuron < network->_n_layer_inputs[layer+1]; neuron++){
			Neuron* current_neuron = &(current_layer[neuron]);
			printf("|");
			for(int weight = 0; weight < network->_n_layer_inputs[layer]; weight++){
				printf("%lf ", current_neuron->_weights[weight]);
			}
		}
		printf("|\n");
	}

}
