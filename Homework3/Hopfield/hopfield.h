#include <stdio.h>
#include <stdlib.h>

#define HEIGHT 10
#define WIDTH 20
#define N_IMAGES_MAX 10
#define N_NODES HEIGHT*WIDTH

struct Node{
	int weight[HEIGHT][WIDTH];
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

				//For initializing every weight in the current node
				for(int i = 0; i < HEIGHT; i++){	
					for(int j = 0; j < WIDTH; j++){
						//initial weight is the product of the two 	
						//pixel values
						int initial_weight = 		
							training_image[image][row][col] * 
							training_image[image][i][j];

						network->node[row][col].weight[i][j] += 
												initial_weight;

					}
				}

				network->node[row][col].weight[row][col] = 0;
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
	
	return image_index;
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
		
		//Reads the lines between the images
		stay_in_loop = fgets(inputString, 200, fp);
		image_index++;
	}

	return image_index;
	
}

int hopfield_node_output(Node node, int image[HEIGHT][WIDTH]){
	int output = 0;
	
	for(int i = 0; i < HEIGHT; i++){
		for(int j = 0; j < WIDTH; j++){
			int w_ij = node.weight[i][j];
			
		
			output += w_ij*image[i][j];
			
		}
	}
	
	return output;	
}

//Saves the resulting image in output_image, 
//compares with old_image to check if any updates 
//where made and returns this as a boolean (termination check)
int hopfield_network_output(Hopfield_network* network,
						int output_image[HEIGHT][WIDTH]){
	
	int old_image[HEIGHT][WIDTH];
	
	//copy old image 
	for(int row = 0; row < HEIGHT; row++){
		for(int col = 0; col < WIDTH; col++){
			old_image[row][col] = output_image[row][col];
		}
	}
	
	int updates_made = 0;

	for(int row = 0; row < HEIGHT; row++){
		for(int col = 0; col < WIDTH; col++){
			int node_output = 
				hopfield_node_output(network->node[row][col], 
									 old_image);
			if(node_output >= 0){
				output_image[row][col] = 1;
			}

			else{
				output_image[row][col] = -1;
			}
			
			//check for changes
			if(output_image[row][col] != old_image[row][col]){
				updates_made = 1;
			}

		}	
	}

	return updates_made;
}

void hopfield_create_char_image(int int_image[HEIGHT][WIDTH], char char_image[HEIGHT][WIDTH]){
	for(int row = 0; row < HEIGHT; row++){
		for(int col = 0; col < WIDTH; col++){
			if(char_image[row][col] == '*'){
				int_image[row][col] = 1;
			}
			
			else if(char_image[row][col] == '.'){
				int_image[row][col] = -1;
			}
		
		}
	}	
}


int hopfield_find_most_similar_image(int image[N_IMAGES_MAX][HEIGHT][WIDTH], int output_image[HEIGHT][WIDTH], int n_images){
	int most_similar_image;
	int highest_pixel_count = 0;
	int similar_pixels = 0;

	for(int i = 0; i < n_images; i++){
		for(int row = 0; row < HEIGHT; row++){
			for(int col = 0; col < WIDTH; col++){
				
				if(image[i][row][col] == output_image[row][col]){
					similar_pixels++;
				}
			}

		}
		
		if(similar_pixels > highest_pixel_count){
				most_similar_image = i;
				highest_pixel_count = similar_pixels;
				
		}

		similar_pixels = 0;
	}
	
	return most_similar_image;
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
			if(image[row][col] >= 0){
				printf("*");
			}

			else if(image[row][col] < 0){
				printf(".");
			}

		}
		printf("\n");
	}
}

void hopfield_print_all_output_images(int output_image[N_IMAGES_MAX][HEIGHT][WIDTH], int n_images){
	for(int i = 0; i < n_images; i++){

		hopfield_print_int_image(output_image[i]);

		if(i != n_images - 1){
			printf("-\n");
		}
	}
	

}


void hopfield_compare_and_print_all_output_images(
					int image[N_IMAGES_MAX][HEIGHT][WIDTH], 
					int output_image[N_IMAGES_MAX][HEIGHT][WIDTH], 
					int n_images){

	int image_index;
	
	for(int i = 0; i < n_images; i++){
		image_index = hopfield_find_most_similar_image(
					image, output_image[i], n_images);
		
		hopfield_print_int_image(image[image_index]);

		if(i != n_images - 1){
			printf("-\n");
		}
	}
}
