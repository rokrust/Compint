#include "neuron.h"

#include <stdio.h>
#include <math.h>
#include <cstdlib>


struct NeuralNetwork{
  	const int N_LAYERS;
    Neuron** _neurons;
	const int* _n_layer_inputs;

};


void train_network_from_data(NeuralNetwork* network){
    double* input[network->n_layer_inputs[0]];
    int data_class;

    scanf("%d,%d,%d", input[0], input[1], &data_class);
    while(data_class != 0){ //end of training data
		
        for(int layer = 1; layer < network->N_LAYERS; layer++){
			int i = 0;
			for(Neuron* neuron = &(network->_neurons[layer][i]);
 	 			i < network->_n_layer_inputs[layer]; 
				neuron = &(network->_neurons[layer][++i]))
				

            	double out = output(neuron, input);
				
				//misclassification
	            if(_convert_output_to_class(out) != data_class){
    	            _update_neuron_weights(neuron, data_class, out, input);
    	        }

        }

	    scanf("%d,%d,%d", input[0], input[1], &data_class);
    }
}


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




//iz good
NeuralNetwork* initialize_network(const int* n_layer_inputs, const int n_layers){
	NeuralNetwork* network;
	network->N_LAYERS = n_layers;
	network->
	

	//initialize weights
	for(int layer = 0; layer < n_layers; layer++){
		network->_n_layer_inputs[layer] = n_layer_inputs[layer];				

		for(int i = 0; i < network->_n_layer_inputs[layer+1]; i++){
			network->_neurons[layer][i].weights = (double)malloc(sizeof(double)*)

			double random_weight = (double)(rand() % 100)/10000;
			network->_neurons[layer][i]._weights[i] = random_weight;
		}
	}

	return network;
}


//iz good
int _convert_output_to_class(double output){
    if(output < 0){
        return (int)floor(output);
    }

    else{
        return (int)ceil(output);
    }

}

