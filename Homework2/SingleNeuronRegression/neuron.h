#pragma once
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

struct Neuron {
	int N_INPUTS;
	double* _weights;
	double net_input;
	double output;	
	double delta_i;
};

typedef struct Neuron Neuron;


double _action_function(const double summed_weighted_input);
double _diff_action_function(double input);
double output(Neuron* neuron, double* input);
double net_input(Neuron* neuron, double* input);
Neuron* initialize_neuron(int n_inputs);


double _action_function(const double summed_weighted_input) {
	return summed_weighted_input;
}

double _diff_action_function(double input){
	return 1;
}

//Maybe add a bias term
double output(Neuron* neuron, double* input) {
	neuron->output = _action_function(net_input(neuron, input));
	return neuron->output;
}

double net_input(Neuron* neuron, double* input){
	double net = 0;

	for (int i = 0; i < neuron->N_INPUTS; i++) {
		net += neuron->_weights[i] * input[i];
	}
	net += neuron->_weights[neuron->N_INPUTS]; //bias
	neuron->net_input = net;

	return net;
}


Neuron* initialize_neuron(int n_inputs) {
	Neuron* neuron = malloc(sizeof(Neuron));
	neuron->N_INPUTS = n_inputs;
	neuron->_weights = malloc(sizeof(double)*(n_inputs+1));
	
	//Initialize weights	
	for (int i = 0; i < n_inputs+1; i++) {

		double random_weight = (double)((rand() % 100)) / pow(10, 4);
		neuron->_weights[i] = random_weight;
	}
	
	return neuron;
}

