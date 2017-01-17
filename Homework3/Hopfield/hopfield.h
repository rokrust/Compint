#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define HEIGHT 10
#define WIDTH 20
#define N_IMAGES_MAX 10
#define N_NODES HEIGHT*WIDTH
#define N_PIXELS HEIGHT*WIDTH
#define NOISE_PERCENTAGE 0.1
#define CORRECT_PIXEL_PERCENTAGE_THRESHOLD 0.7

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

//Returns the image most similar to output_image.
//Returns -1 if under 70 percent of the pixels are equal.
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
	
	//Number of correct pixels under given threshold percentage		
	if(((double)highest_pixel_count)/N_PIXELS < 
		CORRECT_PIXEL_PERCENTAGE_THRESHOLD){
		return -1;
	}

	return most_similar_image;
}

//Used to push the network out of a local minimum
void hopfield_add_noise(int image[HEIGHT][WIDTH]){
	int n_random_pixels = N_PIXELS*NOISE_PERCENTAGE;
	
	//Flip random pixels
	for(int i = 0; i < n_random_pixels; i++){
		int rand_row = rand() % HEIGHT;
		int rand_col = rand() % WIDTH;

		if(image[rand_row][rand_col] == 1){
			image[rand_row][rand_col] = -1;
		}

		else if(image[rand_row][rand_col] == -1){
			image[rand_row][rand_col] = 1;
		}
	}

}

void hopfield_recognize_pattern(Hopfield_network* network,
				int training_image[N_IMAGES_MAX][HEIGHT][WIDTH], 
				int distorted_image[N_IMAGES_MAX][HEIGHT][WIDTH], 
				int n_training_images, int n_distorted_images,
				int most_similar_image[n_distorted_images]){

	for(int i = 0; i < n_distorted_images; i++){
		
		//Run until no changes are made to the image
		while(hopfield_network_output(network, distorted_image[i])){;}
		
		most_similar_image[i] = hopfield_find_most_similar_image
								(training_image, 
								 distorted_image[i], 
								 n_training_images);
		
		//restart iteration
		if(most_similar_image[i] == -1){
			hopfield_add_noise(distorted_image[i]);
			i--;
		}
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

void hopfield_print_recognized_images(
				int image[N_IMAGES_MAX][HEIGHT][WIDTH], 
				int n_output_images){
	
	for(int i = 0; i < n_output_images; i++){
		int image_index = most_similar_image[i];
		hopfield_print_int_image(image[image_index]);

		if(i != n_output_images - 1){
			printf("-\n");
		}
	}



}

