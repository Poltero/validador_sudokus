#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>


struct Threads_args {
	int* list;
	int type;
	int id;
	int position;
};

struct  Threads_result {
	int* list;
	int* errors;
	int* type;
};


int read_sudoku(const char* filename, int sudoku[9][9]);

int validate_list(int* list);

Threads_result threads_result;


int main(int argc, char** argv)
{
	int sudoku[9][9];

	pthread_t threads[18];

	Threads_args* args = (Threads_args*) realloc(NULL, 18*sizeof(int));
	threads_result.list = (int*) calloc(18, sizeof(int));
	threads_result.errors = (int*) calloc(18, sizeof(int));
	threads_result.type = (int*) calloc(18, sizeof(int));

	int i = 0, count = 0, j;

	while(i < 9) {
		args[count].list = (int*) realloc(NULL, 9*sizeof(int));
		args[count].type = 0;
		args[count].id = count;
		args[count].position = i+1;

		args[count+1].list = (int*) realloc(NULL, 9*sizeof(int));
		args[count+1].id = count+1;
		args[count+1].position = i+1;

		//fill row and col
		j = 0;
		while(j<9) {
			args[count].list = sudoku[i][j];
			args[count+1].list = sudoku[j][i];
			j++;
		}

		//Throw threads

		count +=2;
		i++;

		

	}



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


int validate_list(int* list) {
	int result, i=1,j;

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
		if(result == 0)
			return 0;
		i++;
	}

	return 1;
}