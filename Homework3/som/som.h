#include <stdio.h>
#include <math.h>

#define N_NODES 50

double learning_rate = 0.1;
double neighbor_var = 0.1;

struct Position{
	int x;
	int y;

}; typedef struct Position Position;

struct Node{
	Position pos;

}; typedef struct Node Node;

struct Som_network{
	Node node[N_NODES];

}; typedef struct Som_network Som_network;

//Puts input into data and returns number of elements
int som_read_data(int data[1000], Position pos[1000]){
	FILE* fp = fopen("testInput23A.txt", "r");
	char* input_string[100];
	input_string[99] = '\0'
	
	int i = 0;
	while(fgets(input_string, 200, fp) != 0){
		sscanf(input_string, "%d,%d,%d\n", &data[i], &pos[i].x, &pos[i].y);
		
		i++;
	}

	return i;	
}

//Should put all nodes evenly on a circle
void som_init_network(Som_network* network, double radius){
	for(int i = 0; i < N_NODES; i++){
		network->node[i].pos.x = radius*cos((2*M_PI / N_NODES) * i);
		network->node[i].pos.y = radius*sin((2*M_PI / N_NODES) * i);
	}
}

double som_distance(Node node, Position city_pos){
	double squared_distance_x = pow(node.x - city_pos.x), 2);
	double squared_distance_y = pow(node.y - city_pos.y), 2);

	return sqrt(squared_distance_x + squared_distance_y);
}

//Returns the index of the node closest to city_pos
//and saves the distance in the distance variable
int som_determine_closest_node(Som_network* network, Position city_pos, double* distance){
	int closest_node = 0;

	for(int i = 0; i < N_NODES; i++){
		double temp = som_distance(network->node[i], city_pos);

		if(temp < distance){
			*distance = temp;
			closest_node = i;
		}

	}

	return closest_node;
}

void som_update_node_position(Som_network* network, int node_index, Position city_pos){
	network->node[node_index] = 
}
