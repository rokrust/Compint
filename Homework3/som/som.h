#include <stdio.h>
#include <math.h>
#include <time.h>

#define N_NODES 16
#define START_RADIUS 1
#define RUNTIME_US 5
#define NEIGHBORHOOD_RADIUS_STEP 0.001
#define M_PI 3.1415925

double learning_rate = 0.1;
double neighbor_var = 10;
double neighborhood_radius = 0.1;

struct Position{
    double x;
    double y;

}; typedef struct Position Position;


//Might have to add a variable for affiliated city
struct Node{
	Position pos;
    int closest_city;

}; typedef struct Node Node;

struct Som_network{
	Node node[N_NODES];

}; typedef struct Som_network Som_network;


double som_distance(Position pos_a, Position pos_b);
int som_determine_closest_node(Som_network* network, Position city_pos);
void som_update_node_positions(Som_network* network, Position closest_node_pos, Position city_pos, int city_index);
void som_scale_input(Position pos[1000], int n_elements, double scaling_value);

void update_learning_rate(int iteration);
void update_neighborhood_variance(int iteration);
void update_neighborhood_radius();




//*************************************************************************//
//                                                                         //
//                            Public functions                             //
//                                                                         //
//*************************************************************************//


//Puts input into data and returns number of elements
int som_read_data(int data[1000], Position pos[1000]){
	FILE* fp = fopen("testInput23A.txt", "r");
    char input_string[100];

    int largest_value = 0;
	
	int i = 0;
    while(fgets(input_string, 200, fp) != 0){
        sscanf(input_string, "%d,%lf,%lf\n", &(data[i]), &(pos[i].x), &(pos[i].y));
		
        if(pos[i].x > largest_value){
            largest_value = pos[i].x;
        }

        if(pos[i].y > largest_value){
            largest_value = pos[i].y;
        }

		i++;
	}

    som_scale_input(pos, i, 1.0/largest_value);

	return i;	
}

//Should put all nodes evenly on a circle
void som_init_network(Som_network* network){
	for(int i = 0; i < N_NODES; i++){
        network->node[i].pos.x = START_RADIUS*cos((2*M_PI / N_NODES) * i);
        network->node[i].pos.y = START_RADIUS*sin((2*M_PI / N_NODES) * i);

	}

}

void som_run_network(Som_network* network, Position pos[1000], int n_cities){
    time_t start_time = time(NULL);
    time_t end_time = time(NULL);

    for(long int i = 0; end_time - start_time < RUNTIME_US; i++){

        for(int city = 0; city < n_cities; city++){
            int closest_node = som_determine_closest_node(network, pos[city]);
            som_update_node_positions(network, network->node[closest_node].pos, pos[city], city);
        }


        //update_learning_rate(i);
        //update_neighborhood_variance(i);
        //update_neighborhood_radius();

        end_time = time(NULL);
    }
}

void som_print_path(Som_network* network){
    for(int i = 0; i < N_NODES; i++){
        printf("%d\n", network->node[i].closest_city + 1);
    }
}


//*************************************************************************//
//                                                                         //
//                         Private functions                               //
//                                                                         //
//*************************************************************************//
void som_scale_input(Position pos[1000], int n_elements, double scaling_value){
    for(int i = 0; i < n_elements; i++){
        pos[i].x *= scaling_value;
        pos[i].y *= scaling_value;
    }

}

double som_distance(Position pos_a, Position pos_b){
    double squared_distance_x = pow((pos_a.x - pos_b.x), 2);
    double squared_distance_y = pow((pos_a.y - pos_b.y), 2);

	return sqrt(squared_distance_x + squared_distance_y);
}

//Returns the index of the node closest to city_pos
int som_determine_closest_node(Som_network* network, Position city_pos){
	int closest_node = 0;
    double distance = 10000000;

	for(int i = 0; i < N_NODES; i++){
        double temp = som_distance(network->node[i].pos, city_pos);

		if(temp < distance){
            distance = temp;
			closest_node = i;
		}

	}

	return closest_node;
}

void som_update_node_positions(Som_network* network, Position closest_node_pos, Position city_pos, int city_index){

    for(int i = 0; i < N_NODES; i++){
        Position cur_pos = network->node[i].pos;
        double d = som_distance(closest_node_pos, cur_pos);
        //printf("d1: %lf\n", d);

        //Move all nodes that are within a certain radius of the winning node
        if(d < neighborhood_radius){

            //printf("radius: %lf\n", neighborhood_radius);
            if(N_NODES - d < d){
                d = N_NODES - d;
            }

            printf("d2: %lf\n", d);
            double f = exp(-d*d/(neighbor_var*neighbor_var)); //neighborhood function
            //printf("f: %lf\n", f);
            Position new_pos;
            new_pos.x = cur_pos.x + learning_rate * f * (city_pos.x - cur_pos.x);
            new_pos.y = cur_pos.y + learning_rate * f * (city_pos.y - cur_pos.y);


            network->node[i].pos = new_pos;
            network->node[i].closest_city = city_index;
        }

    }
}


void update_learning_rate(int iteration){
    learning_rate = 1.0/iteration;
    printf("learning rate: %lf\n", learning_rate);
}


void update_neighborhood_variance(int iteration){
    neighbor_var = neighbor_var * (1 - 0.01 * iteration);
}

void update_neighborhood_radius(){
    neighborhood_radius -= NEIGHBORHOOD_RADIUS_STEP;
}
