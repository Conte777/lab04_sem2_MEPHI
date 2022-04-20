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
	if (t->key == NULL)
		return UN;
	if (strcmp(t->key->string, key->string) == 0) {
		*info = t;
		return OK;
	}
	if (strcmp(t->key->string, key->string) > 0)
		if (scan(t->left, key, info) == OK)
			return OK;
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
	if (t->key == NULL && t->key == NULL)
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

int add_random(tree* t, int size, int flag) {
	int k = 0;
	for (int i = 0; i < size; i++) {
		k++;
		string* element1 = create_s_random(rand() % 1000 + 1, i + k);
		string* element2 = create_s_random(rand() % 1000 + 1, i + k + 100);
		string* element3 = NULL;
		if (element1 == NULL || element2 == NULL)
			return UN;
		if (add_e(t, element1, element2, &element3) == DB) {
			free_s(&element1);
			free_s(&element3);
			if (flag)
				i--;
		}
	}
	printf("iteration: %d\n", k);
	return OK;
}

string* create_s_random(int size, int ran) {
	string* element1 = (string*)calloc(1, sizeof(string));
	element1->size = size;
	element1->string = (char*)calloc(element1->size + 3, sizeof(char));
	if (element1->string == NULL)
		return NULL;
	for (int j = 0; j <= element1->size; j++) {
		srand(time(NULL) - j + ran);
		element1->string[j] = '!' + rand() % (int)'~';
	}
	element1->string[element1->size + 2] = '\0';
	return element1;
}

string** create_arr_s_random(int size) {
	string** arr = (string**)calloc(size, sizeof(string*));
	for (int i = 0; i < size; i++) {
		arr[i] = create_s_random(rand() % 1000 + 1, i);
	}
	return arr;
}

void test(tree** t, int size, float* tscan, float* tdel) {
	clock_t start, end;
	tree* info;
	int quantity = 10000, flag = 0;
	start = clock();
	add_random(*t, size, 1);
	end = clock();
	printf("work time add: %f sec\n", ((float)(end - start)) / CLOCKS_PER_SEC);
	string** arr = create_arr_s_random(quantity);
	start = clock();
	for (int i = 0; i < quantity; i++)
		scan(*t, arr[i], &info);
	end = clock();
	printf("work time scan: %f sec\n", ((float)(end - start)) / CLOCKS_PER_SEC);
	*tscan += ((float)(end - start)) / CLOCKS_PER_SEC;
	start = clock();
	for (int i = 0; i < quantity; i++)
		if (del_e(t, arr[i]) == OK)
			flag++;
	end = clock();
	add_random(*t, flag, 1);
	printf("work time del: %f sec\n", ((float)(end - start)) / CLOCKS_PER_SEC);
	*tdel += ((float)(end - start)) / CLOCKS_PER_SEC;
	for (int i = 0; i < quantity; i++) {
		free_s(&arr[i]);
	}
	free(arr);
}

void stack_test() {
	printf("Start test\n");
	tree* t;
	int size = 10, size2 = 5;
	float* tscan = (float*)calloc(size, sizeof(float));
	float* tdel = (float*)calloc(size, sizeof(float));
	for (int j = 0; j < size2; j++) {
		create_tree(&t);
		printf("\n\ntest #%d\n", j + 1);
		for (int i = 1; i <= size; i++) {
			printf("\nsizeof arr: %d\n", i * 50000);
			test(&t, 50000, &tscan[i - 1], &tdel[i - 1]);
		}
		free_tree(t);
	}
	for (int i = 0; i < size; i++) {
		tscan[i] /= (float)size2;
		tdel[i] /= (float)size2;
		printf("\navg time of %d size\nscan: %f sec\ndel:  %f sec\n", (i + 1) * 50000, tscan[i], tdel[i]);
	}
	free(tscan);
	free(tdel);
}

void test_add() {
	tree* t;
	string* s = NULL;
	int test = 5, size = 10, start, end;
	float* tadd = (float*)calloc(size, sizeof(float));
	for (int i = 0; i < test; i++) {
		printf("\n\ntest #%d\n", i + 1);
		for (int j = 0; j < size; j++) {
			create_tree(&t);
			printf("\niteration: %d\n", (j + 1) * 50000);
			string** key = create_arr_s_random((j + 1) * 50000);
			string** info = create_arr_s_random((j + 1) * 50000);
			start = clock();
			for (int k = 0; k < (j + 1) * 50000; k++)
				if (add_e(t, key[k], info[k], &s) == DB) {
					free_s(&key[k]);
					free_s(&s);
				}
			end = clock();
			tadd[j] += ((float)(end - start)) / CLOCKS_PER_SEC;
			printf("time add: %f sec\n", ((float)(end - start)) / CLOCKS_PER_SEC);
			free_tree(t);
			free(key);
			free(info);
		}
	}
	for (int i = 0; i < size; i++) {
		tadd[i] /= test;
		printf("avg time add of size %d: %f sec\n", (i + 1) * 50000, tadd[i]);
	}
	free(tadd);
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
		fprintf(file, "digraph viz {");
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
		fprintf(file, ";");
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
		fprintf(file, ";");
	}
	if (t->left != NULL)
		viz_tree(t->left, file);
	if (t->right != NULL)
		viz_tree(t->right, file);
	if (t->parent == NULL) {
		fseek(file, 0, SEEK_END);
		fprintf(file, "}");
	}
	return OK;
}
