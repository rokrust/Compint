#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define DIM 2
#define final_eta 0.0000001
#define start_eta 0.01
#define n_iterations 1000
#define eta_rate_of_change (start_eta - final_eta) / n_iterations
#define cluster_distance_threshold 3


double eta = start_eta;

struct Node{
	double* input_weight;
	double* competitive_weight;
	int n_inputs;

};typedef struct Node Node;

struct Comp_network{
	int k;
	Node* nodes;
};typedef struct Comp_network Comp_network;


void find_initial_centers(Comp_network* network, double data[1000][DIM], int n_points, int k);
void copy_array(double* array1, double* array2, int n);
void print_cluster_centroids(Comp_network* network);
double point_distance(double* point1, double* point2);
void set_init_weights(Comp_network* network, double data[1000][DIM], int k, int n_points);


Node* node_init(int k, double* weight){
	Node* n = malloc(sizeof(Node));

	n->n_inputs = DIM;
	n->input_weight = malloc(sizeof(double)*DIM);	
	n->competitive_weight = malloc(sizeof(double)*(k-1));
	
	for(int i = 0; i < DIM; i++){
		n->input_weight[i] = weight[i];
	}
	
	return n;
}

Comp_network* k_means_init(double data[1000][2], int k, int n_points){
	Comp_network* network = malloc(sizeof(Comp_network));
	network->k = k;
	network->nodes = malloc(sizeof(Node)*k);

	for(int i = 0; i < k; i++){
		//Choose random initial points
		double* weight = data[rand() % n_points];

		Node* temp = node_init(k, weight);
		network->nodes[i] = *temp;

	}	

	set_init_weights(network, data, k, n_points);
	//print_cluster_centroids(network);


	return network;
}

//returns number of elements
int k_means_read_data(int *k, double data[1000][2], double *threshold){
	char inputString[200];
	
	FILE *fp = fopen("testInput21A.txt", "r");

	//first line: number of clusters
	fgets(inputString, 200, fp);
	sscanf(inputString, "%d", k);
	double smallest = 10000000, largest = 0;
	
	int i = 0;
	double dummy[2] = {0, 0};
	while(fgets(inputString, 200, fp) != NULL){
		sscanf(inputString, "%lf,%lf\n", &(data[i][0]), &(data[i][1])); 
		double distance = point_distance(data[i], dummy);

		if(distance < smallest){
			smallest = distance;
		}
		
		if(distance > largest){
			largest = distance;
		}

		i++;
	}
	printf("largest %lf\n", largest);
	printf("smallest %lf\n", smallest);

	*threshold = (largest-smallest) / (*k/2);
	printf("Threshold: %lf\n", *threshold);
	return i;
}


void set_init_weights(Comp_network* network, double data[1000][DIM], int k, int n_points){
	double init_point[k][DIM];
	copy_array(init_point[0], data[rand() % n_points], DIM);
	copy_array(network->nodes[0].input_weight, init_point[0], DIM); 
	

	for(int i = 1; i < k; i++){
		double biggest_distance = 0;
		double smallest_distance = 1000000;

		for(int j = 1; j < n_points; j++){
			
			for(int l = 0; l < i; l++){
				double temp = point_distance(init_point[l], data[j]);

				if(temp < smallest_distance && temp != 0){
					//printf("smallest distance: %lf\n", temp);
					smallest_distance = temp;
				}
			}

			if(smallest_distance > biggest_distance){
				//printf("%d: New distance %lf\n", i, smallest_distance);				
				biggest_distance = smallest_distance;
				copy_array(init_point[i], data[j], DIM);
			}
		}
	}
}


double point_distance(double* point1, double* point2){
	return sqrt(pow(point2[0] - point1[0], 2) +
				pow(point2[1] - point1[1], 2));
}

int winning_node(Comp_network* network, double* point){
	int winner = -1;
	double distance, winner_distance = 100000.0;
	for(int i = 0; i < network->k; i++){
		Node cur_node = network->nodes[i];
		distance = point_distance(cur_node.input_weight, point);
		
		if(distance < winner_distance){
			winner_distance = distance;
			winner = i;
		}

	}

	return winner;
}


void update_weights(Node* cur_node, double* point){
	for(int i = 0; i < DIM; i++){
		double old = cur_node->input_weight[i];
		cur_node->input_weight[i] = old + eta*(point[i] - old);
	}

}

void lower_eta(int i){
	eta = eta_rate_of_change*i + start_eta;
}

void print_cluster_centroids(Comp_network* network){
	for(int i = 0; i < network->k; i++){
		for(int j = 0; j < DIM-1; j++){
			printf("%lf, ", network->nodes[i].input_weight[j]);
		}
		
		printf("%lf\n", network->nodes[i].input_weight[DIM-1]);
	}
}

void k_means_run(Comp_network* network, int n_points, double data[1000][2]){

	for(int i = 0; i < n_points; i++){
		double* point = data[i];
		int winner = winning_node(network, point);
		update_weights(&(network->nodes[winner]), point);

	}
}

int restart_required(Comp_network* network, int k, double threshold){
	
	for(int i = 0; i < k; i++){
		for(int j = i + 1; j < k; j++){
			double cluster_distance = point_distance(
				   network->nodes[i].input_weight, 
				   network->nodes[j].input_weight);
			
			if(cluster_distance < threshold){
				return 1;
			}

		}

	}

	return 0;
}

void copy_array(double* array1, double* array2, int n){
	for(int i = 0; i < n; i++){
		array1[i] = array2[i];
	}
}
