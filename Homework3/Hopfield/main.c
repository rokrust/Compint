#include "hopfield.h"

#include <stdio.h>

int main(){
	FILE* fp = fopen("testInput22A.txt", "r");

	int training_image[N_IMAGES_MAX][HEIGHT][WIDTH];
	int distorted_image[N_IMAGES_MAX][HEIGHT][WIDTH];
	char output_image[HEIGHT][WIDTH];	

	int n_training_images = 
			hopfield_read_training_data(fp, training_image);
	
	int n_distorted_images = 
			hopfield_read_running_data(fp, distorted_image);

	Hopfield_network *network = 
			hopfield_init(training_image, n_training_images);

		for(int i = 0; i < n_distorted_images-1; i++){
			hopfield_network_output(network, distorted_image[i], 
									output_image);
			hopfield_print_image(output_image);
			printf("-\n");
		}

		hopfield_network_output(network, 
								distorted_image[n_distorted_images-1], 
								output_image);
		hopfield_print_image(output_image);

	return 0;
}
