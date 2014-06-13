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

Threads_result threads_result;

int read_sudoku(const char* filename, int sudoku[9][9]);

void* validate_list(void* list);

int count_indexes_not_null_from_list(int* l, int size);

int* delete_positions_nulls_from_list(int* l, int size, int num_positions_nulls);