#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "SAT_Resolver.h"
#include "BasicComponent.h"

void addClause(ClauseNode T, Data D) {
	if (D->firstC != NULL)
		T->nextC = D->firstC;
	D->firstC = T;
	D->clauseNum++;
}
void deleteLiteral(LiteralNode T, ClauseNode C) {
	if (T == NULL)return;
	if (T->preL == NULL) {
		C->firstL = T->nextL;
		if (T->nextL != NULL)
			T->nextL->preL = NULL;
	}
	else {
		if (T->nextL == NULL)
			T->preL->nextL = NULL;
		else {
			T->preL->nextL = T->nextL;
			T->nextL->preL = T->preL;
		}
	}
	free(T);
	C->nodeNum--;
}
void deleteClause(ClauseNode T, Data D) {
	if (T == NULL)return;
	while (T->firstL != NULL)deleteLiteral(T->firstL, T);

	if (T->preC == NULL) {
		D->firstC = T->nextC;
		if (T->nextC != NULL)
			T->nextC->preC = NULL;
	}
	else {
		if (T->nextC == NULL) 
			T->preC->nextC = NULL;
		else {
			T->preC->nextC = T->nextC;
			T->nextC->preC = T->preC;
		}
	}
	free(T);
	D->clauseNum--;
}
Data deleteData(Data D) {
	while (D->firstC != NULL)deleteClause(D->firstC, D);
	free(D);
	return NULL;
}
ClauseNode getClause() {
	ClauseNode p =(ClauseNode)malloc(sizeof(struct ClauseSAT));
	p->nextC = NULL; p->preC = NULL, p->nodeNum = 0;
	return p;
}
LiteralNode getLiteral() {
	LiteralNode p = (LiteralNode)malloc(sizeof(struct LiteralSAT));
	p->isRev = FALSE; p->nextL = NULL; p->preL = NULL; p->varIdx = 0;
	return p;
}
Data getData() {
	Data D = (Data)malloc(sizeof(struct DataSAT));
	D->firstC = NULL; D->clauseNum = D->varNum = 0;
	return D;
}
ClauseNode copyClause(ClauseNode T) {	//didn't copy the pointer of the clause,but set null.
	ClauseNode re = getClause();
	re->nodeNum = T->nodeNum;
	LiteralNode p = T->firstL;
	if (p == NULL) 
		re->firstL = NULL;
	else {
		LiteralNode tem = re->firstL = getLiteral();
		tem->isRev = p->isRev; tem->varIdx = p->varIdx;
		p = p->nextL;
		while (p != NULL) {
			tem->nextL = getLiteral();
			tem->nextL->preL = tem;
			tem = tem->nextL;
			tem->isRev = p->isRev; tem->varIdx = p->varIdx;
			p = p->nextL;
		}
	}
	return re;
}
Data copyAll(Data D) {
	if (D == NULL) return NULL;
	Data DC = getData();
	DC->clauseNum = D->clauseNum; DC->varNum = D->varNum;
	ClauseNode p = D->firstC;
	if(p == NULL)
		DC->firstC = NULL;
	else {
		ClauseNode tem = DC->firstC = copyClause(p);
		p = p->nextC;
		while (p != NULL) {
			tem->nextC = copyClause(p);
			p = p->nextC;
			tem->nextC->preC = tem;
			tem = tem->nextC;
		}
	}
	return DC;
}

void traverse(Data D, FILE* fp) {
	if (D == NULL)return;
	fprintf(fp, "p cnf %d %d\n", D->varNum, D->clauseNum);
	ClauseNode pC = D->firstC;
	LiteralNode pL = NULL;
	while (pC != NULL) {
		pL = pC->firstL;
		while (pL != NULL) {
			fprintf(fp, "%d ", pL->varIdx);
			pL = pL->nextL;
		}
		fprintf(fp, "0\n");
		pC = pC->nextC;
	}
}

ClauseNode* getStackC(int n) {
	return (ClauseNode*)malloc(sizeof(ClauseNode) * n);
}
LiteralNode* getStackL(int n) {
	return (LiteralNode*)malloc(sizeof(LiteralNode) * n);
}
int isSingleClause(ClauseNode T) {
	if (T->nodeNum == 1) {
		if (T->firstL == NULL || T->firstL->nextL != NULL)printf("Error in isSingleClause()");
		return TRUE;
	}
	return FALSE;
}
int isEmptyClause(ClauseNode T) {
	if (T->nodeNum == 0) {
		if (T->firstL != NULL)printf("Error in isEmptyClause()");
		return TRUE;
	}
	return FALSE;
}
int updateClause(ClauseNode T, int varIdx, int isTrue, Data D) {
	if (T == NULL)
		return FALSE;
	LiteralNode pL = NULL;
	LiteralNode tem = T->firstL;
	while (tem != NULL) {
		pL = tem;
		tem = tem->nextL;
		if (pL->varIdx == varIdx) {
			if (pL->isRev == isTrue) {
				deleteLiteral(pL, T);
			}
			else {
				deleteClause(T, D);
				return TRUE;
			}
		}
	}
	return FALSE;
}
int updateData(Data D, int varIdx, int isTrue) {
	ClauseNode pC = D->firstC;
	ClauseNode tem;
	while (pC != NULL) {
		tem = pC;
		pC = pC->nextC;
		if (!updateClause(tem, varIdx, isTrue, D)) {
			if (isEmptyClause(tem))
				return FALSE;
		}
	}
	return TRUE;
}

int updateClauseStack(ClauseNode T, char* stackRev, char* stackIdx, Data D) {
	if (T == NULL)return FALSE;
	LiteralNode pL = NULL;
	LiteralNode tem = T->firstL;
	while (tem != NULL) {
		pL = tem;
		tem = tem->nextL;
		if (stackIdx[pL->varIdx] == TRUE) {
			if (pL->isRev == (stackRev[pL->varIdx] ^ 1)) {
				deleteLiteral(pL, T);
			}
			else {
				deleteClause(T, D);
				return TRUE;
			}
		}
	}
	return FALSE;
}
int updateDataStack(Data D, char* stackRev, char* stackIdx) {
	ClauseNode pC = D->firstC;
	ClauseNode tem;
	while (pC != NULL) {
		tem = pC;
		pC = pC->nextC;
		if (!updateClauseStack(tem, stackRev, stackIdx, D)) {
			if (isEmptyClause(tem))
				return FALSE;
		}
	}
	return TRUE;
}

int* dis(int* a, int* b, int n) {
	for (int i = 1; i <= n; i++)
		if (a[i] == 0)a[i] = b[i];
	free(b);
	return a;
}

char* stackRev;
char* stackIdx;
char* pureIsCheck;
char* pureRev;
char* pureNoStay;
float* score;

void dpllInit(Data D) {
	stackRev = (char*)malloc(sizeof(char) * (D->varNum + 1));
	stackIdx = (char*)malloc(sizeof(char) * (D->varNum + 1));
	pureIsCheck = (char*)malloc(sizeof(char) * (D->varNum + 1));
	pureRev = (char*)malloc(sizeof(char) * (D->varNum + 1));
	pureNoStay = (char*)malloc(sizeof(char) * (D->varNum + 1));
	score = (float*)malloc(sizeof(float) * (D->varNum + 1));
}
void dpllEnd(Data D) {
	free(stackRev);
	free(stackIdx);
	free(pureIsCheck);
	free(pureRev);
	free(pureNoStay);
}

int* DPLL(Data D) {
	if (timeOut()) {
		return 2;
	}
	int* re = (int*)malloc(sizeof(int) * (D->varNum + 1));

	for (int i = 0; i <= D->varNum; i++)re[i] = FALSE;
	
	/*{
		
		for (int i = 0; i <= D->varNum; i++)stackRev[i] = stackIdx[i] = pureIsCheck[i] = pureNoStay[i] = pureRev[i] = FALSE;

		ClauseNode pC = D->firstC;
		LiteralNode pL = NULL;
		int idx, rev;
		while (pC != NULL) {
			pL = pC->firstL;
			while (pL != NULL) {
				idx = pL->varIdx, rev = pL->isRev;
				if (pureNoStay[idx] == FALSE) {
					if (pureIsCheck[idx] == FALSE) {
						pureIsCheck[idx] = TRUE;
						pureRev[idx] = rev;
						stackIdx[idx] = TRUE;
						re[idx] = rev ? -1 : 1;
						stackRev[idx] = rev;
					}
					else {
						if (pureRev[idx] != rev) {
							pureNoStay[idx] = TRUE;
							re[idx] = stackIdx[idx] = FALSE;
							stackRev[idx] = FALSE;
						}
					}
				}
				pL = pL->nextL;
			}
			pC = pC->nextC;
		}
	}*/

	//clockStoreStop();

	int flag = TRUE;
	int idx, rev;
	while (flag) {
		flag = FALSE;
		ClauseNode pC = D->firstC;
		LiteralNode pL = NULL;
		
		for (int i = 0; i <= D->varNum; i++) stackRev[i] = stackIdx[i] = FALSE;

		while (pC != NULL) {	//0.10
			if (isSingleClause(pC)) {
				idx = pC->firstL->varIdx, rev = pC->firstL->isRev;
				if (stackIdx[idx] == TRUE) {
					if (stackRev[idx] != rev) {
						free(re);
						return FALSE;
					}
				}
				else {
					stackIdx[idx] = TRUE;
					re[idx] = rev ? -1 : 1;
					stackRev[idx] = rev;
					flag = TRUE;
				}
			}
			else if (isEmptyClause(pC)) {
				free(re);
				return FALSE;
			}
			pC = pC->nextC;
		}

		if (updateDataStack(D, stackRev, stackIdx) == FALSE) {	//0.20
			free(re);
			return FALSE;
		}
	}

	int varIdx;	//0.025
	{
		int minLen = 1 << 25;
		{
			ClauseNode pC = D->firstC;
			while (pC != NULL) {
				if (pC->nodeNum < minLen)minLen = pC->nodeNum;
				pC = pC->nextC;
			}
		}
		for (int i = 0; i < D->varNum; i++)score[i] = 0;

		ClauseNode pC = D->firstC;

		float max = 0;
		pC = D->firstC;
		LiteralNode pL;
		int idx, len;
		while (pC != NULL) {
			pL = pC->firstL;
			len = pC->nodeNum;
			while (pL != NULL) {
				idx = pL->varIdx;
				score[idx] += pow(0.5, len);
				if (score[idx] > max) {
					max = score[idx];
					varIdx = idx;
				}
				pL = pL->nextL;
			}
			pC = pC->nextC;
		}
	}
		
	int* reTem;
	
	if (D->firstC != NULL) {

		int flag = rand() > (RAND_MAX / 2);

		Data DC = copyAll(D);	//0.57

		if (updateData(D, varIdx, flag) == FALSE) {	//0.05
			free(re);
			deleteData(DC);
			return FALSE;
		}
		reTem = DPLL(D);
		if (reTem == 2) { 
			deleteData(DC);
			return 2; 
		}
		if (reTem != FALSE) {
			deleteData(DC);
			re[varIdx] = flag ? 1 : -1;
			return dis(re, reTem, D->varNum);
		}
		else {
			if (updateData(DC, varIdx, flag ^ 1) == FALSE) {
				free(re);
				deleteData(DC);
				return FALSE;
			}
			reTem = DPLL(DC);
			if (reTem == 2) {
				deleteData(DC);
				return 2;
			}
			if (reTem != NULL) {
				deleteData(DC);
				re[varIdx] = flag ^ 1 ? 1 : -1;
				return dis(re, reTem, D->varNum);
			}
			else {
				free(re);
				deleteData(DC);
				return FALSE;
			}
		}
	}
	else {
		return re;
	}
}



int* BtDPLL(Data D) {
	if (timeOut()) {
		return 2;
	}
	int* re = (int*)malloc(sizeof(int) * (D->varNum + 1));

	for (int i = 0; i <= D->varNum; i++)re[i] = FALSE;

	/*{

		for (int i = 0; i <= D->varNum; i++)stackRev[i] = stackIdx[i] = pureIsCheck[i] = pureNoStay[i] = pureRev[i] = FALSE;

		ClauseNode pC = D->firstC;
		LiteralNode pL = NULL;
		int idx, rev;
		while (pC != NULL) {
			pL = pC->firstL;
			while (pL != NULL) {
				idx = pL->varIdx, rev = pL->isRev;
				if (pureNoStay[idx] == FALSE) {
					if (pureIsCheck[idx] == FALSE) {
						pureIsCheck[idx] = TRUE;
						pureRev[idx] = rev;
						stackIdx[idx] = TRUE;
						re[idx] = rev ? -1 : 1;
						stackRev[idx] = rev;
					}
					else {
						if (pureRev[idx] != rev) {
							pureNoStay[idx] = TRUE;
							re[idx] = stackIdx[idx] = FALSE;
							stackRev[idx] = FALSE;
						}
					}
				}
				pL = pL->nextL;
			}
			pC = pC->nextC;
		}
	}*/

	//clockStoreStop();

	int flag = TRUE;
	int idx, rev;
	while (flag) {
		flag = FALSE;
		ClauseNode pC = D->firstC;
		LiteralNode pL = NULL;

		for (int i = 0; i <= D->varNum; i++) stackRev[i] = stackIdx[i] = FALSE;

		while (pC != NULL) {	//0.10
			if (isSingleClause(pC)) {
				idx = pC->firstL->varIdx, rev = pC->firstL->isRev;
				if (stackIdx[idx] == TRUE) {
					if (stackRev[idx] != rev) {
						free(re);
						return FALSE;
					}
				}
				else {
					stackIdx[idx] = TRUE;
					re[idx] = rev ? -1 : 1;
					stackRev[idx] = rev;
					flag = TRUE;
				}
			}
			else if (isEmptyClause(pC)) {
				free(re);
				return FALSE;
			}
			pC = pC->nextC;
		}

		if (updateDataStack(D, stackRev, stackIdx) == FALSE) {	//0.20
			free(re);
			return FALSE;
		}
	}
	int varIdx;
	if (D->firstC != NULL) varIdx = D->firstC->firstL->varIdx;	//0.025
	/*{
		int minLen = 1 << 25;
		{
			ClauseNode pC = D->firstC;
			while (pC != NULL) {
				if (pC->nodeNum < minLen)minLen = pC->nodeNum;
				pC = pC->nextC;
			}
		}
		for (int i = 0; i < D->varNum; i++)score[i] = 0;

		ClauseNode pC = D->firstC;

		float max = 0;
		pC = D->firstC;
		LiteralNode pL;
		int idx, len;
		while (pC != NULL) {
			pL = pC->firstL;
			len = pC->nodeNum;
			while (pL != NULL) {
				idx = pL->varIdx;
				score[idx] += pow(0.5, len);
				if (score[idx] > max) {
					max = score[idx];
					varIdx = idx;
				}
				pL = pL->nextL;
			}
			pC = pC->nextC;
		}
	}*/

	int* reTem;

	if (D->firstC != NULL) {

		int flag = rand() > (RAND_MAX / 2);

		Data DC = copyAll(D);	//0.57

		if (updateData(D, varIdx, flag) == FALSE) {	//0.05
			free(re);
			deleteData(DC);
			return FALSE;
		}
		reTem = BtDPLL(D);
		if (reTem == 2) {
			deleteData(DC);
			return 2;
		}
		if (reTem != FALSE) {
			deleteData(DC);
			re[varIdx] = flag ? 1 : -1;
			return dis(re, reTem, D->varNum);
		}
		else {
			if (updateData(DC, varIdx, flag ^ 1) == FALSE) {
				free(re);
				deleteData(DC);
				return FALSE;
			}
			reTem = BtDPLL(DC);
			if (reTem == 2) {
				deleteData(DC);
				return 2;
			}
			if (reTem != NULL) {
				deleteData(DC);
				re[varIdx] = flag ^ 1 ? 1 : -1;
				return dis(re, reTem, D->varNum);
			}
			else {
				free(re);
				deleteData(DC);
				return FALSE;
			}
		}
	}
	else {
		return re;
	}
}

int* DPLL3(Data D) {
	if (timeOut()) {
		return 2;
	}
	int* re = (int*)malloc(sizeof(int) * (D->varNum + 1));

	for (int i = 0; i <= D->varNum; i++)re[i] = FALSE;

	/*{

		for (int i = 0; i <= D->varNum; i++)stackRev[i] = stackIdx[i] = pureIsCheck[i] = pureNoStay[i] = pureRev[i] = FALSE;

		ClauseNode pC = D->firstC;
		LiteralNode pL = NULL;
		int idx, rev;
		while (pC != NULL) {
			pL = pC->firstL;
			while (pL != NULL) {
				idx = pL->varIdx, rev = pL->isRev;
				if (pureNoStay[idx] == FALSE) {
					if (pureIsCheck[idx] == FALSE) {
						pureIsCheck[idx] = TRUE;
						pureRev[idx] = rev;
						stackIdx[idx] = TRUE;
						re[idx] = rev ? -1 : 1;
						stackRev[idx] = rev;
					}
					else {
						if (pureRev[idx] != rev) {
							pureNoStay[idx] = TRUE;
							re[idx] = stackIdx[idx] = FALSE;
							stackRev[idx] = FALSE;
						}
					}
				}
				pL = pL->nextL;
			}
			pC = pC->nextC;
		}
	}*/

	//clockStoreStop();

	int flag = TRUE;
	int idx, rev;
	while (flag) {
		flag = FALSE;
		ClauseNode pC = D->firstC;
		LiteralNode pL = NULL;

		for (int i = 0; i <= D->varNum; i++) stackRev[i] = stackIdx[i] = FALSE;

		while (pC != NULL) {	//0.10
			if (isSingleClause(pC)) {
				idx = pC->firstL->varIdx, rev = pC->firstL->isRev;
				if (stackIdx[idx] == TRUE) {
					if (stackRev[idx] != rev) {
						free(re);
						return FALSE;
					}
				}
				else {
					stackIdx[idx] = TRUE;
					re[idx] = rev ? -1 : 1;
					stackRev[idx] = rev;
					flag = TRUE;
				}
			}
			else if (isEmptyClause(pC)) {
				free(re);
				return FALSE;
			}
			pC = pC->nextC;
		}

		if (updateDataStack(D, stackRev, stackIdx) == FALSE) {	//0.20
			free(re);
			return FALSE;
		}
	}

	int varIdx;	//0.025
	{
		int minLen = 1 << 25;
		{
			ClauseNode pC = D->firstC;
			while (pC != NULL) {
				if (pC->nodeNum < minLen)minLen = pC->nodeNum;
				pC = pC->nextC;
			}
		}
		for (int i = 0; i < D->varNum; i++)score[i] = 0;

		ClauseNode pC = D->firstC;

		float max = 0;
		pC = D->firstC;
		LiteralNode pL;
		int idx, len;
		while (pC != NULL) {
			pL = pC->firstL;
			len = pC->nodeNum;
			while (pL != NULL) {
				idx = pL->varIdx;
				if (pL->isRev)score[idx] += 10000;
				else score[idx] += 1;
				int scoreInt = (int)(score[idx] + 0.01);
				if (scoreInt % 10000 > max) {
					max = scoreInt % 10000;
					varIdx = idx;
				}
				if(scoreInt / 10000 > max) {
					max = scoreInt / 10000;
					varIdx = idx;
				}
				pL = pL->nextL;
			}
			pC = pC->nextC;
		}
	}

	int* reTem;

	if (D->firstC != NULL) {

		int flag = rand() > (RAND_MAX / 2);

		Data DC = copyAll(D);	//0.57

		if (updateData(D, varIdx, flag) == FALSE) {	//0.05
			free(re);
			deleteData(DC);
			return FALSE;
		}
		reTem = DPLL(D);
		if (reTem == 2) {
			deleteData(DC);
			return 2;
		}
		if (reTem != FALSE) {
			deleteData(DC);
			re[varIdx] = flag ? 1 : -1;
			return dis(re, reTem, D->varNum);
		}
		else {
			if (updateData(DC, varIdx, flag ^ 1) == FALSE) {
				free(re);
				deleteData(DC);
				return FALSE;
			}
			reTem = DPLL(DC);
			if (reTem == 2) {
				deleteData(DC);
				return 2;
			}
			if (reTem != NULL) {
				deleteData(DC);
				re[varIdx] = flag ^ 1 ? 1 : -1;
				return dis(re, reTem, D->varNum);
			}
			else {
				free(re);
				deleteData(DC);
				return FALSE;
			}
		}
	}
	else {
		return re;
	}
}

int* startDpll(Data D) {
	dpllInit(D);
	int* re = DPLL(D);
	dpllEnd(D);
	return re;
}

void outputErrorClause(ClauseNode pC, int number) {
	printf("UnsatClause%d: ", number);
	for (LiteralNode pL = pC->firstL; pL != NULL; pL = pL->nextL) {
		printf("%d ", pL->varIdx * (pL->isRev ? -1 : 1));
	}
	printf("\n");
}
int* verifyDpllResult(Data D, FILE* fpRead) {
	int tem;
	fgetc(fpRead);
	fscanf(fpRead, "%d", &tem);
	fgetc(fpRead); fgetc(fpRead);
	if (tem == -1) {
		printf("Result: ³¬Ê±\n");
	}
	else if (tem == 0) {
		printf("Result: ²»Âú×ãÑùÀý\n");
	}
	else {
		int assign[10000];
		int tag = 1, flag = 0;
		for (int i = 0; i < 10000; i++)assign[i] = 0;
		for (int i = 0; i < D->varNum; i++) {
			fscanf(fpRead, "%d", &tem);
			if (tem > 0)tag = 1; else tag = -1;
			assign[tem * tag] = tag;
			printf("%d\n", tem);
		}
		int i = 1;
		printf("\n\n");
		tag = 0;
		for (ClauseNode pC = D->firstC; pC != NULL; pC = pC->nextC, i++) {
			flag = 0;
			for (LiteralNode pL = pC->firstL; pL != NULL; pL = pL->nextL) {
				if (pL->isRev == (assign[pL->varIdx] == -1)) { tag = flag = 1; break; }
			}
			if (flag == 0) {
				outputErrorClause(pC, i);
			}
		}
		if (tag == 0)printf("\n\nResult: Unsatisified\n");
		else printf("\n\nResult: Satisified\n");
	}
}