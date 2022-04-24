#include <stdio.h>
#include <stdlib.h>
#include "string1.h"
#include <string.h>
#include <time.h>
//#include <vld.h>

typedef enum {
	OK = 0,
	OF = 1,
	UN = 2,
	DB = 3,
	CZ = 4
} errors;

typedef struct tree {
	string* key, * info;
	struct tree* parent;
	struct tree* left;
	struct tree* right;
} tree;

void create_tree(tree** t);

int add_e(tree* t, string* key, string* info, string** data);

int scan(tree* t, string* key, tree** info);

int scan_max(tree* t, tree** info);

int del_e(tree** t, string* key);

int print_tree(tree* t, string* key1, string* key2);

int format_tree(tree* t, int p);

int from_file(tree* t, string* file_name);

void free_tree(tree* t);

//int add_random(tree* t, int size, int flag);
//
//void test(tree** t, int size, float* tscan, float* tdel);
//
//string** create_arr_s_random(int size);
//
//string* create_s_random(int size, int ran);
//
//void stack_test();
//
//void test_add();

int viz_tree(tree* t, FILE* file);

int add_unique_random_str_to_tree(tree* t, int count_of_el, int el_size);

int test_scan_del(int count_of_tests, int start_pos, int step, int count_of_steps, int size_of_arr_for_test, int el_size);

int test_add(int count_of_tests, int start_pos, int step, int count_of_steps, int size_of_arr_for_test, int el_size);