#pragma once
#define _CRT_SECURE_NO_WARNINGS



#include "neuron.h"

#include <stdio.h>
#include <math.h>
#include <stdlib.h>


#define _LEARNING_RATE 0.7

struct NeuralNetwork {
	int N_LAYERS;
	Neuron** _neurons;
	int* _n_layer_inputs;
};
typedef struct NeuralNetwork NeuralNetwork;

double summed_error = 0;
void copy_array(double* arr1, double* arr2, int length);
void update_network_weights(NeuralNetwork* network, double out, int data_class, double* input);
double network_output(NeuralNetwork* network, double* start_input);
int _convert_output_to_class(double output);

//returns largest input for scaling purposes
//in case it should be needed
double read_training_data(FILE* fp, double inputs[][3], int data_class[]) {
	double largest_input = -INFINITY;

	int i = 0;
	char inputString[200];
	
	fgets(inputString, 200, fp);
	sscanf(inputString, "%lf,%lf,%d\n", &(inputs[i][0]), &(inputs[i][1]), &(data_class[i]));
	inputs[i][3] = 1.0;
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
		inputs[i][3] = 1.0;
		fgets(inputString, 200, fp);	
		sscanf(inputString, "%lf,%lf,%d\n", &(inputs[i][0]), &(inputs[i][1]), &(data_class[i]));
	}
	
	return fabs(largest_input);
}

void train_network_from_data(NeuralNetwork *network, double inputs[][3], int* data_class, double largest_value){
	
	double output;
	for(int i = 0; data_class[i] != 0; i++){
		//printf("Largest value: %lf\n", largest_value);
		//inputs[i][0] /= largest_value;
		//inputs[i][1] /= largest_value;
		//data_class[i] /= largest_value;
		//printf("%lf,%lf,%d\n", inputs[i][0], inputs[i][1], data_class[i]);		

		output = network_output(network, inputs[i]);
		//printf("output: %lf\n", output);	
		update_network_weights(network, output, data_class[i], inputs[i]);
	}

}


void update_network_weights(NeuralNetwork* network, double out, int target, double* input) {
	double delta_j[50]; //error gradient of all neurons in the next layer
	double delta_i; //error gradient of current neuron
	double error = pow((double)(target) - out, 2);
	//summed_error += error;
	double diff_error = 2*((double)(target)-out);
	double new_weights[100][100];

	//Iterate backwards through the network
	for(int layer = network->N_LAYERS-1; layer >= 0; layer--){
		for(int neuron = 0; neuron < network->_n_layer_inputs[layer+1]; neuron++){

			//Output neuron
			if(layer == network->N_LAYERS-1){
				//printf("   Output neuron found\n");
				delta_i = diff_error;
			}
				
			//Hidden neuron
			else{
					
				double net_delta = 0;
				for(int j = 0; j < network->_n_layer_inputs[layer+2]; j++){
					net_delta += network->_neurons[layer][j]._weights[neuron]*delta_j[j];
				}

				delta_i = _diff_action_function(network->_neurons[layer][neuron].net_input)*net_delta;
			}

			//Find new weights
			for(int i = 0; i < network->_neurons[layer][neuron].N_INPUTS; i++){
				double delta_w;
				
				//Non-input layer
				if(layer != 0){
					delta_w = _diff_action_function(network->_neurons[layer][i].net_input)*
							  delta_i*_action_function(network->_neurons[layer-1][i].net_input);
				}
				
				//Input layer
				else{
					delta_w = delta_i*input[i];
				}

				new_weights[layer][neuron] = delta_w
			}
								
			delta_j[neuron] = delta_i;
		}		
	}

	set_weights(network, new_weights);
}

void set_weights(Network* network, double new_weights[][100]){
	for(int layer = 0; layer < network->N_LAYERS; layer++){
		for(int neuron = 0; neuron < network->_n_layer_inputs[layer+1]; neuron++){
			network->_neurons[layer][neuron]._weights[i] += _LEARNING_RATE*new_weights[layer][neuron];

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
		inputs[i][3] = 1;
		i++;
	}
	//printf("%lf,%lf\n", (inputs[0][0]), (inputs[0][1]));
	return i;
}

void run_network_from_data(NeuralNetwork* network, double inputs[][3], int n_inputs, double largest_value){

	for(int i = 0; i < n_inputs; i++){
		//inputs[i][0] /= largest_value;
		//inputs[i][1] /= largest_value;
		printf("%+d\n", _convert_output_to_class(network_output(network, inputs[i])));			
	}
}

