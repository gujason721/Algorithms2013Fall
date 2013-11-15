#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define max(x, y) (x)>(y)? (x) : (y)
#define MAX 100

//#define DEBUG_MATRIX

//char A[m + 1] = "ABCBDAB";
//char B[n + 1] = "BDCABA";
char A[MAX + 1], B[MAX + 1];
int P[MAX + 1][MAX + 1];




int Calc(int m, int n)
{
	int i, j;
	for (i = 0; i <= m; i++) {
		for (j = 0; j <= n; j++) {
			if ((i == 0) || (j == 0)) {
				P[i][j] = 0;
				continue;
			}
			if (*(A + i - 1) == *(B + j - 1)) {
				P[i][j] = P[i - 1][j - 1] + 1;
			} else {
				P[i][j] = max(P[i - 1][j], P[i][j - 1]);
			}
		}
	}
	return 0;
}


char stack[50];
int idx = 0;
void stack_push(char c)
{
	stack[idx++] = c;
}
char stack_pop()
{
	return stack[idx--];
}
int stack_empty()
{
	if (idx == 0) {
		return 1;
	}
	return 0;
}
void stack_output()
{
	int i = idx;
	while (i >= 0) {
		printf ("%c", stack[i--]);
	}
}

int FindLCS(int m, int n, int bound)
{
	int i, j;
	if (m * n * bound == 0) {
//	if (m * n == 0) {
		printf ("\t<");
		stack_output();
		printf (">\n");
		return 0;
	}
	for (i = m; i > 0; i--) {
		for (j = n; j > 0; j--) {
			if (P[i][j] != bound) {
				break;
			}
			if ((P[i][j] == bound) 
					&& (P[i - 1][j - 1] == bound - 1)
					&& (P[i][j -  1] == bound - 1)
					&& (P[i - 1][j] == bound - 1)) {
//				printf ("[%d, %d]", i, j);
//				printf ("%c", *(A + i - 1));
				stack_push(*(A + i - 1));
				FindLCS (i - 1, j - 1, bound - 1);
				stack_pop();
			}
		}
	}
	return 0;
}



int main()
{
	printf ("Please input two string:\n");
	scanf("%s", A);
	scanf("%s", B);
	int m = strlen(A);
	int n = strlen(B);

	int i, j;              
	Calc (m, n);
#ifdef DEBUG_MATRIX
	printf (" |\t0\t");
	for (i = 0; i < n; i++) {
		printf ("%c\t", *(B + i));
	}
	printf ("\n_|_______________________________________________________\n");
	for (i = 0; i <= m; i++) {
		if (i == 0) {
			printf ("0|\t");
		} else {
			printf ("%c|\t", *(A + i - 1));
		}
		for (j = 0; j <= n; j++) {
			printf ("%d\t", P[i][j]);
		}
		printf ("\n");
	}
#endif

	printf ("LCS is %d:\n", P[m][n]);
	FindLCS (m, n, P[m][n]);
	printf ("\n");
//	printf ("Step is %d.\n", step);
	return 0;
}

