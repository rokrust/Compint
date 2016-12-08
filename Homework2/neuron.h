#ifndef NEURON_H
#define NEURON_H

#include <stdio.h>
#include <math.h>
#include <cstdlib>

#define N_INPUTS 2
#define N_NEURONS 1

struct Neuron{
    double _weights[N_INPUTS];
    double _action_function(double summed_weighted_input);


    double output();

};

double Neuron::_action_function(double summed_weighted_input){
    return tanh(summed_weighted_input);
}


//Maybe add a bias term
double Neuron::output(){
    double summed_weighted_output = 0;

    for(int j = 0; j < N_INPUTS; j++){
        summed_weighted_output += _weights[j]*input[j];
    }

    return _action_function(summed_weighted_input);
}




struct NeuralNetwork{
    //Should Neuron have child_neuron as member instead?
    //Or maybe this should be two dimensional with N_LAYERS added
    Neuron _neuron[N_NEURONS];
    const double _learning_rate = 0.2;

    void _update_neuron_weights();
    int _convert_output_to_class();



    void initialize_data();

    //Should probably call this train_network or something
    void train_network_from_data();

    void run();
};

int NeuralNetwork::_convert_output_to_class(double output){
    if(output < 0){
        return (int)output; //Simple floor function
    }

    else{
        return (int)ceil(output);
    }

}



void NeuralNetwork::_update_neuron_weights(int current_neuron, int data_class, double output, double input){
    for(int i = 0; i < N_INPUTS; i++){
        double old_weight = _neuron[current_neuron]._weights[i];

        _neuron[current_neuron].weights[i] = old_weight + _learning_rate*(data_class-output)*input;
    }

}




void NeuralNetwork::train_network_from_data(){
    double* input[N_INPUTS];
    int data_class;

    scanf("%d,%d,%d", input[0], input[1], &data_class);
    while(data_class != 0){ //end of training data

        //do stuff. Something with tanh action function
        for(int i = 0; i < N_NEURONS; i++){
            double output = _neuron[i].output();

            if(_convert_output_to_class(output) != data_class){
                _update_neuron_weights(data_class, output, input);
            }
        }

        scanf("%d,%d,%d", x, y, data_class);
    }
}



void NeuralNetwork::initialize_data(){
    for(int i = 0; i < N_NEURONS; i++){
        for(int j = 0; j < N_INPUTS; j++){
            //Weight is a random number less than 0.01
            double random_weight = (double)(rand() % 100)/10000;
            _neuron[i]._weights[j] = random_weight;
        }
    }
}

void NeuralNetwork::run(){
    double* input[N_INPUTS];
    int data_class;

    while(scanf("%d,%d,%d", input[0], input[1], &data_class) != EOF){

        //Gotta pipe this into other neurons
        for(int i = 0; i < N_NEURONS; i++){
            double output = _neuron[i].output();
        }

        printf("%+d\n", _convert_output_to_class(output));
    }

}


#endif // NEURON_H
