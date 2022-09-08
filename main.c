#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <Windows.h>

#include "SAT_Resolver.h"
#include "BasicComponent.h"

//copyһ�δ�ź�ʱ1.0~1.2ms deleteһ�δ��2.0~2.3ms (������ܲ��Լ�ais10.cnf)

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
		printf("---��ѡ���������ֵĸ�ʽ\n");
		printf("1: ����.cnf\n2: �ļ�����.../����.cnf\n");
		scanf("%d", &tem);
		if (tem >= 1 && tem <= 2) {
			printf("---������:\n");
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
			printf("ѡ�����\n");
		}
		if (flag) {
			if (fpRead == NULL) {
				printf("�ļ���ʧ��\n");
			}
			else {
				printf("�ļ��򿪳ɹ�\n");
				flag = 2;
			}
		}
		if (flag == 2) {
			if (D != NULL) {
				deleteData(D);
			}
			D = initByCnf(fpRead);
			if (D != NULL)printf("�ļ������ɹ�\n");
			else printf("�ļ�����ʧ��\n");
		}
		fclose(fpRead);
		getchar(); getchar();
		break;


	case 2:
		printf("---��ѡ�������ʽ(1: ��Ļ 2: �ļ�):\n");
		scanf("%d", &tem);
		if (tem == 1) {
			outputParsedCnfIntoCnf(D);
		}
		else if (tem == 2) {
			printf("---������Ҫ������ļ���:\n");
			scanf("%s", fileName);
			fpWrite = fopen(fileName, "w");
			turnDataIntoCnf(D, fpWrite);
			fclose(fpWrite);
			printf("����ɹ�\n");
		}
		else {
			printf("�������\n");
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
		printf("---������������ı��:\n");
		scanf("%d", &tem);
		timeConsume[0][tem] = spendTime;
		system("cls");
		printf("---Dpll over\n");
		printf("---The result:\n");
		if (re == FALSE) {
			printf("�޽�\n");
			res = 0;
		}
		else if (re == 2) {
			printf("����ʱ������\n");
			res = 2;
		}
		else {
			printf("�н�\n");
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
		printf("---�����뱣�������(����.cnf):\n");
		scanf("%d", fileName);
		fpWrite = openPath("cnf", fileName, "w");
		turnDataIntoCnf(D, fpWrite);
		printf("����ɹ�\n");
		fclose(fpWrite);
		getchar(); getchar();
		break;

	case 6:
		if (D == NULL) {
			printf("�����\n");
		}
		else {
			deleteData(D);
			D = NULL;
			printf("����ɹ�\n");
		}
		getchar(); getchar();
		break;

	case 7:
		printf("---����������:\n");
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
		printf("---�����뱣�������:\n");
		scanf("%s", fileName);
		fpWrite = fopen(fileName, "w");
		outputResult(res, D->varNum, fpWrite, spendTime);
		fclose(fpWrite);
		fpWrite = NULL;
		printf("����ɹ�\n");
		getchar(); getchar();
		break;

	case 9:
		printf("---�����뱣�������:\n");
		scanf("%s", fileName);
		fpWrite = fopen(fileName, "w");
		while (!createSingleSudoku(24, fpWrite)) {
			fclose(fpWrite);
			fpWrite = fopen(fileName, "w");
		};
		fclose(fpWrite);

		printf("�������\n");

		getchar(); getchar();
		break;

	case 10:
		printf("---�����뱣�������:\n");
		scanf("%s", fileName);
		fpWrite = fopen(fileName, "w");
		while (!createDoubleSudoku(20, fpWrite)) {
			fclose(fpWrite);
			fpWrite = fopen(fileName, "w");
		};
		fclose(fpWrite);

		printf("�������\n");

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
		printf("---������������ı��:\n");
		scanf("%d", &tem);
		timeConsume[1][tem] = spendTime;
		system("cls");
		printf("---Dpll over\n");
		printf("---The result:\n");
		if (re == FALSE) {
			printf("�޽�\n");
			res = 0;
		}
		else if (re == 2) {
			printf("����ʱ������\n");
			res = 2;
		}
		else {
			printf("�н�\n");
			res = re;
		}
		getchar(); getchar();
		break;

	case 12:
		printf("---��������Ҫ�Ƚϵ��ļ����:\n");
		scanf("%d", &tem);
		printf("---��������Ҫ�Ƚϵ�dpll(���Ϊδ�Ż���):\n");
		scanf("%d%d", &x, &y);
		x--, y--;
		if (!(tem > 0 && tem < 20))printf("�������\n");
		else {
			if (timeConsume[x][tem] == -1 || timeConsume[y][tem] == -1)printf("δ�Ը��ļ�������һ��dpll\n");
			else printf("�Ż���Ϊ:%f%%\n", (float)(timeConsume[x][tem] - timeConsume[y][tem]) / (timeConsume[x][tem] + 0.01) * 100);
		}
		getchar(); getchar();
		break;

	case 13:
		printf("---������Ҫ����������ļ���:\n");
		scanf("%s", fileName);
		fpRead = fopen(fileName, "r");
		
		playSingleSudoku(fpRead);

		fclose(fpRead);

		getchar(); getchar();
		break;

	case 14:
		printf("---������Ҫ����������ļ���:\n");
		scanf("%s", fileName);
		fpRead = fopen(fileName, "r");

		playDoubleSudoku(fpRead);

		fclose(fpRead);

		getchar(); getchar();
		break;

	case 15:
		printf("---����������ʱ��(s):\n");
		scanf("%d", &tem);
		timeLimit = tem * 1000;
		printf("ʱ���޸�Ϊ:%ds\n", timeLimit / 1000);
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
		printf("---������������ı��:\n");
		scanf("%d", &tem);
		timeConsume[2][tem] = spendTime;
		system("cls");
		printf("---Dpll over\n");
		printf("---The result:\n");
		if (re == FALSE) {
			printf("�޽�\n");
			res = 0;
		}
		else if (re == 2) {
			printf("����ʱ������\n");
			res = 2;
		}
		else {
			printf("�н�\n");
			res = re;
		}
		getchar(); getchar();
		break;

	case 17:

		printf("---������cnf������:\n");
		scanf("%s", fileName);
		fp1 = fopen(fileName, "r");
		printf("---������res��:\n");
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
		printf("---�����뱣�������:\n");
		scanf("%s", fileName);
		fpWrite = fopen(fileName, "w");
		createDoubleSudokuUnique(fpWrite);
		fclose(fpWrite);

		printf("�������\n");

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