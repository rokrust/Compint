#include "hopfield.h"

#include <stdio.h>

int main(){
	FILE* fp = fopen("testInput22A.txt", "r");

	int training_image[N_IMAGES_MAX][HEIGHT][WIDTH];
	int distorted_image[N_IMAGES_MAX][HEIGHT][WIDTH];
	
	printf("Training data\n");
	int n_training_images = 
			hopfield_read_training_data(fp, training_image);
	
	printf("Running data\n");
	int n_distorted_images = 
			hopfield_read_running_data(fp, distorted_image);

	printf("Init\n");
	Hopfield_network *network = 
			hopfield_init(training_image, n_training_images);

	return 0;
}
