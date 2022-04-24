#include "D:\GAMES\Учеба\test\tree.h"

int pow10(int a) {
	if (a == 0)
		return 1;
	int count = 1;
	for (int i = 0; i < a; i++)
		count *= 10;
	return count;
}

int char_to_int(string* s, int* data) {
	int count = 0;
	for (int i = s->size; i > -1; i--) {
		switch (s->string[i]) {
		case '1':
			count += 1 * pow10(s->size - i);
			break;
		case '2':
			count += 2 * pow10(s->size - i);
			break;
		case '3':
			count += 3 * pow10(s->size - i);
			break;
		case '4':
			count += 4 * pow10(s->size - i);
			break;
		case '5':
			count += 5 * pow10(s->size - i);
			break;
		case '6':
			count += 6 * pow10(s->size - i);
			break;
		case '7':
			count += 7 * pow10(s->size - i);
			break;
		case '8':
			count += 8 * pow10(s->size - i);
			break;
		case '9':
			count += 9 * pow10(s->size - i);
			break;
		case '0':
			break;
		default:
			return UN;
		}
		count++;
		*data = count;
		return OK;
	}
}

int int_to_char(string** data, int a) {
	int size = 0;
	int b = a;
	while (b != 0) {
		size++;
		b /= 10;
	}
	if (size == 0)
		return UN;
	*data = (string*)calloc(1, sizeof(string));
	(*data)->size = size - 1;
	if (*data == NULL)
		return OF;
	(*data)->string = (char*)calloc(size + 1, sizeof(char));
	if ((*data)->string == NULL)
		return OF;
	for (int i = 0; i < size; i++) {
		switch (a % 10) {
		case 1:
			(*data)->string[i] = '1';
			break;
		case 2:
			(*data)->string[i] = '2';
			break;
		case 3:
			(*data)->string[i] = '3';
			break;
		case 4:
			(*data)->string[i] = '4';
			break;
		case 5:
			(*data)->string[i] = '5';
			break;
		case 6:
			(*data)->string[i] = '6';
			break;
		case 7:
			(*data)->string[i] = '7';
			break;
		case 8:
			(*data)->string[i] = '8';
			break;
		case 9:
			(*data)->string[i] = '9';
			break;
		case 0:
			(*data)->string[i] = '0';
			break;
		default:
			return UN;
		}
		a /= 10;
	}
	(*data)->string[size] = '\0';
	return OK;
}

int print_count(tree* t) {
	if (t == NULL)
		return UN;
	if (t->key == NULL && t->info == NULL)
		return UN;
	if (t->parent == NULL)
		printf("word count\n");
	if (t->left != NULL)
		print_count(t->left);
	print_string(t->key);
	printf(" ");
	print_string(t->info);
	printf("\n");
	if (t->right != NULL)
		print_count(t->right);
	return OK;
}

int write_text() {
	tree* t, * data;
	string* key, * count = NULL, * buf;
	int error, buffer;
	create_tree(&t);
	error = read_word_without_upper_lower_case_and_punctuation_marks(&key);
	while (!error || error == -1) {
		if (scan(t, key, &data) == OK) {
			if (char_to_int(data->info, &buffer) == OK) {
				if (int_to_char(&count, buffer) == OK) {
					add_e(t, key, count, &buf);
					free_s(&key);
					free_s(&buf);
				}
			}
		}
		else {
			count = (string*)calloc(1, sizeof(string));
			count->size = 0;
			count->string = (char*)calloc(2, sizeof(char));
			count->string[0] = '1';
			count->string[1] = '\0';
			add_e(t, key, count, &buf);
		}
		if (error == -1)
			break;
		error = read_word_without_upper_lower_case_and_punctuation_marks(&key);
	}
	free_s(&key);
	print_count(t);
	free_tree(t);
}

int main() {
	printf("Start entering text: ");
	write_text();
	return OK;
}