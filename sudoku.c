#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SAT_Resolver.h"
#include "BasicComponent.h"

int randomQuery[10] = {0, 5,9,3,4,8,1,6,7,2};

void randomChange() {
	for (int i = 0; i < 2; i++) {
		int x = rand() % 9 + 1, y = rand() % 9 + 1;
		int tem = randomQuery[x];
		randomQuery[x] = randomQuery[y];
		randomQuery[y] = tem;
	}
}

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

//int board[10][10];

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
int decode(int x) {
	int re = 0;
	x--;
	re += x % 9 + 1;
	x /= 9;
	re += (x % 9 + 1) * 10;
	x /= 9;
	re += (x % 9 + 1) * 100;
	return re;
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
					for (int n = 1; n <= 9; n++) {
						int u = zipAll(x + (i - 1) / 3 + 1, y + (i - 1) % 3 + 1, n, TRUE);
						int v = zipAll(x + (j - 1) / 3 + 1, y + (j - 1) % 3 + 1, n, TRUE);
						fprintf(fp, "%d %d 0\n", u, v);
					}
}

//void initCnf(FILE* fpCsv, char* path, char* pathTem) {
//	while (fgetc(fpCsv) != '\n');
//	
//	char* s = (char*)malloc(sizeof(char) * 50);
//	char* tems;
//	char* b;
//	for (int i = 1; i <= 1000; i++) {
//		printf("%d\n", i);
//		strcpy(s, path);
//		tems = intToString(i);
//		strcat(s, tems);
//		strcat(s, ".cnf");
//		FILE* fp = fopen(s, "w");
//
//		b = csvReadLine(fpCsv);
//
//		int x, y;
//		for (int j = 0; j < 81; j++) {
//			x = j / 9 + 1; y = j - j / 9 * 9 + 1;
//			if (b[j] == '.')board[x][y] = 0;
//			else board[x][y] = b[j] - '0';
//		}
//
//
//		FILE* fpTem = fopen(pathTem, "w");
//		getEmptySudokuCnf(fpTem);
//		fclose(fpTem);
//		fpTem = fopen(pathTem, "r");
//		Data D = initByCnf(fpTem);
//		for (int x = 1; x <= 9; x++) {
//			for (int y = 1; y <= 9; y++) {
//				if (board[x][y] != 0) {
//					ClauseNode pC = getClause();
//					pC->firstL = getLiteral();
//					pC->firstL->nextL = pC->firstL->preL = NULL;
//					pC->firstL->varIdx = zipAll(x, y, board[x][y], FALSE);
//					pC->firstL->isRev = FALSE;
//					pC->nodeNum = 1; pC->preC = pC->nextC = NULL;
//					addClause(pC, D);
//				}
//			}
//		}
//		fclose(fpTem);
//
//		turnDataIntoCnf(D, fp);
//
//		free(tems);
//		fclose(fp);
//	}
//
//	free(s);
//}

int judgeSingleSudoku(int board[10][10]) {
	int cnt = 0;
	int tem;
	for (int row = 1; row <= 9; row++) {
		cnt = 0;
		for (int i = 1; i <= 9; i++) {
			tem = board[row][i];
			if (tem == 0)continue;
			if ((cnt & (1 << tem)) != 0)return 0;
			else cnt |= 1 << tem;
		}
	}
		
	
	for (int col = 1; col <= 9; col++) {
		cnt = 0;
		for (int i = 1; i <= 9; i++) {
			tem = board[i][col];
			if (tem == 0)continue;
			if ((cnt & (1 << tem)) != 0)return 0;
			else cnt |= 1 << tem;
		}
	}

	for (int drow = 0; drow < 9; drow += 3)
		for (int dcol = 0; dcol < 9; dcol += 3) {
			cnt = 0;
			for (int i = 1; i <= 9; i++) {
				tem = board[drow + (i - 1) / 3 + 1][dcol + (i - 1) % 3 + 1];
				if (tem == 0)continue;
				if ((cnt & (1 << tem)) != 0)return 0;
				else cnt |= 1 << tem;
			}
		}

	return 1;
}

int* solveSingleSudoku(char* s) {
	int x, y;
	int board[10][10];
	FILE* fpCnfWrite = fopen("temtem", "w");
	FILE* fpCnfRead = fopen("temtem", "r");

	for (int j = 0; j < 81; j++) {
		x = j / 9 + 1; y = j - j / 9 * 9 + 1;
		if (s[j] == '.')board[x][y] = 0;
		else board[x][y] = s[j] - '0';
	}

	getEmptySudokuCnf(fpCnfWrite);
	fclose(fpCnfWrite);
	Data D = initByCnf(fpCnfRead);

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
	fclose(fpCnfRead);

	fpCnfWrite = fopen("temtem", "w");
	turnDataIntoCnf(D, fpCnfWrite);
	fclose(fpCnfWrite);

	return startDpll(D);
}

int createSingleSudoku(int n, FILE* fpWrite) {
	int board[10][10];
	for (int i = 0; i < 10; i++)for (int j = 0; j < 10; j++)board[i][j] = 0;

	for (int i = 0; i < n; i++) {
		randomChange();
		int x, y;
		int cnt = 0;
		int isSuccess = 1;
		do {
			cnt++;
			if (cnt > 30) {
				isSuccess = 0;
				break;
			}
			x = rand() % 9 + 1, y = rand() % 9 + 1;
		} while (board[x][y] != 0);
		if (isSuccess) {
			int flag = 0;
			for (int m = 1; m <= 9; m++) {
				board[x][y] = randomQuery[m];
				if (judgeSingleSudoku(board)) {
					flag = 1;
					break;
				}
				else board[x][y] = 0;
			}
			if (!flag)isSuccess = 0;
		}
		if (!isSuccess) {
			int flag = 0;
			for (int i = 1; i <= 9; i++) {
				if (flag)break;
				for (int j = 1; j <= 9; j++) {
					if (flag)break;
					if (board[i][j] == 0) {
						x = i, y = j;
						for (int m = 1; m <= 9; m++) {
							board[x][y] = randomQuery[m];
							if (judgeSingleSudoku(board)) {
								flag = 1;
								isSuccess = 1;
								break;
							}
							else board[x][y] = 0;
						}
					}
				}
			}
			if (flag)isSuccess = 1;
		}
		if (!isSuccess) {
			printf("失败了\n");
			return 0;
		}
	}

	char s[85];
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
			if (board[i][j] > 0) s[(i - 1) * 9 + j - 1] = board[i][j] + '0';
			else s[(i - 1) * 9 + j - 1] = '.';
	int* re = solveSingleSudoku(s);
	if (re > 2) {
		for (int i = 1; i <= 9; i++)
			for (int j = 1; j <= 9; j++)
				if (board[i][j] > 0) fprintf(fpWrite, "%d", board[i][j]);
				else fprintf(fpWrite, ".");
		return 1;
	}
	else {
		return 0;
	}

	
}

//void representSudoku(int* board) {
//	int flag = FALSE, idx;
//	for (int i = 1; i <= 9; i++) {
//		if ((i - 1) % 3 == 0 && i != 1 && i != 9)printf("-----------\n");
//		for (int j = 1; j <= 9; j++) {
//			flag = FALSE;
//			int px;
//			for (int x = 1; x <= 9; x++) {
//				idx = zipAll(i, j, x, FALSE);
//				if (board[idx] == 0) {
//					printf("\n\n\n---------something went wrong in reprensenting---------\n\n\n");
//					return;
//				}
//				if (board[idx] == 1) {
//					if (flag == FALSE) {
//						flag = TRUE;
//						px = x;
//					}
//					else {
//						printf("\n\n\n---------something went wrong in reprensenting---------\n\n\n");
//						return;
//					}
//				}
//			}
//			if (flag = FALSE) {
//				printf("\n\n\n---------something went wrong in reprensenting---------\n\n\n");
//				return;
//			}
//
//			if ((j - 1) % 3 == 0 && j != 1 && j != 9)printf("|");
//			printf("%d", px);
//
//		}
//		printf("\n");
//		
//	}
//}

void transformSingleSudokuIntoCnf(FILE* fpRead, char* fileName) {
	int s[85];
	int x, y;
	int board[10][10];
	FILE* fpCnfWrite = fopen(fileName, "w");
	FILE* fpCnfRead = fopen(fileName, "r");
	fscanf(fpRead, "%s", s);

	for (int j = 0; j < 81; j++) {
		x = j / 9 + 1; y = j - j / 9 * 9 + 1;
		if (s[j] == '.')board[x][y] = 0;
		else board[x][y] = s[j] - '0';
	}

	getEmptySudokuCnf(fpCnfWrite);
	fclose(fpCnfWrite);
	Data D = initByCnf(fpCnfRead);

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
	fclose(fpCnfRead);

	fpCnfWrite = fopen(fileName, "w");
	turnDataIntoCnf(D, fpCnfWrite);
	fclose(fpCnfWrite);
}



void printSingleSudoku(int board[10][10]) {
	for (int i = 1; i <= 9; i++) {
		if ((i - 1) % 3 == 0 && i != 1)printf("-----------\n");
		for (int j = 1; j <= 9; j++) {
			if ((j - 1) % 3 == 0 && j != 1)printf("|");
			if (board[i][j] == 0)printf(" ");
			else printf("%d", board[i][j]);
		}
		printf("\n");
	}
}

int solveDpllSingleSudoku(int board[10][10], int s[]) {
	int* re = solveSingleSudoku(s);
	if (re > 2) {
		for (int i = 1; i <= 729; i++) {
			if (re[i] == -1)continue;
			int di = decode(i);
			int x = di / 100, y = di / 10 % 10, n = di % 10;
			board[x][y] = n;
		}
		return 1;
	}
	else return 0;
}

void playSingleSudoku(FILE* fpRead) {
	char s[85];
	int board[10][10], originBoard[10][10], x, y, row, col, num, isSatisified = 0;
	int ansBoard[10][10];
	fscanf(fpRead, "%s", s);
	for (int j = 0; j < 81; j++) {
		x = j / 9 + 1; y = j - j / 9 * 9 + 1;
		if (s[j] == '.')originBoard[x][y] = board[x][y] = 0;
		else originBoard[x][y] = board[x][y] = s[j] - '0';
	}

	int isSolved = 0;
	do {
		system("cls");
		printSingleSudoku(board);
		int flag = 0;

		for (int i = 1; i <= 9; i++)for (int j = 1; j <= 9; j++)if (board[i][j] == 0)flag = 1;
		if (flag == 0) { printf("\nYou Win!\n"); break; }

		printf("\n输入格式: 行 列 数字\n");
		printf("其它: -1: exit -2: 输出原始数独结果 -3: 检验当前数独是否有解\n");
		printf("---请输入:\n");

		scanf("%d", &row);
		if (row == -1) {
			break;
		}
		else if (row == -2) {
			if (isSolved != 1) {
				isSolved = 1;
				if (!solveDpllSingleSudoku(ansBoard, s))printf("数独无解\n");
				else isSatisified = 1;
			}
			printf("\n\n原始数独结果:\n");
			printSingleSudoku(ansBoard);
			getchar(); getchar();
			continue;
		}
		else if (row == -3) {
			if (isSolved != 1) {
				isSolved = 1;
				if (solveDpllSingleSudoku(ansBoard, s))isSatisified = 1;
			}
			if (isSatisified)printf("数独有解\n");
			else printf("数独无解\n");
			getchar(); getchar();
			continue;
		}

		scanf("%d%d", &col, &num);
		if (!(row >= 1 && row <= 9 && col >= 1 && col <= 9 && num >=1 && num <= 9)) {
			printf("输入不合法\n");
		}
		if (board[row][col] != 0) {
			printf("当前格子已有数字\n");
			continue;
		}
		board[row][col] = num;
		if (!judgeSingleSudoku(board)) {
			board[row][col] = 0;
			printf("当前输入无法满足\n");
		}
		
	} while (1);
	printf("\n\nExit\n");
}


int zipAllDouble(int x, int y, int n, int isRev, int which) {
	return 729 * (which - 1) * (isRev ? -1 : 1) + zipAll(x, y, n, isRev);
}
int decodeDouble(int x) {
	int re = 1000;
	x--;
	if (x / 729)re += 1000, x -= 729;
	x++;
	re += decode(x);
	return re;
}

void getEmptyDoubleSudokuCnf(FILE* fp) {	//重复了重复的格子，可以优化
	fprintf(fp, "p cnf %d %d\n", 729 * 2, 11988 * 2);

	//格 列 行 宫 存在
	for (int which = 1; which <= 2; which++) {
		for (int x = 1; x <= 9; x++)
			for (int y = 1; y <= 9; y++) {
				for (int n = 1; n <= 9; n++)
					fprintf(fp, "%d ", zipAllDouble(x, y, n, FALSE, which));
				fprintf(fp, "0\n");
			}

		for (int x = 1; x <= 9; x++)
			for (int n = 1; n <= 9; n++) {
				for (int y = 1; y <= 9; y++)
					fprintf(fp, "%d ", zipAllDouble(x, y, n, FALSE, which));
				fprintf(fp, "0\n");
			}
		for (int y = 1; y <= 9; y++)
			for (int n = 1; n <= 9; n++) {
				for (int x = 1; x <= 9; x++)
					fprintf(fp, "%d ", zipAllDouble(x, y, n, FALSE,which));
				fprintf(fp, "0\n");
			}

		for (int x = 0; x < 9; x += 3)
			for (int y = 0; y < 9; y += 3)
				for (int n = 1; n <= 9; n++) {
					for (int i = 1; i <= 9; i++)
						fprintf(fp, "%d ", zipAllDouble(x + (i - 1) / 3 + 1, y + (i - 1) % 3 + 1, n, FALSE, which));
					fprintf(fp, "0\n");
				}

		//格 列 行 宫 不冲突

		for (int x = 1; x <= 9; x++)
			for (int y = 1; y <= 9; y++)
				for (int n = 1; n <= 8; n++)
					for (int m = n + 1; m <= 9; m++)
						fprintf(fp, "%d %d 0\n", zipAllDouble(x, y, n, TRUE, which), zipAllDouble(x, y, m, TRUE, which));

		for (int x = 1; x <= 9; x++)
			for (int y = 1; y <= 8; y++)
				for (int i = y + 1; i <= 9; i++)
					for (int n = 1; n <= 9; n++)
						fprintf(fp, "%d %d 0\n", zipAllDouble(x, y, n, TRUE, which), zipAllDouble(x, i, n, TRUE, which));

		for (int y = 1; y <= 9; y++)
			for (int x = 1; x <= 8; x++)
				for (int i = x + 1; i <= 9; i++)
					for (int n = 1; n <= 9; n++)
						fprintf(fp, "%d %d 0\n", zipAllDouble(x, y, n, TRUE, which), zipAllDouble(i, y, n, TRUE, which));

		for (int x = 0; x < 9; x += 3)
			for (int y = 0; y < 9; y += 3)
				for (int i = 1; i <= 8; i++)
					for (int j = i + 1; j <= 9; j++)
						for (int n = 1; n <= 9; n++) {
							int u = zipAllDouble(x + (i - 1) / 3 + 1, y + (i - 1) % 3 + 1, n, TRUE, which);
							int v = zipAllDouble(x + (j - 1) / 3 + 1, y + (j - 1) % 3 + 1, n, TRUE, which);
							fprintf(fp, "%d %d 0\n", u, v);
						}
	}

	for(int x = 7 ; x <= 9 ; x++)
		for (int y = 7; y <= 9; y++) 
			for (int n = 1; n <= 9; n++) {
				int u = zipAllDouble(x, y, n, FALSE, 1);
				int v = zipAllDouble(x - 6, y - 6, n, FALSE, 2);
				fprintf(fp, "%d %d 0\n", u, -v);
				fprintf(fp, "%d %d 0\n", -u, v);
			}
	
}

int judgeDoubleSudoku(int board[2][10][10]) {	//0右下和1左上
	if (judgeSingleSudoku(board[0]) && judgeSingleSudoku(board[1])) {
		for (int i = 7; i <= 9; i++)for (int j = 7; j <= 9; j++)if (board[0][i][j] != board[1][i - 6][j - 6]) {
			printf("Error in judgeDoubleSudoku");
			return 0;
		}
		return 1;
	}
	return 0;
}

int isDuplicate(int x, int y, int whichIdx) {
	if (whichIdx == 0) {
		if (x >= 7 && y >= 7)return 1;
		else return 0;
	}
	else if (whichIdx == 1) {
		if (x <= 3 && y <= 3)return 1;
		else return 0;
	}
	else {
		printf("Error in isDuplicate");
	}
}

void assignValue(int board[2][10][10], int x, int y, int n, int whichIdx) {
	board[whichIdx][x][y] = n;
	if (isDuplicate(x, y, whichIdx)) {
		if (whichIdx == 0) {
			board[whichIdx ^ 1][x - 6][y - 6] = n;
		}
		else {
			board[whichIdx ^ 1][x + 6][y + 6] = n;
		}
	}
}

Data transformDoubleSudokuIntoData(char* s) {
	int x, y;
	int board[2][10][10];
	FILE* fpCnfWrite = fopen("temtem", "w");
	for (int j = 0; j < 81; j++) {
		x = j / 9 + 1; y = j - j / 9 * 9 + 1;
		if (s[j] == '.')board[0][x][y] = 0;
		else board[0][x][y] = s[j] - '0';

		if (s[j + 81] == '.')board[1][x][y] = 0;
		else board[1][x][y] = s[j + 81] - '0';
	}

	getEmptyDoubleSudokuCnf(fpCnfWrite);
	fclose(fpCnfWrite);

	FILE* fpCnfRead = fopen("temtem", "r");
	Data D = initByCnf(fpCnfRead);

	for (int x = 1; x <= 9; x++) {
		for (int y = 1; y <= 9; y++) {
			if (board[0][x][y] != 0) {
				ClauseNode pC = getClause();
				pC->firstL = getLiteral();
				pC->firstL->nextL = pC->firstL->preL = NULL;
				pC->firstL->varIdx = zipAllDouble(x, y, board[0][x][y], FALSE, 1);
				pC->firstL->isRev = FALSE;
				pC->nodeNum = 1; pC->preC = pC->nextC = NULL;
				addClause(pC, D);
			}
		}
	}
	for (int x = 1; x <= 9; x++) {
		for (int y = 1; y <= 9; y++) {
			if (x <= 3 && y <= 3)continue;
			if (board[1][x][y] != 0) {
				ClauseNode pC = getClause();
				pC->firstL = getLiteral();
				pC->firstL->nextL = pC->firstL->preL = NULL;
				pC->firstL->varIdx = zipAllDouble(x, y, board[1][x][y], FALSE, 2);
				pC->firstL->isRev = FALSE;
				pC->nodeNum = 1; pC->preC = pC->nextC = NULL;
				addClause(pC, D);
			}
		}
	}
	return D;
}

int* solveDoubleSudoku(char* s) {
	int x, y;
	int board[2][10][10];
	FILE* fpCnfWrite = fopen("temtem", "w");
	FILE* fpCnfRead = fopen("temtem", "r");

	for (int j = 0; j < 81; j++) {
		x = j / 9 + 1; y = j - j / 9 * 9 + 1;
		if (s[j] == '.')board[0][x][y] = 0;
		else board[0][x][y] = s[j] - '0';

		if (s[j + 81] == '.')board[1][x][y] = 0;
		else board[1][x][y] = s[j + 81] - '0';
	}

	getEmptyDoubleSudokuCnf(fpCnfWrite);
	fclose(fpCnfWrite);
	Data D = initByCnf(fpCnfRead);

	for (int x = 1; x <= 9; x++) {
		for (int y = 1; y <= 9; y++) {
			if (board[0][x][y] != 0) {
				ClauseNode pC = getClause();
				pC->firstL = getLiteral();
				pC->firstL->nextL = pC->firstL->preL = NULL;
				pC->firstL->varIdx = zipAllDouble(x, y, board[0][x][y], FALSE, 1);
				pC->firstL->isRev = FALSE;
				pC->nodeNum = 1; pC->preC = pC->nextC = NULL;
				addClause(pC, D);
			}
		}
	}
	for (int x = 1; x <= 9; x++) {
		for (int y = 1; y <= 9; y++) {
			if (x <= 3 && y <= 3)continue;
			if (board[1][x][y] != 0) {
				ClauseNode pC = getClause();
				pC->firstL = getLiteral();
				pC->firstL->nextL = pC->firstL->preL = NULL;
				pC->firstL->varIdx = zipAllDouble(x, y, board[1][x][y], FALSE, 2);
				pC->firstL->isRev = FALSE;
				pC->nodeNum = 1; pC->preC = pC->nextC = NULL;
				addClause(pC, D);
			}
		}
	}
	fclose(fpCnfRead);

	fpCnfWrite = fopen("temtem", "w");
	turnDataIntoCnf(D, fpCnfWrite);
	fclose(fpCnfWrite);

	return startDpll(D);
}

int createDoubleSudoku(int n, FILE* fpWrite) {
	int board[2][10][10];
	for(int which = 0; which < 2; which++) for (int i = 0; i < 10; i++)for (int j = 0; j < 10; j++)board[which][i][j] = 0;

	for (int i = 0; i < n; i++) {
		randomChange();
		int x, y, which;
		int cnt = 0;
		int isSuccess = 1;
		do {
			cnt++;
			if (cnt > 30) {
				isSuccess = 0;
				break;
			}
			x = rand() % 9 + 1, y = rand() % 9 + 1, which = rand() & 1;
		} while (board[which][x][y] != 0);
		if (isSuccess) {
			int flag = 0;
			for (int m = 1; m <= 9; m++) {
				assignValue(board, x, y, randomQuery[m], which);
				if (judgeDoubleSudoku(board)) {
					flag = 1;
					break;
				}
				else {
					assignValue(board, x, y, 0, which);
				}
			}
			if (!flag)isSuccess = 0;
		}
		if (!isSuccess) {
			int flag = 0;
			for (which = 0; which < 2; which++)
				for (int i = 1; i <= 9; i++) {
					if (flag)break;
					for (int j = 1; j <= 9; j++) {
						if (flag)break;
						if (board[which][i][j] != 0) {
							x = i, y = j;
							for (int m = 1; m <= 9; m++) {
								assignValue(board, x, y, randomQuery[m], which);
								if (judgeSingleSudoku(board)) {
									flag = 1;
									isSuccess = 1;
									break;
								}
								else { 
									assignValue(board, x, y, 0, which);
								}
							}
						}
					}
				}
			if (flag)isSuccess = 1;
		}
		if (!isSuccess) {
			printf("失败了\n");
			return 0;
		}
	}

	char s[170];
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
			if (board[0][i][j] > 0) s[(i - 1) * 9 + j - 1] = board[0][i][j] + '0';
			else s[(i - 1) * 9 + j - 1] = '.';
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
			if (board[1][i][j] > 0) s[(i - 1) * 9 + j - 1 + 81] = board[1][i][j] + '0';
			else s[(i - 1) * 9 + j - 1 + 81] = '.';
	int* re = solveDoubleSudoku(s);
	if (re > 2) {
		for (int which = 0; which < 2; which++)
			for (int i = 1; i <= 9; i++)
				for (int j = 1; j <= 9; j++)
					if (board[which][i][j] > 0) fprintf(fpWrite, "%d", board[which][i][j]);
					else fprintf(fpWrite, ".");
		return 1;
	}
	else {
		return 0;
	}
}

void addClauseEasy(Data D, int varIdx, int isRev) {
	ClauseNode pC = getClause();
	pC->firstL = getLiteral();
	pC->firstL->nextL = pC->firstL->preL = NULL;
	pC->firstL->varIdx = varIdx;
	pC->firstL->isRev = FALSE;
	pC->nodeNum = 1; pC->preC = pC->nextC = NULL;
	addClause(pC, D);
}

void createDoubleSudokuUnique(FILE* fpWrite) {
	int board[2][10][10];
	int sp = 0;
	int stack[10];
	int needToBeDeleteBoard[2][10][10];
	char s[170];

	for (int which = 0; which < 2; which++) 
		for (int i = 0; i < 10; i++)
			for (int j = 0; j < 10; j++)
				board[which][i][j] = needToBeDeleteBoard[which][i][j] = 0;

	

	FILE* fp = fopen("temtem", "w");
	while (!createDoubleSudoku(35, fp)) {
		fclose(fp);
		fp = fopen("temtem", "w");
	};
	fclose(fp);

	fp = fopen("temtem", "r");
	fscanf(fp, "%s", s);
	fclose(fp);

	for (int whichIdx = 0; whichIdx < 2; whichIdx++)
		for (int j = 0; j < 81; j++) {
			int x = j / 9 + 1, y = j - j / 9 * 9 + 1;
			if (s[j + whichIdx * 81] == '.')board[whichIdx][x][y] = 0;
			else board[whichIdx][x][y] = s[j + whichIdx * 81] - '0';
		}

	Data D = transformDoubleSudokuIntoData(s);
	Data DC = NULL;

	

	int i = 0;
	for (int whichIdx = 0; whichIdx < 2; whichIdx++) {
		for (int x = 1; x <= 9; x++) {
			for (int y = 1; y <= 9; y++) {
				if (board[whichIdx][x][y])i++;
			}
		}
	}

	for (; i < 81 * 2; i++) {
		//randomChange();
		int x, y, whichIdx;
		int cnt = rand() % (81*2 - i) + 1;
		int isSuccess = 1;
		int flag = 0;
		for (int w = 0; w < 2 && flag == 0; w++) {
			for (int row = 1; row <= 9 && flag == 0; row++) {
				for (int col = 1; col <= 9 && flag == 0; col++) {
					if (board[w][row][col] == 0) {
						cnt--;
						if (cnt != 0) continue;
						x = row, y = col, whichIdx = w;
						flag = 1;
					}
				}
			}
		}
		sp = 0;
		for (int j = 1; j <= 9; j++) {
			assignValue(board, x, y, j, whichIdx);
			if (!judgeDoubleSudoku(board)) {
				assignValue(board, x, y, 0, whichIdx);
				continue;
			};
			assignValue(board, x, y, 0, whichIdx);

			addClauseEasy(D, zipAllDouble(x, y, j, FALSE, whichIdx + 1), FALSE);
			DC = copyAll(D);
			int* re = startDpll(DC);
			deleteData(DC);
			clockStoreStop();
			clockStoreClear();
			DC = NULL;
			if (re > 2) {
				stack[sp++] = j;
			}
			deleteClause(D->firstC, D);
		}

		int choice;
		if (sp == 1) {
			choice = 0;
			needToBeDeleteBoard[whichIdx][x][y] = 1;
		}
		else if (sp >= 2) {
			choice = rand() % sp;
		}
		else 
			printf("error in createDoubleSudokuUnique 2");
		if (isDuplicate(x, y, whichIdx))i++;
		assignValue(board, x, y, stack[choice], whichIdx);
		addClauseEasy(D, zipAllDouble(x, y, stack[choice], FALSE, whichIdx + 1), FALSE);
	}
	for (int whichIdx = 0; whichIdx < 2; whichIdx++) {
		for (int i = 1; i <= 9; i++) {
			for (int j = 1; j <= 9; j++) {
				if (needToBeDeleteBoard[whichIdx][i][j])assignValue(board, i, j, 0, whichIdx);
			}
		}
	}

	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
			if (board[0][i][j] > 0) s[(i - 1) * 9 + j - 1] = board[0][i][j] + '0';
			else s[(i - 1) * 9 + j - 1] = '.';
	for (int i = 1; i <= 9; i++)
		for (int j = 1; j <= 9; j++)
			if (board[1][i][j] > 0) s[(i - 1) * 9 + j - 1 + 81] = board[1][i][j] + '0';
			else s[(i - 1) * 9 + j - 1 + 81] = '.';
	for (int which = 0; which < 2; which++)
		for (int i = 1; i <= 9; i++)
			for (int j = 1; j <= 9; j++)
				if (board[which][i][j] > 0) fprintf(fpWrite, "%d", board[which][i][j]);
				else fprintf(fpWrite, ".");
}

void printDoubleSudoku(int board[2][10][10]) {
	for (int i = 1; i <= 6; i++) {
		for (int j = 1; j <= 9; j++) {
			if ((j - 1) % 3 == 0 && j != 1)printf("|");
			if (board[0][i][j] == 0)printf(" ");
			else printf("%d", board[0][i][j]);
		}
		printf("\n");
		if(i % 3 == 0)printf("-----------\n");
	}
	for (int i = 7; i <= 9; i++) {
		for (int j = 1; j <= 9; j++) {
			if (board[0][i][j] == 0)printf(" ");
			else printf("%d", board[0][i][j]);
			if (j % 3 == 0) printf("|");
		}
		for (int j = 4; j <= 9; j++) {
			if ((j - 1) % 3 == 0 && j != 4) printf("|");
			if (board[1][i][j] == 0)printf(" ");
			else printf("%d", board[1][i][j]);
		}
		printf("\n");
	}

	for (int i = 4; i <= 9; i++) {
		printf("        ");
		if ((i - 1) % 3 == 0)printf("-----------\n        ");
		for (int j = 1; j <= 9; j++) {
			if ((j - 1) % 3 == 0 && j != 1)printf("|");
			if (board[1][i][j] == 0)printf(" ");
			else printf("%d", board[1][i][j]);
		}
		printf("\n");
	}
}

void transformDoubleSudokuIntoCnf(FILE* fpRead, char* fileName) {
	int s[170];
	int x, y;
	int board[2][10][10];
	FILE* fpCnfWrite = fopen(fileName, "w");
	FILE* fpCnfRead = fopen(fileName, "r");
	fscanf(fpRead, "%s", s);

	for (int j = 0; j < 81; j++) {
		x = j / 9 + 1; y = j - j / 9 * 9 + 1;
		if (s[j] == '.')board[0][x][y] = 0;
		else board[0][x][y] = s[j] - '0';

		if (s[j + 81] == '.')board[1][x][y] = 0;
		else board[1][x][y] = s[j + 81] - '0';
	}


	getEmptyDoubleSudokuCnf(fpCnfWrite);
	fclose(fpCnfWrite);
	Data D = initByCnf(fpCnfRead);

	for (int x = 1; x <= 9; x++) {
		for (int y = 1; y <= 9; y++) {
			if (board[0][x][y] != 0) {
				ClauseNode pC = getClause();
				pC->firstL = getLiteral();
				pC->firstL->nextL = pC->firstL->preL = NULL;
				pC->firstL->varIdx = zipAllDouble(x, y, board[0][x][y], FALSE, 1);
				pC->firstL->isRev = FALSE;
				pC->nodeNum = 1; pC->preC = pC->nextC = NULL;
				addClause(pC, D);
			}
		}
	}
	for (int x = 1; x <= 9; x++) {
		for (int y = 1; y <= 9; y++) {
			if (x <= 3 && y <= 3)continue;
			if (board[1][x][y] != 0) {
				ClauseNode pC = getClause();
				pC->firstL = getLiteral();
				pC->firstL->nextL = pC->firstL->preL = NULL;
				pC->firstL->varIdx = zipAllDouble(x, y, board[1][x][y], FALSE, 2);
				pC->firstL->isRev = FALSE;
				pC->nodeNum = 1; pC->preC = pC->nextC = NULL;
				addClause(pC, D);
			}
		}
	}

	fclose(fpCnfRead);

	fpCnfWrite = fopen(fileName, "w");
	turnDataIntoCnf(D, fpCnfWrite);
	fclose(fpCnfWrite);
}

int solveDpllDoubleSudoku(int board[2][10][10], char s[]) {
	int* re = solveDoubleSudoku(s);
	if (re > 2) {
		for (int i = 1; i <= 729 * 2; i++) {
			if (re[i] == -1)continue;
			int di = decodeDouble(i);
			int whichIdx = di / 1000 - 1;
			di %= 1000;
			int x = di / 100, y = di / 10 % 10, n = di % 10;
			board[whichIdx][x][y] = n;
		}
		return 1;
	}
	else return 0;
}

void playDoubleSudoku(FILE* fpRead) {
	char s[170];
	int board[2][10][10], originBoard[2][10][10], x, y, row, col, num, isSatisified = 0, which;
	int ansBoard[2][10][10];
	int opCnt = 0;
	fscanf(fpRead, "%s", s);
	for(int whichIdx = 0; whichIdx < 2; whichIdx++)
		for (int j = 0; j < 81; j++) {
			x = j / 9 + 1; y = j - j / 9 * 9 + 1;
			if (s[j + whichIdx * 81] == '.')originBoard[whichIdx][x][y] = board[whichIdx][x][y] = 0;
			else originBoard[whichIdx][x][y] = board[whichIdx][x][y] = s[j + whichIdx * 81] - '0';
		}

	Data originData = transformDoubleSudokuIntoData(s);
	Data operateData = copyAll(originData);
	Data DC = NULL;


	int isSolved = 0;
	do {
		system("cls");
		printDoubleSudoku(board);
		int flag = 0;

		for (int whichIdx = 0; whichIdx < 2; whichIdx++)for (int i = 1; i <= 9; i++)for (int j = 1; j <= 9; j++)if (board[whichIdx][i][j] == 0)flag = 1;
		if (flag == 0) { printf("\nYou Win(TODO)!\n"); break; }

		printf("\n输入格式: 行 列 数字 哪个数独(1:左上角 2:右下角)\n");
		printf("其它: -1: exit -2: 输出原始数独结果 -3: 检验当前数独是否有解 -4:当前结果 -5:撤回\n");
		printf("---请输入:\n");

		scanf("%d", &row);
		if (row == -1) {
			break;
		}
		else if (row == -2) {
			if (isSolved != 1) {
				isSolved = 1;
				if (!solveDpllDoubleSudoku(ansBoard, s)) { printf("数独无解\n"); break; }
				else isSatisified = 1;
			}
			printf("\n\n原始数独结果:\n");
			printDoubleSudoku(ansBoard);
			getchar(); getchar();
			continue;
		}
		else if (row == -3) {
			if (isSolved != 1) {
				isSolved = 1;
				if (solveDpllDoubleSudoku(ansBoard, s))isSatisified = 1;
			}
			if (isSatisified)printf("数独有解\n");
			else printf("数独无解\n");
			getchar(); getchar();
			continue;
		}
		else if (row == -4) {
			char sTem[170];
			DC = copyAll(operateData);
			int* re = startDpll(DC);
			deleteData(DC);
			clockStoreStop();
			clockStoreClear();
			DC = NULL;
			if (re > 2) {
				for (int i = 1; i <= 729 * 2; i++) {
					if (re[i] == -1)continue;
					int di = decodeDouble(i);
					int whichIdx = di / 1000 - 1;
					di %= 1000;
					int x = di / 100, y = di / 10 % 10, n = di % 10;
					ansBoard[whichIdx][x][y] = n;
				}
				printf("\n\n");
				printDoubleSudoku(ansBoard);
			}
			else printf("\n\n当前数独无解\n");
			getchar(); getchar();
			continue;
		} 
		else if (row == -5) {
			if (opCnt > 0) {
				opCnt--;
				ClauseNode pC = operateData->firstC;
				int index = decodeDouble(pC->firstL->varIdx);
				deleteClause(pC, operateData);
				if (index > 1000) which = 1, index -= 1000; else which = 0;
				num = index % 10; index /= 10;
				col = index % 10; index /= 10;
				row = index % 10; index /= 10;
				assignValue(board, row, col, 0, which - 1);
				continue;
			}
			else printf("\n无法撤回\n");
			getchar(); getchar();
			continue;
		}

		scanf("%d%d%d", &col, &num, &which);
		if (!(row >= 1 && row <= 9 && col >= 1 && col <= 9 && num >= 1 && num <= 9 && which >= 1 && which <= 2)) {
			printf("输入不合法\n");
			getchar(); getchar();
		}
		which--;
		if (board[which][row][col] != 0) {
			printf("当前格子已有数字\n");
			getchar(); getchar();
			continue;
		}
		assignValue(board, row, col, num, which);
		if (!judgeDoubleSudoku(board)) {
			assignValue(board, row, col, 0, which);
			printf("当前输入无法满足\n");
			getchar(); getchar();
			continue;
		}

		opCnt++;
		ClauseNode pC = getClause();
		pC->firstL = getLiteral();
		pC->firstL->nextL = pC->firstL->preL = NULL;
		pC->firstL->varIdx = zipAllDouble(row, col, board[which][row][col], FALSE, which + 1);
		pC->firstL->isRev = FALSE;
		pC->nodeNum = 1; pC->preC = pC->nextC = NULL;
		addClause(pC, operateData);

	} while (1);
	printf("\n\nExit");

}