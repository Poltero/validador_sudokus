#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>

#define NUM_OF_THREADS 18

typedef struct {
	int* list;
	int type;
	int id;
	int position;
}Threads_args;

typedef struct {
	int* list;
	int* errors;
	int* types;
}Threads_result;

typedef struct {
	int* list;
	int size;
}Vector;


int read_sudoku(const char* filename, int sudoku[9][9]);

void* validate_list(void* list);

int count_indexes_not_null_from_list(int* l, int size);

int* delete_positions_nulls_from_list(int* l, int size, int num_positions_nulls);

Threads_result threads_result;


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

	/*printf("After: \n");
	i = 0;
	while(i < 18) {
		printf("%d ", threads_result.errors[i]);
		i++;
	}*/

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

		//printf("%d\n=======\n", threads_result.errors[4]);

		//Throw threads
		pthread_create(&threads[count], NULL, validate_list, &args[count]);
		pthread_create(&threads[count+1], NULL, validate_list, &args[count+1]);

		printf("%d, i = %d\n", count, i);

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

		i = 0;
		int index;
		while(i < count_errors) {
			index = indexes[i]-1; 
			printf("Tipo: %d\n", threads_result.types[index]);
			printf("Posicion: %d\n", threads_result.list[index]);
			i++;
		}
	}


	pthread_exit(NULL);

	return 0;
}


int read_sudoku(const char* filename, int sudoku[9][9]) {
	int readCounteri = 0;
	int readCounterj = 0;
	int readCounter = 0;

	int fd = open(filename, O_RDONLY);

	//If error in open file
	if(fd == -1)
		return -1;

	char buffer[1];
	ssize_t bytesRead;

	while((bytesRead = read(fd, &buffer, 1)) > 0) {
		if((readCounter % 2) == 0) {
			sudoku[readCounterj][readCounteri] = atoi(buffer);
			readCounteri++;

			if(readCounteri > 8) {
				readCounteri = 0;
				readCounterj++;
			}
		}

		readCounter++;
	}

	return 0;
}


void* validate_list(void* ptr) {
	int result, i=1,j;

	Threads_args* args = (Threads_args*)ptr;
	int* list = args->list;

	while(i<=9) {
		j=0;
		result = 0;
		while(j<9) {
			if(i == list[j]) {
				result = 1;
				break;
			}
			j++;
		}
		if(result == 0) {
			threads_result.list[args->id] = args->position;
			threads_result.errors[args->id] = args->id+1;
			threads_result.types[args->id] = args->type;
			//printf("%d\n--------ee\n", threads_result.errors[4]);
			pthread_exit(NULL);	
		}
		i++;
	}

	pthread_exit(NULL);
}

/*Cuenta cuantos indices distintos de NULL exisiten de en una lista*/
int count_indexes_not_null_from_list(int* l, int size) {
	int i = 0, counter = 0;

	while(i < size) {
		if(l[i] != 0)
			counter++;
		i++;
	}

	return counter;
}

int* delete_positions_nulls_from_list(int* l, int size, int num_positions_nulls) {
	int* list_result = (int*) realloc(NULL, num_positions_nulls*sizeof(int));

	int i = 0, j = 0;

	while(i < size) {
		if(l[i] != 0) {
			list_result[j] = l[i];
			j++; 
		}
		i++;
	}

	return list_result;
}


