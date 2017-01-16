#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 10
#define WIDTH 20
#define N_IMAGES_MAX 10
#define N_NODES HEIGHT*WIDTH

struct Node{
	double weight[HEIGHT][WIDTH];
	int row, col;
	
}; typedef struct Node Node;


struct Hopfield_network{
	Node node[HEIGHT][WIDTH];

}; typedef struct Hopfield_network Hopfield_network;


void hopfield_print_int_image(int image[HEIGHT][WIDTH]);


Hopfield_network* hopfield_init(int training_image[N_IMAGES_MAX][HEIGHT][WIDTH], int n_training_images){
	Hopfield_network* network = malloc(sizeof(Hopfield_network));

	
	//Iterate through all the images
	for(int image = 0; image < n_training_images; image++){

		//Iterate through all the nodes
		for(int row = 0; row < HEIGHT; row++){

			for(int col = 0; col < WIDTH; col++){
			
				network->node[row][col].row = row;
				network->node[row][col].col = col;				
				network->node[row][col].weight[row][col] = 0;
				
				//For initializing every weight in the current node
				for(int i = 0; i < HEIGHT; i++){	
					for(int j = 0; j < WIDTH; j++){
						//initial weight is the product of the two 	
						//pixel values
						double initial_weight = 		
							training_image[image][row][col] * 
							training_image[image][i][j];
						
						network->node[row][col].weight[i][j] += 
												initial_weight;

					}
				}
			}
		}
	}

	return network;
}

int hopfield_read_training_data(FILE* fp, int training_image[N_IMAGES_MAX][HEIGHT][WIDTH]){
	char inputString[WIDTH+2]; //Include newline and null
	inputString[WIDTH+1] = '\0';

	int image_index = 0;


	while(inputString[1] != '-'){

		for(int row = 0; row < HEIGHT; row++){
			fgets(inputString, 200, fp);

			for(int col = 0; col < WIDTH; col++){
				
				if(inputString[col] == '*'){
					training_image[image_index][row][col] = 1;
				}

				else if(inputString[col] == '.'){
					training_image[image_index][row][col] = -1;
				}
			}
		}
		fgets(inputString, 200, fp);
		image_index++;
	}
	
	return image_index - 1;
}


int hopfield_read_running_data(FILE* fp, int distorted_image[N_IMAGES_MAX][HEIGHT][WIDTH]){
	char inputString[WIDTH+2];
	inputString[WIDTH+1] = '\0';

	int image_index = 0;
	int stay_in_loop = 1;

	while(stay_in_loop){

		for(int row = 0; row < HEIGHT; row++){
			fgets(inputString, 200, fp);

			for(int col = 0; col < WIDTH; col++){
					
				if(inputString[col] == '*'){
					distorted_image[image_index][row][col] = 1;
				}
	
				else if(inputString[col] == '.'){
					distorted_image[image_index][row][col] = -1;
				}

			}
			
		}
		
		stay_in_loop = fgets(inputString, 200, fp);
		image_index++;
	}

	return image_index;
	
}

double hopfield_node_output(Node node, int image[HEIGHT][WIDTH]){
	double output = 0;
	
	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			double w_ij = node.weight[i][j];
			
		
			output += w_ij*image[i][j];	
			
		}
	}
	
}

void hopfield_network_output(Hopfield_network* network, int image[HEIGHT][WIDTH], char output_image[HEIGHT][WIDTH]){

	for(int row = 0; row < HEIGHT; row++){
		for(int col = 0; col < WIDTH; col++){
			double node_output = 
				hopfield_node_output(network->node[row][col], image);
			
			if(node_output >= 0){
				output_image[row][col] = '*';
			}

			else{
				output_image[row][col] = '.';
			}
		}
	}
}

void hopfield_update_weights(Hopfield_network* network){
	
}

void hopfield_print_image(char image[HEIGHT][WIDTH]){
	for(int row = 0; row < HEIGHT; row++){
		for(int col = 0; col < WIDTH; col++){
			printf("%c", image[row][col]);
		}
		printf("\n");
	}
}

void hopfield_print_int_image(int image[HEIGHT][WIDTH]){
	for(int row = 0; row < HEIGHT; row++){
		for(int col = 0; col < WIDTH; col++){
			if(image[row][col] == 1){
				printf("*");
			}
			else if(image[row][col] == -1){
				printf(".");
			}
		}
		printf("\n");
	}
}
