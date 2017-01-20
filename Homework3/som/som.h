#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>


#define RUNTIME_S 50
#define NEIGHBORHOOD_RADIUS_STEP 0.001
#define AMOUNT_OF_NEIGHBORS_TO_MOVE 3
#define M_PI 3.1415925

int N_NODES;
double learning_rate = 0.3;
double neighbor_var = 500;
double neighborhood_radius = 500;
double termination_threshold;

struct Position{
    double x;
    double y;

}; typedef struct Position Position;

struct Ellipse{
    Position center;
    double a;
    double b;

}; typedef struct Ellipse Ellipse;


//Might have to add a variable for affiliated city
struct Node{
    Position pos;
    int* closest_city;
    int n_closest_cities;

}; typedef struct Node Node;

struct Som_network{
    Node* node;

}; typedef struct Som_network Som_network;


double som_distance(Position pos_a, Position pos_b);
int som_determine_closest_node(Som_network* network, Position city_pos);
void som_update_node_positions(Som_network* network, int winning_node_index, Position city_pos);
void som_scale_input(Position pos[1000], int n_elements, double scaling_value);
int som_terminate(Som_network* network, Position pos[1000], int n_cities);
void update_tuning_parameters();



//*************************************************************************//
//                                                                         //
//                            Public functions                             //
//                                                                         //
//*************************************************************************//


//Puts input into data and returns number of elements
int som_read_data(int data[1000], Position pos[1000], Ellipse* ellipse){
	FILE* fp = fopen("testInput23A.txt", "r");
    char input_string[100];

    int largest_x = -10000000;
    int largest_y = -10000000;
    int smallest_x = 10000000;
    int smallest_y = 10000000;
	
	int i = 0;
    while(fgets(input_string, 200, fp) != 0){
        sscanf(input_string, "%d,%lf,%lf\n", &(data[i]), &(pos[i].x), &(pos[i].y));
        int x = pos[i].x;
        int y = pos[i].y;

        //For calculating initial ellipse for nodes
        if(x > largest_x){
            largest_x = x;
        }

        if(x < smallest_x){
            smallest_x = x;
        }

        if(y > largest_y){
            largest_y = y;
        }

        if(y < smallest_y){
            smallest_y = y;
        }

		i++;
	}

    ellipse->a = (largest_x - smallest_x) / 2;
    ellipse->b = (largest_y - smallest_y) / 2;

    ellipse->center.x = (double)largest_x - ellipse->a;
    ellipse->center.y = (double)largest_y - ellipse->b;

    neighborhood_radius = (ellipse->a + ellipse->b) * 2;

    neighbor_var = i/5;
    termination_threshold = neighborhood_radius/15;


    N_NODES = (int)(1.4 * i);

	return i;	
}

//Should put all nodes evenly on a circle
void som_init_network(Som_network* network, Ellipse ellipse){
    network->node = malloc(sizeof(Node)*N_NODES);

    for(int i = 0; i < N_NODES; i++){
        network->node[i].closest_city = malloc(sizeof(int)*N_NODES);
    }

	for(int i = 0; i < N_NODES; i++){
        network->node[i].pos.x = ellipse.center.x + ellipse.a * cos((2*M_PI / N_NODES) * i);
        network->node[i].pos.y = ellipse.center.y + ellipse.b * sin((2*M_PI / N_NODES) * i);

        network->node[i].n_closest_cities = 0;
	}


}

void som_run_network(Som_network* network, Position pos[1000], int n_cities){
    time_t start_time = time(NULL);
    time_t end_time = time(NULL);

    int terminate = 0;

    for(long int j = 0; end_time - start_time < RUNTIME_S && !terminate; j++){
        for(int city = 0; city < n_cities; city++){

            int closest_node = som_determine_closest_node(network, pos[city]);
            som_update_node_positions(network, closest_node, pos[city]);
        }


        update_tuning_parameters();
        terminate = som_terminate(network, pos, n_cities);
        end_time = time(NULL);
    }

    for(int i = 0; i < n_cities; i++){
        int node_index = som_determine_closest_node(network, pos[i]);

        int n_closest_cities = network->node[node_index].n_closest_cities++;

        network->node[node_index].closest_city[n_closest_cities] = i;
    }
}

void som_print_path(Som_network* network){
    for(int i = 0; i < N_NODES; i++){
        int n_closest_cities = network->node[i].n_closest_cities;
        Position cur_pos = network->node[i].pos;

        for(int j = 0; j < n_closest_cities; j++){
            printf("%d\n", network->node[i].closest_city[j] + 1);
        }
    }
}


//*************************************************************************//
//                                                                         //
//                         Private functions                               //
//                                                                         //
//*************************************************************************//
int som_terminate(Som_network* network, Position pos[1000], int n_cities){
    double longest_distance = 0;

    for(int i = 0; i < n_cities; i++){
        double city_shortest_distance = 1000000;
        for(int j = 0; j < N_NODES; j++){
            double cur_distance = som_distance(network->node[j].pos, pos[i]);
            if(cur_distance < city_shortest_distance){
                city_shortest_distance = cur_distance;
            }

        }

        if(city_shortest_distance > longest_distance){
            longest_distance = city_shortest_distance;

        }
    }
    printf("threshold %lf\n", termination_threshold);
    printf("longest distance %lf\n", longest_distance);
    if(longest_distance > 0.5){
        return 0;
    }

    else{
        return 1;
    }
}


int som_ring_buffer_index(int i){
    if(i < 0){
        return N_NODES + (i % N_NODES);
    }

    else{
        return i % N_NODES;
    }
}


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
    int closest_node;
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

void som_update_node_positions(Som_network* network, int winning_node_index, Position city_pos){

    Position closest_node_pos = network->node[winning_node_index].pos;

    //Iterate through neighbors of the winning node and check if they are close enough
    //to qualify for moving
    int start_value = winning_node_index - AMOUNT_OF_NEIGHBORS_TO_MOVE;
    int end_value = winning_node_index + AMOUNT_OF_NEIGHBORS_TO_MOVE + 1;
    for(int i = start_value; i < end_value; i++){
        int node_index = som_ring_buffer_index(i);

        Position current_node_pos = network->node[node_index].pos;

        double d = som_distance(closest_node_pos, current_node_pos);


        //Move all neighboring nodes that are within a certain radius of the winning node
        if(d < neighborhood_radius){

            double f = exp(-(d*d/(neighbor_var*neighbor_var))); //neighborhood function

            Position new_pos;
            new_pos.x = current_node_pos.x + learning_rate * f * (city_pos.x - current_node_pos.x);
            new_pos.y = current_node_pos.y + learning_rate * f * (city_pos.y - current_node_pos.y);

            network->node[node_index].pos = new_pos;

        }
    }


}

void update_learning_rate(){
    if(learning_rate < 0.01){
        return;
    }

    learning_rate -= 0.001;
}


void update_neighborhood_variance(){
    if(neighbor_var < 10){
        return;
    }

    neighbor_var -= 0.01;
}

void update_neighborhood_radius(){
    if(neighborhood_radius < 4){
        return;
    }

    neighborhood_radius -= NEIGHBORHOOD_RADIUS_STEP;
}

void update_tuning_parameters(){
    update_learning_rate();
    update_neighborhood_variance();
    update_neighborhood_radius();
}
