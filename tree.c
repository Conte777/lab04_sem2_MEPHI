#include "tree.h"

void create_tree(tree** t) {
	*t = (tree*)calloc(1, sizeof(tree));
	(*t)->key = NULL;
	(*t)->info = NULL;
	(*t)->parent = NULL;
	(*t)->left = NULL;
	(*t)->right = NULL;
}

int add_e(tree* t, string* key, string* info, string** data) {
	if (key != NULL && info != NULL && t != NULL) {
		tree* par = t, * ptr;
		if (t->key == NULL) {
			t->key = key;
			t->info = info;
			return OK;
		}
		else {
			if (strcmp(key->string, par->key->string) == 0) {
				*data = t->info;
				t->info = info;
				return DB;
			}
			if (strcmp(key->string, t->key->string) < 0)
				ptr = t->left;
			else
				ptr = t->right;
			while (ptr != NULL && strcmp(par->key->string, key->string) != 0) {
				tree* p = ptr;
				if (strcmp(key->string, par->key->string) < 0)
					ptr = par->left;
				else
					ptr = par->right;
				par = p;
			}
			if (strcmp(key->string, par->key->string) == 0) {
				*data = ptr->info;
				par->info = info;
				return DB;
			}
			if (strcmp(key->string, par->key->string) < 0) {
				par->left = (tree*)calloc(1, sizeof(tree));
				par->left->info = info;
				par->left->key = key;
				par->left->parent = par;
			}
			else {
				par->right = (tree*)calloc(1, sizeof(tree));
				par->right->info = info;
				par->right->key = key;
				par->right->parent = par;
			}
			return OK;
		}
	}
	return UN;
}

int scan(tree* t, string* key, tree** info) {
	if (t == NULL || key == NULL)
		return UN;
	if (t->key == NULL && t->info == NULL)
		return UN;
	if (strcmp(t->key->string, key->string) == 0) {
		*info = t;
		return OK;
	}
	if (t->left != NULL)
		if (strcmp(t->key->string, key->string) > 0)
			if (scan(t->left, key, info) == OK)
				return OK;
	if (t->right != NULL)
		if (strcmp(t->key->string, key->string) < 0)
			if (scan(t->right, key, info) == OK)
				return OK;
	return UN;
}

int scan_max(tree* t, tree** info) {
	if (t == NULL)
		return UN;
	if (t->right != NULL)
		scan_max(t->right, info);
	else
		*info = t;
	return OK;
}

int del_e(tree** t, string* key) {
	if (t == NULL || key == NULL)
		return UN;
	tree* info = NULL;
	if (scan(*t, key, &info) == OK) {
		if (info->left == NULL && info->right == NULL) {
			free_s(&info->key);
			free_s(&info->info);
			if (info->parent != NULL) {
				if (info->parent->left == info)
					info->parent->left = NULL;
				if (info->parent->right == info)
					info->parent->right = NULL;
				free(info);
			}
			else {
				info->key = NULL;
				info->info = NULL;
			}
			return OK;
		}
		if (info->left == NULL && info->right != NULL) {
			free_s(&info->key);
			free_s(&info->info);
			if (info->parent != NULL) {
				info->right->parent = info->parent;
				if (info->parent->left == info)
					info->parent->left = info->right;
				if (info->parent->right == info)
					info->parent->right = info->right;
			}
			else {
				info->right->parent = NULL;
				*t = info->right;
			}
			free(info);
			return OK;
		}
		if (info->left != NULL && info->right == NULL) {
			free_s(&info->key);
			free_s(&info->info);
			if (info->parent != NULL) {
				info->left->parent = info->parent;
				if (info->parent->left == info)
					info->parent->left = info->left;
				if (info->parent->right == info)
					info->parent->right = info->left;
			}
			else {
				info->left->parent = NULL;
				*t = info->left;
			}
			free(info);
			return OK;
		}
		if (info->left != NULL && info->right != NULL) {
			free_s(&info->info);
			free_s(&info->key);
			tree* max = NULL, * left = NULL;
			scan_max(info->left, &max);
			left = max;
			while (left->left != NULL)
				left = left->left;
			if (max != left) {
				left->left = info->left;
				info->left->parent = left;
			}
			else {
				max->left = info->left;
				info->left->parent = max;
			}
			if (max->parent->left == max)
				max->parent->left = NULL;
			if (max->parent->right == max)
				max->parent->right = NULL;
			if (info->parent != NULL) {
				max->parent = info->parent;
				if (info->parent->left == info)
					info->parent->left = max;
				if (info->parent->right == info)
					info->parent->right = max;
			}
			else {
				*t = max;
				max->parent = NULL;
			}
			max->right = info->right;
			info->right->parent = max;
			free(info);
			return OK;
		}
	}
	return UN;
}

int print_tree(tree* t, string* key1, string* key2) {
	if (t == NULL || key1 == NULL || key2 == NULL)
		return UN;
	if (t->key == NULL && t->info == NULL)
		return UN;
	if (t->parent == NULL)
		printf("key info\n");
	if (strcmp(t->key->string, key1->string) > 0)
		print_tree(t->left, key1, key2);
	if (strcmp(t->key->string, key1->string) > 0 && strcmp(t->key->string, key2->string) < 0) {
		print_string(t->key);
		printf(" ");
		print_string(t->info);
		printf("\n");
	}
	if (strcmp(t->key->string, key2->string) < 0)
		print_tree(t->right, key1, key2);
	return OK;
}

int format_tree(tree* t, int p) {
	if (t == NULL)
		return UN;
	if (t->key == NULL && t->info == NULL)
		return UN;
	print_string(t->key);
	printf(" ");
	print_string(t->info);
	printf("\n");
	if (t->left != NULL || t->right != NULL) {
		for (int i = 0; i < p - 2; i++)
			printf("|  ");
		printf("|\n");
	}
	if (t->left != NULL) {
		for (int i = 0; i < p - 1; i++)
			printf("|  ");
		printf("L: ");
		format_tree(t->left, p + 1);
	}
	if (t->left != NULL && t->right != NULL) {
		for (int i = 0; i < p - 2; i++)
			printf("|  ");
		printf("|\n");
	}
	if (t->right != NULL) {
		for (int i = 0; i < p - 1; i++)
			printf("|  ");
		printf("R: ");
		format_tree(t->right, p + 1);
	}
	return OK;
}

int from_file(tree* t, string* file_name) {
	FILE* file = NULL;
	file = fopen(file_name->string, "r");
	string* key, * info, * data;
	if (file == NULL)
		return UN;
	while (get_f(&key, file) == OK && get_f(&info, file) == OK) {
		if (add_e(t, key, info, &data) == DB) {
			free_s(&key);
			free_s(&data);
		}
	}
	fclose(file);
	return OK;
}

void free_tree(tree* t) {
	if (t != NULL) {
		if (t->left != NULL)
			free_tree(t->left);
		if (t->right != NULL)
			free_tree(t->right);
		t->right = NULL;
		t->left = NULL;
		free_s(&t->key);
		free_s(&t->info);
		free(t);
	}
}

int viz_tree(tree* t, FILE* file) {
	if (t == NULL)
		return UN;
	if (t->key == NULL && t->info == NULL)
		return UN;
	if (t->parent == NULL)
		fprintf(file, "digraph viz {\n");
	if (t->parent == NULL && t->left == NULL && t->right == NULL) {
		fprintf(file, "\"Key: ");
		fwrite(t->key->string, sizeof(char), t->key->size + 1, file);
		fprintf(file, "\\nInfo: ");
		fwrite(t->info->string, sizeof(char), t->info->size + 1, file);
		fprintf(file, "\"");
	}
	if (t->left != NULL) {
		fprintf(file, "\"Key: ");
		fwrite(t->key->string, sizeof(char), t->key->size + 1, file);
		fprintf(file, "\\nInfo: ");
		fwrite(t->info->string, sizeof(char), t->info->size + 1, file);
		fprintf(file, "\"");
		fprintf(file, "->");
		fprintf(file, "\"Key: ");
		fwrite(t->left->key->string, sizeof(char), t->left->key->size + 1, file);
		fprintf(file, "\\nInfo: ");
		fwrite(t->left->info->string, sizeof(char), t->left->info->size + 1, file);
		fprintf(file, "\"");
		fprintf(file, ";\n");
	}
	if (t->right != NULL) {
		fprintf(file, "\"Key: ");
		fwrite(t->key->string, sizeof(char), t->key->size + 1, file);
		fprintf(file, "\\nInfo: ");
		fwrite(t->info->string, sizeof(char), t->info->size + 1, file);
		fprintf(file, "\"");
		fprintf(file, "->");
		fprintf(file, "\"Key: ");
		fwrite(t->right->key->string, sizeof(char), t->right->key->size + 1, file);
		fprintf(file, "\\nInfo: ");
		fwrite(t->right->info->string, sizeof(char), t->right->info->size + 1, file);
		fprintf(file, "\"");
		fprintf(file, ";\n");
	}
	if (t->left != NULL)
		viz_tree(t->left, file);
	if (t->right != NULL)
		viz_tree(t->right, file);
	if (t->parent == NULL) {
		fseek(file, 0, SEEK_END);
		fprintf(file, "\n}");
	}
	return OK;
}

int add_unique_random_str_to_tree(tree* t, int count_of_el, int el_size) {
	if (t == NULL || count_of_el < 0 || el_size < 0)
		return UN;
	int error;
	string* key = NULL, * info = NULL, * data = NULL;
	for (int i = 0; i <= count_of_el; i++) {
		error = create_random_string(&key, el_size);
		if (error)
			return error;
		error = create_random_string(&info, 0);
		if (error)
			return error;
		if (add_e(t, key, info, &data)) {
			free_s(&key);
			free_s(&data);
			i--;
		}
	}
	return OK;
}

int test_scan_del(int count_of_tests, int start_pos, int step, int count_of_steps, int size_of_arr_for_test, int el_size) {
	printf("Test start\n");
	tree* t;
	tree* buffer;
	srand(time(NULL));
	double* scan_time = (double*)calloc(count_of_steps + 1, sizeof(double));
	double* del_time = (double*)calloc(count_of_steps + 1, sizeof(double));
	int error;
	for (int i = 0; i < count_of_tests; i++) {
		printf("\nTest #%d\n", i + 1);
		string** arr_s;
		double start, end;
		error = create_random_arr_of_string(&arr_s, size_of_arr_for_test, el_size);
		if (error)
			return error;
		create_tree(&t);
		for (int j = 0; j <= count_of_steps; j++) {
			//printf("\nElemets in tree: %d\n", start_pos + step * j);
			int count_of_del = 0;
			if (j == 0)
				error = add_unique_random_str_to_tree(t, start_pos, el_size);
			else
				error = add_unique_random_str_to_tree(t, step, el_size);
			if (error)
				return error;
			start = clock();
			for (int k = 0; k < size_of_arr_for_test; k++)
				scan(t, arr_s[k], &buffer);
			end = clock();
			//printf("%f ", ((float)(end - start)) / CLOCKS_PER_SEC);
			scan_time[j] += (end - start) / (double)CLOCKS_PER_SEC;
			start = clock();
			for (int k = 0; k < size_of_arr_for_test; k++)
				if (del_e(&t, arr_s[k]) == OK)
					count_of_del++;
			end = clock();
			//printf("%f\n", ((float)(end - start)) / CLOCKS_PER_SEC);
			del_time[j] += (end - start) / (double)CLOCKS_PER_SEC;
			error = add_unique_random_str_to_tree(t, count_of_del, el_size);
			if (error)
				return error;
		}
		free_tree(t);
		for (int j = 0; j < size_of_arr_for_test; j++)
			free_s(&arr_s[j]);
		free(arr_s);
	}
	printf("\nAvg time of tests\n");
	for (int i = 0; i <= count_of_steps; i++) {
		//scan_time[i] /= count_of_tests;
		//del_time[i] /= count_of_tests;
		printf("%f %f\n", scan_time[i] / (double)count_of_tests, del_time[i] / (double)count_of_tests);
	}
	free(scan_time);
	free(del_time);
	return OK;
}

int test_add(int count_of_tests, int start_pos, int step, int count_of_steps, int size_of_arr_for_test, int el_size) {
	printf("Test start\n");
	tree* t;
	srand(time(NULL));
	double* add_time = (double*)calloc(count_of_steps + 1, sizeof(double));
	int error;
	double start, end;
	for (int i = 0; i < count_of_tests; i++) {
		printf("\nTest #%d\n", i + 1);
		string** arr_key, ** arr_info, * buffer = NULL;
		error = create_random_arr_of_string(&arr_key, size_of_arr_for_test, el_size);
		if (error)
			return error;
		error = create_random_arr_of_string(&arr_info, size_of_arr_for_test, 0);
		if (error)
			return error;
		create_tree(&t);
		for (int j = 0; j <= count_of_steps; j++) {
			if (j == 0)
				error = add_unique_random_str_to_tree(t, start_pos, el_size);
			else
				error = add_unique_random_str_to_tree(t, step - size_of_arr_for_test, el_size);
			if (error)
				return error;
			start = clock();
			for (int k = 0; k < size_of_arr_for_test; k++)
				if (add_e(t, arr_key[k], arr_info[k], &buffer) == DB) {
					free_s(&buffer);
					free_s(&arr_key[k]);
				}
			end = clock();
			//printf("%f\n", (end - start) / (double)CLOCKS_PER_SEC);
			add_time[j] += (end - start) / (double)CLOCKS_PER_SEC;
		}
		free_tree(t);
		free(arr_key);
		free(arr_info);
	}
	for (int i = 0; i <= count_of_steps; i++)
		printf("%f\n", add_time[i] / (double)count_of_tests);
	free(add_time);
}