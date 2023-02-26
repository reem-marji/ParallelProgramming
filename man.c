#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <time.h>   
#include <math.h>

#define height 600
#define width 800
#define MAX_ITERATION 1000


int MandelBrot(double x, double y) {

    double real = x;
    double imag = y;

    for (int i = 0; i < MAX_ITERATION; i++) {

        double rsq = real * real;
        double isq = imag * imag;

        if (rsq + isq > 4) {
            break;
        }
	
	// compute formula
        imag = 2 * real * imag + y;
        real = rsq - isq + x;
    }
    // number of iterations
    return i;

}


int main(int argc, char** argv) {

    double running_time = 0.0;
    clock_t begin = clock();

    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

	
    // bounds

    double ylower = -1.0;
    double yupper = 1.0;

    double xlower = -2.0;
    double xupper = 1.0;
   


    // dividing the image into four horizontal strips 

    int heightOfStrip = height / size;
    int start = rank * heightOfStrip;
    int end = (rank + 1) * heightOfStrip;

    if (rank == size - 1) {
        end = height;
    }

    int* image = (int*) malloc(width * (end - start) * sizeof(int));


    int m, n, i;

    // outer loop covers designated height 
    for (n = start; n < end; n++) {
       
        // inner loop covers width of strip
        for (m = 0; m < width; m++) {

            double real = xlower + (double) m / width * (xupper - xlower);
            double imag = ylower + (double) n / height * (yupper - ylower);

            i = mandelbrot(real, imag);

            image[(n - start) * width + m] = i;
        }
    }


    // Gathering the computations of each strip 

    if (rank == 0) { // master 

        int* finalImage = (int*) malloc(WIDTH * HEIGHT * sizeof(int));

        MPI_Gather(image, width * heightOfStrip, MPI_INT, finalImage, width * heightOfStrip, MPI_INT, 0, MPI_COMM_WORLD);


        printf("P3\n%d %d\n255\n", WIDTH, HEIGHT);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {

                i = finalImage[y * width + x];

                int r = (i * 13) % 256;
                int g = (i * 17) % 256;
                int b = (i * 23) % 256;

                printf("%d %d %d\n", r, g, b);

            }

        }

        free(finalImage);

    } else {

        MPI_Gather(image, width*heightOfStrip, MPI_INT, NULL, width*heightOfStrip, MPI_INT, 0, MPI_COMM_WORLD);

	}


    free(image);
    MPI_Finalize();
  
 
    clock_t endr = clock();
    running_time += (double)(endr - begin);
    printf("The running time is %f", running_time);


return 0;

}

