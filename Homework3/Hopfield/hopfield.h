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


Hopfield_network* hopfield_init(int training_image[N_IMAGES_MAX][HEIGHT][WIDTH], int n_training_images){
	Hopfield_network* network;

	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			network->node[i][j] = node;
		}
	}
	
	//Iterate through all the images
	for(int image = 0; image < n_training_images; image++){
		printf("Image: %d\n", image);

		//Iterate through all the nodes
		for(int row = 0; row < HEIGHT; row++){
			printf("Row: %d\n", row);

			for(int col = 0; col < WIDTH; col++){
				printf("Col: %d\n", col);

				network->node[row][col].row = row;
				network->node[row][col].col = col;				
				network->node[row][col].weight[row][col] = 0;
				
				//For initializing every weight in the current node
				for(int i = 0; i < HEIGHT; i++){	
					printf("i: %d\n", i);

					for(int j = i + 1; j < WIDTH; j++){
						printf("j :%d\n", j);
						//initial weight is the product of the two 	
						//pixel values
						double initial_weight = 			
							training_image[image][row][col] * 
							training_image[image][i][j];

						network->node[row][col].weight[i][j] += 
												initial_weight;
							

						network->node[row][col].weight[j][i] += 
												initial_weight;

					}
				}
			}
		}
	}
	
	return network;
}

int hopfield_read_training_data(FILE* fp, int training_image[N_IMAGES_MAX][HEIGHT][WIDTH]){
	char inputString[WIDTH];

	int image_index = 0;
	int row = 1;

	fgets(inputString, 200, fp);
	while(inputString[1] != '-'){
		for(row; row < HEIGHT + 1; row++){
			//printf("row: %d, %s", row, inputString);
			for(int col = 0; col < WIDTH; col++){
				
				if(inputString[col] == '*'){
					training_image[image_index][row][col] = 1;
				}

				else if(inputString[col] == '.'){
					training_image[image_index][row][col] = -1;
				}
			}
			//printf("row: %d\n", row);

			fgets(inputString, 200, fp);
		}
		row = 0;

		image_index++;
	}

	return image_index - 1;
}


int hopfield_read_running_data(FILE* fp, int distorted_image[N_IMAGES_MAX][HEIGHT][WIDTH]){
	char inputString[WIDTH];

	int image_index = 0;
	
	while(fgets(inputString, WIDTH, fp) != 0){
		for(int row = 0; row < HEIGHT; row++){
			for(int col = 0; col < WIDTH; col++){
					
				if(inputString[col] == '*'){
					distorted_image[image_index][row][col] = 1;
				}

				else if(inputString[col] == '.'){
					distorted_image[image_index][row][col] = -1;
				}

			}
			
			fgets(inputString, WIDTH, fp);
		}

		image_index++;
		fgets(inputString, WIDTH, fp);
	
	}	

	return image_index;

}

double hopfield_node_output(Hopfield_network* network, int row, int col){
	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			double w_ij = network->node[row][col].weight[i][j];
			
			
		}
	}
	
}

void hopfield_update_weights(Hopfield_network* network){
	
}
