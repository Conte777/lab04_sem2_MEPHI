#include "tree.h"
// 1 59 59 1 53 53 1 3 3 1 43 43 1 51 51 1 86 86 1 62 62 1 24 24 1 93 93 1 33 33
int scanfs(int* a, int b);
int errors_get(int a, string* s);
int choice(tree** t, int a);

int errors_get(int a, string* s) {
	switch (a) {
	case CZ:
		return CZ;
	case OF:
		printf("The line is too long! Try again: ");
		errors_get(get_s(&s), s);
	}
	return OK;
}

int choice(tree** t, int a) {
	string* key = NULL, * info = NULL, * key2 = NULL;
	tree* info1 = NULL;
	FILE* file = NULL;
	int error;
	clock_t start, end;
	switch (a) {
	case 1:
		printf("Enter key: ");
		if (errors_get(get_s(&key), key))
			return CZ;
		printf("Enter info: ");
		if (errors_get(get_s(&info), info))
			return CZ;
		error = add_e(*t, key, info, &key2);
		if (error == OK)
			printf("The item was successfully added\n");
		if (error == UN) {
			printf("Something is wrong with the tree or with the key\n");
			free_s(&key);
			free_s(&info);
		}
		if (error == DB) {
			printf("Duplicate was found\nHere's what was replaced: ");
			print_string(key2);
			printf("\n");
			free_s(&key);
			free_s(&key2);
		}
		return OK;
	case 2:
		printf("Enter the key that we will use to search for: ");
		if (errors_get(get_s(&key), key))
			return CZ;
		if (scan(*t, key, &info1) == OK) {
			printf("key:  ");
			print_string(info1->key);
			printf("\ninfo: ");
			print_string(info1->info);
			printf("\n");
		}
		else {
			printf("Nothing was found on your key.\n");
		}
		free_s(&key);
		return OK;
	case 3:
		printf("Enter key: ");
		if (errors_get(get_s(&key), key) == CZ)
			return CZ;
		if (del_e(t, key) == UN) {
			printf("Nothing was found on your key.\n");
		}
		else {
			printf("The item was successfully deleted\n");
		}
		free_s(&key);
		return OK;
	case 4:
		start = clock();
		if (scan_max(*t, &info1) == OK) {
			end = clock();
			printf("key info\n");
			print_string(info1->key);
			printf(" ");
			print_string(info1->info);
			printf("\n");
		}
		else {
			end = clock();
			printf("Tree is empty\n");
		}
		printf("work time: %f\n", ((float)(end - start)) / CLOCKS_PER_SEC);
		return OK;
	case 5:
		printf("Enter the path to the file: ");
		if (errors_get(get_s(&key), key) == CZ)
			return CZ;
		start = clock();
		if (from_file(*t, key) == UN) {
			end = clock();
			printf("Could not open the file\n");
		}
		else {
			end = clock();
			printf("The data from the file is copied\n");
		}
		free_s(&key);
		printf("work time: %f\n", ((float)(end - start)) / CLOCKS_PER_SEC);
		return OK;
	case 6:
		start = clock();
		if (format_tree(*t, 1))
			printf("Tree is empty.\n");
		end = clock();
		printf("work time: %f\n", ((float)(end - start)) / CLOCKS_PER_SEC);
		return OK;
	case 7:
		printf("Enter key1: ");
		get_s(&key);
		printf("Enter key2: ");
		get_s(&key2);
		start = clock();
		if (print_tree(*t, key, key2))
			printf("Tree is empty.\n");
		end = clock();
		printf("\n");
		free_s(&key);
		free_s(&key2);
		printf("work time: %f\n", ((float)(end - start)) / CLOCKS_PER_SEC);
		return OK;
	case 8:
		file = fopen("viz.gv", "w+");
		if (file == NULL)
			return UN;
		if (viz_tree(*t, file) == OK) {
			fclose(file);
			system("dot -Tpng viz.gv -o viz.png");
			system("start viz.png");
		}
		else {
			printf("Tree is empty.\n");
			fclose(file);
		}
		remove("viz.gv");
		return OK;
		/*case 9:
			write_text();
			return OK;*/
	case 10:
		test_scan_del(10, 1000, 1000, 1099, 1000, 10);
		return OK;
	case 11:
		test_add(10, 1000, 1000, 1099, 500, 10);
		return OK;
	}
	return CZ;
}

int scanfs(int* a, int b) {
	int c;
	do {
		c = scanf("%d", a);
		if (c < 0)
			return 1;
		if ((b == 1 && *a <= 0) || c == 0) {
			printf("Error, try again: ");
			scanf("%*c");
		}
	} while ((b == 1 && *a <= 0) || c == 0);
	return 0;
}

/*int main() {
	int flag = 1, a = 0, b = 0;
	tree* t;
	create_tree(&t);
	while (flag) {
		printf("\nEnter the number of the item to be executed\n0. Exit\n1. Add new element\n2. Scan\n3. Delete\n4. Scaning max\n5. Add items from a file\n6. Print format tree\n7. Print tree\n8. Vizual tree\nEnter number: ");
		b = scanf("%d", &a);
		// system("cls");
		if (b > 0 && a > 0 && a <= 12) {
			if (choice(&t, a) == CZ)
				flag = 0;
		}
		else {
			if (b == 0) {
				printf("You didn't enter a number, try again: ");
				scanf("%*c");
			}
			else if (a == 0 || b < 0)
				flag = 0;
		}
	}
	free_tree(t);
	return 0;
}*/