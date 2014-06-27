/*#include <stdio.h>
#include <stdlib.h>*/
#include <string.h>

typedef struct
{
	int interval;
	int size;
	int hollow;
}Data_vector;

typedef struct 
{
	char* list;
	Data_vector data;

}Vector_chars;

typedef struct
{
	Vector_chars* list;
	Data_vector data;

}Vector_pointers_char;



Vector_pointers_char* create_vector_of_pointers_chars(int interval) {

	Vector_pointers_char* v = (Vector_pointers_char*) realloc(NULL, sizeof(Vector_pointers_char));
	v->data.interval = interval;
	v->data.size = 0;

	v->list = (Vector_chars*) realloc(NULL, interval*sizeof(Vector_chars));
	v->data.hollow = interval;

	return v;
}

Vector_chars* create_vector_of_chars(int interval) {
	Vector_chars* v = (Vector_chars*) realloc(NULL, sizeof(Vector_chars));
	v->data.interval = interval;
	v->data.size = 0;

	v->list = (char*) realloc(NULL, interval*sizeof(v->list));
	v->data.hollow = interval;

	return v;
}

void push_back_char(Vector_chars* v, char c) {
	if(v->data.hollow == 0) {
		v->list = (char*) realloc(v->list, v->data.size*v->data.interval*sizeof(v->list));
		v->data.hollow = v->data.interval;
	}

	v->list[v->data.size] = c;
	v->data.size++;
	v->data.hollow--;
}

void push_back_char_pointer(Vector_pointers_char* v_pointers_char, Vector_chars* v_chars) {
	if(v_pointers_char->data.hollow == 0) {
		v_pointers_char->list = (Vector_chars*) realloc(v_pointers_char->list, v_pointers_char->data.size*v_pointers_char->data.interval*sizeof(Vector_chars));
		v_pointers_char->data.hollow = v_pointers_char->data.interval;
	}


	v_pointers_char->list[v_pointers_char->data.size] = *v_chars;
	v_pointers_char->data.size++;
	v_pointers_char->data.hollow--;
}


/*int main(int argc, char const *argv[])
{
	Vector_pointers_char* vector = create_vector_of_pointers_chars(5);

	Vector_chars* cadena1 = create_vector_of_chars(10);

	push_back_char(cadena1, 'a');
	push_back_char(cadena1, 'l');
	push_back_char(cadena1, 'o');
	push_back_char(cadena1, 'h');

	push_back_char_pointer(vector, cadena1);

	int i = vector->list[0].data.size;

	while(i >= 0) {
		printf("%c", vector->list[0].list[i]);
		i--;
	}


	return 0;
}*/