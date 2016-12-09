struct Neuron{
	int N_INPUTS;
    double* _weights;
};


double _action_function(const double summed_weighted_input){
    return tanh(summed_weighted_input);
}


//Maybe add a bias term
double output(const Neuron* neuron, const double* input){
    double summed_weighted_input = 0;

    for(int j = 0; j < N_INPUTS; j++){
        summed_weighted_input += neuron->_weights[j]*input[j];
    }

    return _action_function(summed_weighted_input);
}

//Gotta be fixed
void _update_neuron_weights(const Neuron* neuron, int data_class, double output, double input){
    for(int i = 0; i < N_INPUTS; i++){
        double old_weight = neuron->_weights[i];

        neuron->weights[i] = old_weight + _LEARNING_RATE*(data_class-output)*input;
    }

}

