#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>


int main(int argc, char** argv)
{
	int sudoku[9][9];

	read_sudoku("fichero.txt", sudoku);

	pthread_t threads[18];

	Threads_args* args = (Threads_args*) realloc(NULL, 18*sizeof(Threads_args));
	threads_result.list = (int*) calloc(18, sizeof(int));
	threads_result.errors = (int*) calloc(18, sizeof(int));
	threads_result.types = (int*) calloc(18, sizeof(int));

	int i = 0, count = 0, j;


	while(i < 9) {
		args[count].list = (int*) realloc(NULL, 9*sizeof(int));
		args[count].type = 0;
		args[count].id = count;
		args[count].position = i+1;

		args[count+1].list = (int*) realloc(NULL, 9*sizeof(int));
		args[count+1].type = 1;
		args[count+1].id = count+1;
		args[count+1].position = i+1;

		//fill row and col
		j = 0;
		while(j<9) {
			args[count].list[j] = sudoku[i][j];
			args[count+1].list[j] = sudoku[j][i];
			j++;
		}


		//Throw threads
		pthread_create(&threads[count], NULL, validate_list, &args[count]);
		pthread_create(&threads[count+1], NULL, validate_list, &args[count+1]);

		count +=2;
		i++;

		

	}

	i = 0;
	while(i < 18) {
		pthread_join(threads[i], NULL);
		i++;
	}


	int count_errors = count_indexes_not_null_from_list(threads_result.errors, NUM_OF_THREADS);

	if(count_errors == 0) {
		printf("Fichero correcto\n");
	} 
	else
	{
		int* indexes = delete_positions_nulls_from_list(threads_result.errors, NUM_OF_THREADS, count_errors);

		print_errors(indexes, count_errors);
	}


	pthread_exit(NULL);


	return 0;
}



