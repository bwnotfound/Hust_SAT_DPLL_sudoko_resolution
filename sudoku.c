#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SAT_Resolver.h"
#include "BasicComponent.h"

char* csvParseLine(char* s) {
	//if (s[0] == '\n')s++;
	return strtok(s, ",");
}

char* csvReadLine(FILE* fp) {
	char s[120], ch;
	ch = fgetc(fp);
	for (int i = 0; i < 118; i++) {
		s[i] = ch;
		ch = fgetc(fp);
		if (ch == '\n') {
			//s[i + 1] = '\n';
			s[i + 1] = '\0';
			break;
		}
	}
	return csvParseLine(s);
}

//int pow(int x, int y) {
//	if (y = 0)return 1;
//	return pow(x, y - 1) * x;
//}
char* intToString(int x) {
	/*if (x < 10) {
		char* s = (char*)malloc(sizeof(char) * 2);
		s[0] = x + '0'; s[1] = '\0';
		return s;
	}
	char* s = (char*)malloc(sizeof(char) * 10);
	int tem = x, d = 0;
	char* tems;
	for (int i = 0; i < 10; i++) {
		while (x > 10)x /= 10, d++;
		s[0] = x + '0'; s[1] = '\0';
		tems = intToString(tem - x * pow(10, d));
		
	}*/
	char tems[2], tem;
	char* s = (char*)malloc(sizeof(char) * 10);
	s[0] = '\0';
	while (x != 0) {
		tems[0] = x % 10 + '0'; tems[1] = '\0';
		strcat(s, tems);
		x /= 10;
	}
	int i = 0;
	for (;; i++)if (s[i] == '\0')break;
	i--;
	for (int j = 0; j < i - j; j++) {
		tem = s[j];
		s[j] = s[i - j];
		s[i - j] = tem;
	}
	return s;
}

int board[10][10];

int zipAll(int i, int j, int n, int isRev) {	//ijn -> (i-1)*81+(j-1)*9+n
	if (isRev == FALSE)
		return (i - 1) * 81 + (j - 1) * 9 + n;
	else
		return ((i - 1) * 81 + (j - 1) * 9 + n) * -1;
}
int zipInt(int x) {
	if(x > 0)
		return (x / 100 - 1) * 81 + (x / 10 % 10 - 1) * 9 + x % 10;
	else {
		x *= -1;
		return ((x / 100 - 1) * 81 + (x / 10 % 10 - 1) * 9 + x % 10) * -1;
	}
}

void getEmptySudokuCnf(FILE* fp) {
	fprintf(fp, "p cnf %d %d\n", 729, 11988);

	//格 列 行 宫 存在

	for (int x = 1; x <= 9; x++) 
		for (int y = 1; y <= 9; y++) {
			for (int n = 1; n <= 9; n++) 
				fprintf(fp, "%d ", zipAll(x, y, n, FALSE));
			fprintf(fp, "0\n");
		}

	for (int x = 1; x <= 9; x++)
		for (int n = 1; n <= 9; n++) {
			for (int y = 1; y <= 9; y++)
				fprintf(fp, "%d ", zipAll(x, y, n, FALSE));
			fprintf(fp, "0\n");
		}
	for (int y = 1; y <= 9; y++)
		for (int n = 1; n <= 9; n++) {
			for (int x = 1; x <= 9; x++)
				fprintf(fp, "%d ", zipAll(x, y, n, FALSE));
			fprintf(fp, "0\n");
		}
		
	for (int x = 0; x < 9; x += 3)
		for (int y = 0; y < 9; y += 3)
			for (int n = 1; n <= 9; n++) {
				for (int i = 1; i <= 9; i++)
					fprintf(fp, "%d ", zipAll(x + (i - 1) / 3 + 1, y + (i - 1) % 3 + 1, n, FALSE));
				fprintf(fp, "0\n");
			}

	//格 列 行 宫 不冲突

	for (int x = 1; x <= 9; x++)
		for (int y = 1; y <= 9; y++)
			for (int n = 1; n <= 8; n++)
				for (int m = n + 1; m <= 9; m++)
					fprintf(fp, "%d %d 0\n", zipAll(x, y, n, TRUE), zipAll(x, y, m, TRUE));

	for (int x = 1; x <= 9; x++) 
		for (int y = 1; y <= 8; y++) 
			for (int i = y + 1; i <= 9; i++) 
				for(int n = 1; n <= 9; n++)
					fprintf(fp, "%d %d 0\n", zipAll(x, y, n, TRUE), zipAll(x, i, n, TRUE));
		
	for (int y = 1; y <= 9; y++)
		for (int x = 1; x <= 8; x++)
			for (int i = x + 1; i <= 9; i++)
				for (int n = 1; n <= 9; n++)
					fprintf(fp, "%d %d 0\n", zipAll(x, y, n, TRUE), zipAll(i, y, n, TRUE));

	for (int x = 0; x < 9; x += 3)
		for (int y = 0; y < 9; y += 3) 
			for (int i = 1; i <= 8; i++)
				for (int j = i + 1; j <= 9; j++)
					for (int n = 1; n <= 9; n++)
						fprintf(fp, "%d %d 0\n", zipAll(x + (i - 1) / 3 + 1, y + (i - 1) % 3 + 1, n, TRUE), zipAll(x + (j - 1) / 3 + 1, y + (j - 1) % 3 + 1, n, TRUE));
}

void turnDataIntoCnf(Data D, FILE* fp) {
	fprintf(fp, "p cnf %d %d\n", D->varNum, D->clauseNum);
	ClauseNode pC = D->firstC;
	LiteralNode pL;
	while (pC != NULL) {
		pL = pC->firstL;
		while (pL != NULL) {
			fprintf(fp, "%d ", pL->varIdx * (pL->isRev? -1 : 1));
			pL = pL->nextL;
		}
		fprintf(fp, "0\n");
		pC = pC->nextC;
	}
}

void initCnf(FILE* fpCsv, char* path, char* pathTem) {
	while (fgetc(fpCsv) != '\n');
	
	char* s = (char*)malloc(sizeof(char) * 50);
	char* tems;
	char* b;
	for (int i = 1; i <= 1000; i++) {
		printf("%d\n", i);
		strcpy(s, path);
		tems = intToString(i);
		strcat(s, tems);
		strcat(s, ".cnf");
		FILE* fp = fopen(s, "w");

		b = csvReadLine(fpCsv);

		int x, y;
		for (int j = 0; j < 81; j++) {
			x = j / 9 + 1; y = j - j / 9 * 9 + 1;
			if (b[j] == '.')board[x][y] = 0;
			else board[x][y] = b[j] - '0';
		}


		FILE* fpTem = fopen(pathTem, "w");
		getEmptySudokuCnf(fpTem);
		fclose(fpTem);
		fpTem = fopen(pathTem, "r");
		Data D = initByCnf(fpTem);
		for (int x = 1; x <= 9; x++) {
			for (int y = 1; y <= 9; y++) {
				if (board[x][y] != 0) {
					ClauseNode pC = getClause();
					pC->firstL = getLiteral();
					pC->firstL->nextL = pC->firstL->preL = NULL;
					pC->firstL->varIdx = zipAll(x, y, board[x][y], FALSE);
					pC->firstL->isRev = FALSE;
					pC->nodeNum = 1; pC->preC = pC->nextC = NULL;
					addClause(pC, D);
				}
			}
		}
		fclose(fpTem);

		turnDataIntoCnf(D, fp);

		free(tems);
		fclose(fp);
	}

	free(s);
}

void createSudoku(int n, char* path, char* pathTem) {
	char s[50];
	FILE* fp, * fpTem;
	int board[10][10];
	for (int i = 1; i <= n; i++) {
		for (int x = 1; x <= 9; x++)for (int y = 1; y <= 9; y++)board[x][y] = 0;
		strcpy(s, path);
		strcat(s, intToString(i));
		strcat(s, ".cnf");
		fp = fopen(s, "w");

		

	}
}

void representSudoku(int* board) {
	int flag = FALSE, idx;
	for (int i = 1; i <= 9; i++) {
		if ((i - 1) % 3 == 0 && i != 1 && i != 9)printf("-----------\n");
		for (int j = 1; j <= 9; j++) {
			flag = FALSE;
			int px;
			for (int x = 1; x <= 9; x++) {
				idx = zipAll(i, j, x, FALSE);
				if (board[idx] == 0) {
					printf("\n\n\n---------something went wrong in reprensenting---------\n\n\n");
					return;
				}
				if (board[idx] == 1) {
					if (flag == FALSE) {
						flag = TRUE;
						px = x;
					}
					else {
						printf("\n\n\n---------something went wrong in reprensenting---------\n\n\n");
						return;
					}
				}
			}
			if (flag = FALSE) {
				printf("\n\n\n---------something went wrong in reprensenting---------\n\n\n");
				return;
			}

			if ((j - 1) % 3 == 0 && j != 1 && j != 9)printf("|");
			printf("%d", px);

		}
		printf("\n");
		
	}
}