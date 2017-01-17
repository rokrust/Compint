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

	//Save patterns in network
	Hopfield_network *network = 
			hopfield_init(training_image, n_training_images);
	
	//Recognize pattern
	int most_similar_image[n_distorted_images];
	hopfield_recognize_pattern(network, training_image,
 							   distorted_image, 
							   n_training_images, n_distorted_images, 
							   most_similar_image);

	//Print results
	hopfield_print_recognized_images(training_image, 
									  n_distorted_images);

	return 0;
}
