#include "network.h"

int main() {
	const int n_layers = 1;
	int n_layer_inputs[1] = { 1 };

	NeuralNetwork *network = initialize_network(n_layer_inputs, n_layers);

	return 0;
}
