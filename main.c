#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 256

// Intoarce registrul asociat caracterului
char *char_to_register(char x) {
	char *reg = malloc(4 * sizeof(char));

	switch (x) {
	case 'a':
		snprintf(reg, sizeof("eax"), "eax");
		break;
	case 'b':
		snprintf(reg, sizeof("ebx"), "ebx");
		break;
	case 'c':
		snprintf(reg, sizeof("ecx"), "ecx");
		break;
	case 'd':
		snprintf(reg, sizeof("edx"), "edx");
		break;
	}

	return reg;
}

int este_cifra(char x) {
	return (x >= '0' && x <= '9');
}

int este_operatie_logica(char *s) {
	return (strchr(s, '&') || strchr(s, '|') || strchr(s, '^'));
}

int este_aritmetica_plus_minus(char *s) {
	return (strchr(s, '+') || strchr(s, '-'));
}

int este_aritmetica_mul_div(char *s) {
	return (strchr(s, '*') || strchr(s, '/'));
}

int este_shiftare(char *s) {
	return ((s[6] == '<' && s[7] == '<') || (s[6] == '>' && s[7] == '>'));
}

// Intoarce: 1 - if; 2 - for; 3 - while;
int este_if_for_while(char *s) {
	if (strstr(s, "if"))
		return 1;
	if (strstr(s, "for"))
		return 2;
	if (strstr(s, "while"))
		return 3;
	return 0;
}

// Sterge tab-urile de la inceputul instructiunii (cod ASCII 9)
void sterge_tab_inceput(char *s) {
	int i = 0;
	while ((int)s[i] == 9)
		i++;
	if (i > 0) {
		char aux[256];
		strcpy(aux, s + i);
		strcpy(s, aux);
	}
}

// Retine structura curenta: 0 - niciuna; 1 - if; 2 - for; 3 - while;
int tip_structura;

/**
 * Starting point of the program, write anything here
 */
int main(void) {
	char s[256];

	// Operandul principal al operatiei curente
	char a = 0;
	// Operandul principal al structurii curente
	char op_str = 0;

	// Citim instructiunile
	while (fgets(s, MAX_LENGTH, stdin)) {
		sterge_tab_inceput(s);

		// Verificam daca operatia este logica sau aritmetica (doar + sau -)
		if ((este_operatie_logica(s) || este_aritmetica_plus_minus(s)) &&
			!este_if_for_while(s)) {
			// Extragem primul operand
			a = s[0];

			char b = s[4];
			char op = s[6];

			char op_nume[4];
			switch (op) {
			case '&':
				strcpy(op_nume, "AND");
				break;
			case '|':
				strcpy(op_nume, "OR");
				break;
			case '^':
				strcpy(op_nume, "XOR");
				break;
			case '+':
				strcpy(op_nume, "ADD");
				break;
			case '-':
				strcpy(op_nume, "SUB");
				break;
			}

			// Verificam daca al treilea operand este numar sau variabila
			if (este_cifra(s[8])) {
				int c = atoi(s + 8);

				char *reg_a = char_to_register(a);

				printf("%s %s, %d\n", op_nume, reg_a, c);

				free(reg_a);
			} else {
				char c = s[8];

				char *reg_b = char_to_register(b);
				char *reg_c = char_to_register(c);

				printf("%s %s, %s\n", op_nume, reg_b, reg_c);

				free(reg_b);
				free(reg_c);
			}
		} else if (s[0] == '}') {
			if (tip_structura == 2) {
				char *reg_a = char_to_register(op_str);
				printf("ADD %s, 1\n", reg_a);
				free(reg_a);
			}
			if (tip_structura == 2 || tip_structura == 3)
				printf("JMP start_loop\n");
			if (tip_structura == 2)
				printf("end_loop:\n");
			else
				printf("end_label:\n");
			tip_structura = 0;
		} else if (este_aritmetica_mul_div(s)) {
			// Extragem primul operand
			a = s[0];

			char op = s[6];

			// Tratam cazul cu impartirea la 0
			if (op == '/' && s[8] == '0') {
				printf("Error\n");
				// Trecem mai departe
				continue;
			}

			char op_nume[4];
			switch (op) {
			case '*':
				strcpy(op_nume, "MUL");
				break;
			case '/':
				strcpy(op_nume, "DIV");
				break;
			}

			// Verificam daca primul operand este diferit de 'a'
			char *source2 = char_to_register(a);
			if (strcmp(source2, "eax"))
				printf("MOV eax, %s\n", source2);

			// Verificam daca al doilea operand este numar sau variabila
			if (este_cifra(s[8])) {
				int b = atoi(s + 8);
				printf("%s %d\n", op_nume, b);
			} else {
				char b = s[8];
				char *reg_b = char_to_register(b);
				printf("%s %s\n", op_nume, reg_b);
				free(reg_b);
			}

			// Verificam daca trebuie mutat registrul la loc
			if (strcmp(source2, "eax"))
				printf("MOV %s, eax\n", source2);

			free(source2);
		} else if (este_shiftare(s)) {
			// Extragem primul operand
			a = s[0];

			char op = s[6];
			int nr = atoi(s + 9);
			char *reg_a = char_to_register(a);

			if (op == '>')
				printf("SHR %s, %d\n", reg_a, nr);
			else
				printf("SHL %s, %d\n", reg_a, nr);

			free(reg_a);
		} else if (este_if_for_while(s)) {
			tip_structura = este_if_for_while(s);

			if (tip_structura == 1) {
				a = s[4];
				op_str = a;
				char *reg_a = char_to_register(a);

				int cmp1 = s[6], cmp2 = s[7];

				/*
				 * Pozitia celui de-al doilea operator depinde daca
				 * comparatia are 2 simboluri (==, <=, >=) sau unul
				 * singur (<, >)
				 *
				*/
				int poz_op2 = 9;
				if (cmp2 == ' ')
					poz_op2 = 8;

				if (este_cifra(s[poz_op2])) {
					int b = atoi(s + poz_op2);
					printf("CMP %s, %d\n", reg_a, b);
				} else {
					char b = s[poz_op2];
					char *reg_b = char_to_register(b);
					printf("CMP %s, %s\n", reg_a, reg_b);
					free(reg_b);
				}
				free(reg_a);

				if (cmp1 == '>' && cmp2 == '=')
					printf("JL end_label\n");
				else if (cmp1 == '<' && cmp2 == '=')
					printf("JG end_label\n");
				else if (cmp1 == '=' && cmp2 == '=')
					printf("JNE end_label\n");
				else if (cmp1 == '<')
					printf("JGE end_label\n");
				else if (cmp1 == '>')
					printf("JLE end_label\n");
			} else if (tip_structura == 2) {
				a = s[5];
				op_str = a;
				int b = atoi(s + 9);
				char *reg_a = char_to_register(a);
				printf("MOV %s, %d\n", reg_a, b);
				printf("start_loop:\n");
				int c = atoi(s + 16);
				printf("CMP %s, %d\n", reg_a, c);
				free(reg_a);

				if (strstr(s, "=="))
					printf("JNE end_label\n");
				else if (strstr(s, "<="))
					printf("JG end_label\n");
				else if (strstr(s, ">="))
					printf("JL end_label\n");
				else if (strstr(s, "<"))
					printf("JGE end_label\n");
				else if (strstr(s, ">"))
					printf("JLE end_label\n");
			} else if (tip_structura == 3) {
				printf("start_loop:\n");

				a = s[7];
				op_str = a;
				char *reg_a = char_to_register(a);

				int cmp1 = s[9], cmp2 = s[10];

				/*
				 * Pozitia celui de-al doilea operator depinde daca
				 * comparatia are 2 simboluri (==, <=, >=) sau unul
				 * singur (<, >)
				 *
				*/
				int poz_op2 = 12;
				if (cmp2 == ' ')
					poz_op2 = 11;

				if (este_cifra(s[poz_op2])) {
					int b = atoi(s + poz_op2);
					printf("CMP %s, %d\n", reg_a, b);
				} else {
					char b = s[poz_op2];
					char *reg_b = char_to_register(b);
					printf("CMP %s, %s\n", reg_a, reg_b);
					free(reg_b);
				}
				free(reg_a);

				if (cmp1 == '>' && cmp2 == '=')
					printf("JL end_label\n");
				else if (cmp1 == '<' && cmp2 == '=')
					printf("JG end_label\n");
				else if (cmp1 == '=' && cmp2 == '=')
					printf("JNE end_label\n");
				else if (cmp1 == '<')
					printf("JGE end_label\n");
				else if (cmp1 == '>')
					printf("JLE end_label\n");
			}
		} else {
			// Extragem primul operand
			a = s[0];
			// Verificam daca al doilea operand este numar sau variabila
			if (este_cifra(s[4])) {
				int b = atoi(s + 4);
				char *reg_a = char_to_register(a);
				printf("MOV %s, %d\n", reg_a, b);
				free(reg_a);
			} else {
				char b = s[4];
				char *reg_a = char_to_register(a);
				char *reg_b = char_to_register(b);
				printf("MOV %s, %s\n", reg_a, reg_b);
				free(reg_a);
				free(reg_b);
			}
		}
	}

	return 0;
}
