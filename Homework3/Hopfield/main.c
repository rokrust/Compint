#include "hopfield.h"

#include <stdio.h>

int main(){
	FILE* fp = fopen("testInput22A.txt", "r");

	int training_image[N_IMAGES_MAX][HEIGHT][WIDTH];
	int distorted_image[N_IMAGES_MAX][HEIGHT][WIDTH];

	//Read traning images
	int n_training_images = 
			hopfield_read_training_data(fp, training_image);
	
	//Read distorted images
	int n_distorted_images = 
			hopfield_read_running_data(fp, distorted_image);

	//Save patters in network
	Hopfield_network *network = 
			hopfield_init(training_image, n_training_images);
	
	//Recognize pattern
	for(int i = 0; i < n_distorted_images; i++){
		
		//Run until no changes are made to the image
		while( hopfield_network_output(network, distorted_image[i]) ){;}
	
	}

	hopfield_print_all_output_images(distorted_image, n_distorted_images);

	return 0;
}
