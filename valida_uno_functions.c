#include "valida_uno_functions.h"


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