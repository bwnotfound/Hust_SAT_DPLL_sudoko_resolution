#include <stdio.h>
#include <string.h>
#include "SAT_Resolver.h"

FILE* openPath(char file[], char name[], char method[]) {
	char s[30];
	strcpy(s, file);
	strcat(s, "/");
	strcat(s, name);
	return fopen(s, method);
}

FILE* openCnf(char name[], char method[]) {
	char s[30];
	strcpy(s, "cnf/");
	strcat(s, name);
	return fopen(s, method);
}

void turnDataIntoCnf(Data D, FILE* fpWrite) {
	fprintf(fpWrite, "p cnf %d %d\n", D->varNum, D->clauseNum);
	ClauseNode pC = D->firstC;
	LiteralNode pL;
	while (pC != NULL) {
		pL = pC->firstL;
		while (pL != NULL) {
			fprintf(fpWrite, "%d ", pL->varIdx * (pL->isRev ? -1 : 1));
			pL = pL->nextL;
		}
		fprintf(fpWrite, "0\n");
		pC = pC->nextC;
	}
}

ClauseNode readLine(FILE* fpRead) {
	int in;
	int fre = fscanf(fpRead, "%d", &in);
	if (fre == 0 || fre == EOF)return NULL;
	if (in == 0) { 
		printf("Something wrong in readline(); Maybe the cnf file contains a empty clause."); 
		return NULL; 
	}	//Should never be triggered
	ClauseNode re = getClause();
	re->firstL = getLiteral();
	LiteralNode p = re->firstL;
	if (in < 0)p->isRev = TRUE, p->varIdx = in * -1;
	else p->varIdx = in;
	re->nodeNum++;
	fscanf(fpRead, "%d", &in);
	while (in != 0) {
		p->nextL = getLiteral();
		p->nextL->preL = p;
		p = p->nextL;
		if (in < 0)p->isRev = TRUE, p->varIdx = in * -1;
		else p->varIdx = in;
		fscanf(fpRead, "%d", &in);
		re->nodeNum++;
	}
	return re;
}

Data initByCnf(FILE* fpRead) {	//NULL represent unsat, because of the empty clause.
	char ch;
	while ((ch = fgetc(fpRead)) == 'c')while ((ch = fgetc(fpRead)) != '\n');
	Data D = getData();

	int status = FALSE;
	while ((ch = fgetc(fpRead)) != 'c');
	if (fgetc(fpRead) == 'n' && fgetc(fpRead) == 'f')status = TRUE;

	if (status) {
		if (fscanf(fpRead, "%d%d", &D->varNum, &D->clauseNum) == EOF)printf("Something went wrong in initByCnf();");
		D->firstC = readLine(fpRead);
		ClauseNode p = D->firstC, tem;
		while (p != NULL) {
			tem = readLine(fpRead);
			p->nextC = tem;
			if (tem == NULL)break;
			p->nextC->preC = p;
			p = p->nextC;
		}
	}
	else printf("TODO");


	return D;
}

void outputParsedCnfIntoCnf(Data D) {
	printf("p cnf %d %d\n", D->varNum, D->clauseNum);
	ClauseNode pC = D->firstC;
	LiteralNode pL;
	while (pC != NULL) {
		pL = pC->firstL;
		while (pL != NULL) {
			printf("%d ", pL->varIdx * (pL->isRev ? -1 : 1));
			pL = pL->nextL;
		}
		printf( "0\n");
		pC = pC->nextC;
	}
}

void outputDataIntoCnf(Data D, int* res){
	int varNum = D->varNum;
	for (int i = 0; i <= varNum / 9; i++) {
		for (int j = 0; i  * 9 + j < varNum && j < 9; j++) {
			if (j)printf(" | ");
			int p = i * 9 + j + 1;
			printf("%d: %d", p, res[p]);
		}
		printf("\n");
	}
}

void outputResult(int* res, int len, FILE* fpWrite, int time) {
	if (res == 0) {
		fprintf(fpWrite, "s 0\n");
	}
	else if (res == 2) {
		fprintf(fpWrite, "s -1\n");
	}
	else {
		fprintf(fpWrite, "s 1\n");
		fprintf(fpWrite, "v ");
		for (int i = 1; i <= len; i++)
			fprintf(fpWrite, "%d ", (res[i] >= 0 ? 1 : -1) * i);
		fprintf(fpWrite, "\nt %d\n", time);
	}
}

