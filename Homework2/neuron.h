#pragma once
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

struct Neuron {
	int N_INPUTS;
	double* _weights;
};

typedef struct Neuron Neuron;



double _action_function(const double summed_weighted_input) {
	return tanh(summed_weighted_input);
}


//Maybe add a bias term
double output(Neuron* neuron, double* input) {
	double summed_weighted_input = 0;

	for (int j = 0; j < neuron->N_INPUTS; j++) {
	//	printf("Weight: %f\n", neuron->_weights[j]);
		summed_weighted_input += neuron->_weights[j] * input[j];
	}

	return _action_function(summed_weighted_input);
}

//Gotta be fixed
void update_neuron_weights(Neuron* neuron, int data_class, double output, double input, double _LEARNING_RATE) {
	for (int i = 0; i < neuron->N_INPUTS; i++) {
		double old_weight = neuron->_weights[i];

		neuron->_weights[i] = old_weight + _LEARNING_RATE*(data_class - output)*input;
	}
}


//segfault coming from weights after init
Neuron* initialize_neuron(int n_inputs) {
	Neuron* neuron = malloc(sizeof(Neuron));
	neuron->N_INPUTS = n_inputs;
	neuron->_weights = malloc(sizeof(double)*n_inputs);

	for (int i = 0; i < n_inputs; i++) {
	
		double random_weight = (double)(rand() % 100) / 100;
		neuron->_weights[i] = random_weight;
	}
	
	return neuron;
}

