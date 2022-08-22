#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "SAT_Resolver.h"
#include "BasicComponent.h"

//copy一次大概耗时1.0~1.2ms delete一次大概2.0~2.3ms (针对性能测试集ais10.cnf)

int digit(int x) {
	if (x < 10)return 1;
	else return 2;
}

int main() {
	//FILE* fp = fopen("1.txt", "r");
	
	FILE* fp1 = fopen("1.out", "w");
	//char s[25] = "testcase/";
	//char s[25] = "testyesS/";
	char s[25] = "testyesM/";
	//char s[25] = "testyesL/";
	//char s[25] = "testtest/";
	int p = 9, j;
	for (int i = 5; i <= 15; i++) {
		clockStart();

		if (digit(i) == 1) {
			s[p] = i + '0';
			s[p + 1] = '.'; s[p + 2] = 'c'; s[p + 3] = 'n'; s[p + 4] = 'f'; s[p + 5] = '\0';
		}
		else {
			if (i < 20) {
				s[p] = 1 + '0';
			}
			else if (i < 30) {
				s[p] = 2 + '0';
			}
			s[++p] = i % 10 + '0';
			s[p + 1] = '.'; s[p + 2] = 'c'; s[p + 3] = 'n'; s[p + 4] = 'f'; s[p + 5] = '\0';
			p--;
		}
		FILE* fp = fopen(s, "r");
		Data D = initByCnf(fp);
		if (DPLL(D, fp1))printf("This cnf is satisfied.\n");
		else printf("This cnf is not satisfied.\n");


		clockStop(1);

		clockStoreOut();
		clockStoreClear();

		fclose(fp);
	}
	
	fclose(fp1);
	////traverse(D, fp1);

	//FILE* fp = fopen("sudoku.csv", "r");
	//char path[] = "testsudo/";
	//char pathTem[] = "testtemp/1.cnf";
	////initCnf(fp, path, pathTem);
	////FILE* fp1 = fopen("1.txt", "w");
	////getEmptySudokuData(fp1);
	//char s[50];
	//fclose(fp);
	//clockStart();
	//for (int i = 1; i <= 1; i++) {
	//	strcpy(s, path);
	//	strcat(s, intToString(i));
	//	strcat(s, ".cnf");
	//	fp = fopen(s, "r");

	//	

	//	Data D = initByCnf(fp);
	//	int* board = DPLL(D, fp1);

	//	//for (int i = 1; i <= D->varNum; i++)printf("%d: %d\n", i, board[i]);
	//	//representSudoku(board);

	//	//if (board != FALSE)printf("This cnf is satisfied.\n");
	//	//else printf("This cnf is not satisfied.\n");


	//	if (i % 10 == 0) { clockStop(1); clockStart();}

	//	//clockStoreOut();
	//	//clockStoreClear();

	//	fclose(fp);
	//}
	/*clockStop(1);*/
	return 0;
}