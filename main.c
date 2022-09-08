#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <Windows.h>

#include "SAT_Resolver.h"
#include "BasicComponent.h"

//copy一次大概耗时1.0~1.2ms delete一次大概2.0~2.3ms (针对性能测试集ais10.cnf)

int main() {
	FILE* fpRead = NULL, *fpWrite = NULL, *fp1 = NULL, *fp2 = NULL;
	Data D = NULL;
	int* res = 0;
	int spendTime = 0, x, y;
	int boardSingle[10][10];
	char fileName[40];
	int OP = 1;
	int* re;
	int tem, flag = 0, row, col, num;
	int timeConsume[3][20];
	for (int i = 0; i < 20; i++)timeConsume[0][i] = timeConsume[1][i] = timeConsume[2][i] = -1;
	srand(35);
while (OP != 0) {
	system("cls");
	printf("----------------------Normal method---------------------------\n");
	printf("1.  Read and parse cnf          2.  Output parsed cnf\n");
	printf("3.  Dpll                        4.  Output Dpll timeClock\n");
	printf("5.  -Dpll result save as cnf 	6.  Clear Dpll result\n");
	printf("7.  -Output result from cnf     8.  save result\n");
	printf("------------------------Generator-----------------------------\n");
	printf("9.  Generate single sudoku cnf  10. Generate Double sudoku cnf\n");
	printf("---------------------improve method---------------------------\n");
	printf("11. Better Dpll                 12. print the improve result  \n");
	printf("-------------------Play single sudoku-------------------------\n");
	printf("13. play from a sudoku file\n");
	printf("-------------------Play double sudoku-------------------------\n");
	printf("14. play from a sudoku file\n");
	printf("--------------------------------------------------------------\n");
	printf("15. set time limit\n");
	printf("--------------------------------------------------------------\n");
	printf("16. Dpll3\n");
	printf("--------------------------------------------------------------\n");
	printf("17. verify\n");
	printf("--------------------------------------------------------------\n");
	printf("18. Generate Double sudoku cnf which is unique\n");
	printf("--------------------------------------------------------------\n");
	scanf("%d", &OP);
	system("cls");
	switch (OP) {
	case 1:
		printf("---请选择输入名字的格式\n");
		printf("1: 名字.cnf\n2: 文件夹名.../名字.cnf\n");
		scanf("%d", &tem);
		if (tem >= 1 && tem <= 2) {
			printf("---请输入:\n");
		}
		if (tem == 1) {
			scanf("%s", fileName);
			fpRead = openCnf(fileName, "r");
			flag = 1;
		}
		else if (tem == 2) {
			scanf("%s", fileName);
			fpRead = fopen(fileName, "r");
			flag = 1;
		}
		else {
			printf("选择错误\n");
		}
		if (flag) {
			if (fpRead == NULL) {
				printf("文件打开失败\n");
			}
			else {
				printf("文件打开成功\n");
				flag = 2;
			}
		}
		if (flag == 2) {
			if (D != NULL) {
				deleteData(D);
			}
			D = initByCnf(fpRead);
			if (D != NULL)printf("文件解析成功\n");
			else printf("文件解析失败\n");
		}
		fclose(fpRead);
		getchar(); getchar();
		break;


	case 2:
		printf("---请选择输出方式(1: 屏幕 2: 文件):\n");
		scanf("%d", &tem);
		if (tem == 1) {
			outputParsedCnfIntoCnf(D);
		}
		else if (tem == 2) {
			printf("---请输入要保存的文件名:\n");
			scanf("%s", fileName);
			fpWrite = fopen(fileName, "w");
			turnDataIntoCnf(D, fpWrite);
			fclose(fpWrite);
			printf("保存成功\n");
		}
		else {
			printf("输出错误\n");
		}
		getchar(); getchar();
		break;


	case 3:
		clockStoreClear();
		clockStoreStart();
		//clockStart();
		dpllInit(D);
		re = DPLL(D);
		dpllEnd(D);
		//clockStop(1);
		clockStoreStop();
		spendTime = getClockStored();
		printf("---请输入该样例的编号:\n");
		scanf("%d", &tem);
		timeConsume[0][tem] = spendTime;
		system("cls");
		printf("---Dpll over\n");
		printf("---The result:\n");
		if (re == FALSE) {
			printf("无解\n");
			res = 0;
		}
		else if (re == 2) {
			printf("超出时间限制\n");
			res = 2;
		}
		else {
			printf("有解\n");
			res = re;
		}
		getchar(); getchar();
		break;

	case 4:
		printf("Time cost: %dms\n", spendTime);
		clockStoreOut();
		getchar(); getchar();
		break;

	case 5:
		printf("---请输入保存的名字(包括.cnf):\n");
		scanf("%d", fileName);
		fpWrite = openPath("cnf", fileName, "w");
		turnDataIntoCnf(D, fpWrite);
		printf("保存成功\n");
		fclose(fpWrite);
		getchar(); getchar();
		break;

	case 6:
		if (D == NULL) {
			printf("已清除\n");
		}
		else {
			deleteData(D);
			D = NULL;
			printf("清除成功\n");
		}
		getchar(); getchar();
		break;

	case 7:
		printf("---请输入名字:\n");
		scanf("%s", fileName);
		fpRead = openCnf(fileName, "r");
		char ch;
		while ((ch = fgetc(fpRead)) != EOF) {
			putchar(ch);
		}
		fclose(fpRead);
		getchar(); getchar();
		break;

	case 8:
		printf("---请输入保存的名字:\n");
		scanf("%s", fileName);
		fpWrite = fopen(fileName, "w");
		outputResult(res, D->varNum, fpWrite, spendTime);
		fclose(fpWrite);
		fpWrite = NULL;
		printf("保存成功\n");
		getchar(); getchar();
		break;

	case 9:
		printf("---请输入保存的名字:\n");
		scanf("%s", fileName);
		fpWrite = fopen(fileName, "w");
		while (!createSingleSudoku(24, fpWrite)) {
			fclose(fpWrite);
			fpWrite = fopen(fileName, "w");
		};
		fclose(fpWrite);

		printf("生成完毕\n");

		getchar(); getchar();
		break;

	case 10:
		printf("---请输入保存的名字:\n");
		scanf("%s", fileName);
		fpWrite = fopen(fileName, "w");
		while (!createDoubleSudoku(20, fpWrite)) {
			fclose(fpWrite);
			fpWrite = fopen(fileName, "w");
		};
		fclose(fpWrite);

		printf("生成完毕\n");

		getchar(); getchar();
		break;

	case 11:
		clockStoreClear();
		clockStoreStart();
		//clockStart();
		dpllInit(D);
		re = BtDPLL(D);
		dpllEnd(D);
		//clockStop(1);
		clockStoreStop();
		spendTime = getClockStored();
		printf("---请输入该样例的编号:\n");
		scanf("%d", &tem);
		timeConsume[1][tem] = spendTime;
		system("cls");
		printf("---Dpll over\n");
		printf("---The result:\n");
		if (re == FALSE) {
			printf("无解\n");
			res = 0;
		}
		else if (re == 2) {
			printf("超出时间限制\n");
			res = 2;
		}
		else {
			printf("有解\n");
			res = re;
		}
		getchar(); getchar();
		break;

	case 12:
		printf("---请输入所要比较的文件编号:\n");
		scanf("%d", &tem);
		printf("---请输入所要比较的dpll(左边为未优化的):\n");
		scanf("%d%d", &x, &y);
		x--, y--;
		if (!(tem > 0 && tem < 20))printf("输入错误\n");
		else {
			if (timeConsume[x][tem] == -1 || timeConsume[y][tem] == -1)printf("未对该文件各进行一次dpll\n");
			else printf("优化率为:%f%%\n", (float)(timeConsume[x][tem] - timeConsume[y][tem]) / (timeConsume[x][tem] + 0.01) * 100);
		}
		getchar(); getchar();
		break;

	case 13:
		printf("---请输入要导入的数独文件名:\n");
		scanf("%s", fileName);
		fpRead = fopen(fileName, "r");
		
		playSingleSudoku(fpRead);

		fclose(fpRead);

		getchar(); getchar();
		break;

	case 14:
		printf("---请输入要导入的数独文件名:\n");
		scanf("%s", fileName);
		fpRead = fopen(fileName, "r");

		playDoubleSudoku(fpRead);

		fclose(fpRead);

		getchar(); getchar();
		break;

	case 15:
		printf("---请输入限制时间(s):\n");
		scanf("%d", &tem);
		timeLimit = tem * 1000;
		printf("时间修改为:%ds\n", timeLimit / 1000);
		getchar(); getchar();
		break;

	case 16:
		clockStoreClear();
		clockStoreStart();
		//clockStart();
		dpllInit(D);
		re = DPLL3(D);
		dpllEnd(D);
		//clockStop(1);
		clockStoreStop();
		spendTime = getClockStored();
		printf("---请输入该样例的编号:\n");
		scanf("%d", &tem);
		timeConsume[2][tem] = spendTime;
		system("cls");
		printf("---Dpll over\n");
		printf("---The result:\n");
		if (re == FALSE) {
			printf("无解\n");
			res = 0;
		}
		else if (re == 2) {
			printf("超出时间限制\n");
			res = 2;
		}
		else {
			printf("有解\n");
			res = re;
		}
		getchar(); getchar();
		break;

	case 17:

		printf("---请输入cnf样例名:\n");
		scanf("%s", fileName);
		fp1 = fopen(fileName, "r");
		printf("---请输入res名:\n");
		scanf("%s", fileName);
		fp2 = fopen(fileName, "r");

		if (D != NULL)deleteData(D), D = NULL;
		D = initByCnf(fp1);
		verifyDpllResult(D, fp2);

		fclose(fp1);
		fclose(fp2);
		getchar(); getchar();
		break;

	case 18:
		printf("---请输入保存的名字:\n");
		scanf("%s", fileName);
		fpWrite = fopen(fileName, "w");
		createDoubleSudokuUnique(fpWrite);
		fclose(fpWrite);

		printf("生成完毕\n");

		getchar(); getchar();
		break;


	default:
		printf("input error.");
		getchar(); getchar();
		break;
	}
}
	return 0;
}