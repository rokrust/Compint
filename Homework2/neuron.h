#pragma once
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

struct Neuron {
	int N_INPUTS;
	double* _weights;
	double net_input;
	int is_output_neuron;
	

	double (*action_function)(double);
	double (*diff_action_function)(double);
};

typedef struct Neuron Neuron;


double _action_function(const double summed_weighted_input);
double _diff_action_function(double input);
double output(Neuron* neuron, double* input);
double net_input(Neuron* neuron, double* input);
Neuron* initialize_neuron(int n_inputs);


double _action_function(const double summed_weighted_input) {
	return tanh(summed_weighted_input);
}

double _diff_action_function(double input){
	return 1-pow(tanh(input), 2);
}

//Maybe add a bias term
double output(Neuron* neuron, double* input) {
	return _action_function(net_input(neuron, input));
}

double net_input(Neuron* neuron, double* input){
	double net = 0;

	for (int i = 0; i < neuron->N_INPUTS; i++) {
		net += neuron->_weights[i] * input[i];
	}

	neuron->net_input = net;

	return net;
}


Neuron* initialize_neuron(int n_inputs) {
	Neuron* neuron = malloc(sizeof(Neuron));
	neuron->N_INPUTS = n_inputs;
	neuron->_weights = malloc(sizeof(double)*n_inputs);
	
	//Initialize weights	
	for (int i = 0; i < n_inputs; i++) {

		double random_weight = (double)((rand() % 100)-49) / 100000;
		neuron->_weights[i] = random_weight;
	}
	
	return neuron;
}

