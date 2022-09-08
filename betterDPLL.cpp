//#include <stdlib.h>
//#include <stdio.h>
//
//typedef struct StackNp {
//	int* stack;
//	int p, cap;	//p point to the empty place after the last element.
//
//	void clear() {
//		p = 0;
//	}
//	int top() {
//		return stack[p - 1];
//	}
//	bool pop() {
//		if (p == 0)return false;
//		p--;
//		return true;
//	}
//	void push(int x) {
//		if (p >= cap) {
//			int* temStack;
//			if (cap < 100) {
//				cap = cap * 2;
//				temStack = (int*)malloc(sizeof(int) * cap);
//
//			}
//			else {
//				cap = int(cap * 1.25);
//				temStack = (int*)malloc(sizeof(int) * cap);
//			}
//			for (int i = 0; i < p; i++)	temStack[i] = stack[i];
//			int* tem = stack;
//			stack = temStack;
//			free(tem);
//		}
//		stack[p++] = x;
//	}
//} *Stack;
//typedef struct VectorNp {
//	int* vector;
//	int cap, len;
//
//	bool isEmpty() { return len == 0; }
//	void clear() {
//		len = 0;
//	}
//	int size() {
//		return len;
//	}
//	int capicity() {
//		return cap;
//	}
//	void push(int x) {
//		if (len >= cap) {
//			int* temVector;
//			if (cap < 100) {
//				cap = cap * 2;
//				temVector = (int*)malloc(sizeof(int) * cap);
//			}
//			else {
//				cap = int(cap * 1.25);
//				temVector = (int*)malloc(sizeof(int) * cap);
//			}
//			for (int i = 0; i < len; i++)	temVector[i] = vector[i];
//			int* tem = vector;
//			vector = temVector;
//			free(tem);
//		}
//		vector[len++] = x;
//	}
//} *Vector;
//
//typedef struct BtLearnLiteralNp {
//	BtLearnLiteralNp* next;
//	int varIdx, isRev;
//} *BtLearnLiteral;
//typedef struct BtLearnClauseNp {
//	BtLearnClauseNp* next;
//	BtLearnLiteral first;
//} *BtLearnClause;
//
//typedef struct BtLiteralNp {
//	BtLiteralNp* next;
//	BtLiteralNp* pre;
//	int varIdx, isRev;
//} *BtLiteral;
//typedef struct BtClauseNp {
//	BtClauseNp* next;
//	BtClauseNp* pre;
//	BtLiteral first;
//	int idx;
//} *BtClause;
//
//
//typedef struct BtTrialNp {
//	Vector trail;
//
//	BtLearnClause handleConflict() {
//
//	}
//
//} *BtTrial;
//typedef struct BackNp {
//	BtLiteral pL;
//
//} *Back;
//
//typedef struct BtDataNp {
//	BtClause first;
//	BtClause last;
//	BtLearnClause learnFirst;
//	int varNum; int level;
//	BtTrial trail;
//} *BtData;
//
//typedef struct BtOriginDataNp {
//	BtClause* clause;
//	BtLearnClause learnFirst;
//	int varNum;
//	BtTrial trail;
//} *BtOriginData;
//
//BtData OriginData;
//int* globalArray; int globalArrayCap;
//
//void clearGlobalArray(int n = globalArrayCap) {
//	for (int i = 0; i < n; i++)globalArray[i] = 0;
//}
//int abs(int x) {
//	if (x > 0)return x;
//	return -x;
//}
//
//Stack getStack() {
//	Stack stack = (Stack)malloc(sizeof(StackNp));
//	stack->cap = 10;
//	stack->p = 0;
//	stack->stack = (int*)malloc(sizeof(int) * stack->cap);
//	return stack;
//}
//Vector getVector() {
//	Vector vector = (Vector)malloc(sizeof(VectorNp));
//	vector->cap = 10;
//	vector->len = 0;
//	vector->vector = (int*)malloc(sizeof(int) * vector->cap);
//	return vector;
//}
//void deleteVector(Vector v) {
//	free(v->vector);
//	free(v);
//}
//void deleteStack(Stack v) {
//	free(v->stack);
//	free(v);
//}
//
//BtData getData() {
//	//BtData D = (BtData)malloc(sizeof(BtDataNp));
//	//D->first = NULL; D->last = NULL;  D->learnFirst = NULL; D->trail = NULL;
//	//D->varNum = 0;
//	//return D;
//}
//BtClause getClause() {
//	//BtClause pC = (BtClause)malloc(sizeof(BtClauseNp));
//	//pC->first = NULL; pC->next = NULL;
//	//return pC;
//}
//BtLiteral getLiteral() {
//	//BtLiteral pL = (BtLiteral)malloc(sizeof(BtLiteralNp));
//	//pL->next = NULL; pL->isRev = false; pL->varIdx = 0;
//	//return pL;
//}
//BtTrial getTrail();
//
//bool isSingleClause(BtClause pC) {
//	if (pC == NULL) { printf("\n------error in isSingleClause---------\n"); return false; }
//	if (pC->first != NULL && pC->first->next != NULL)return true;
//	return false;
//}
//bool isEmptyClause(BtClause pC) {
//	if (pC == NULL) { printf("\n------error in isEmptyClause---------\n"); return false; }
//	if (pC->first == NULL)return true;
//	return false;
//}
//bool deleteClause(BtClause pC, BtData D);
//bool deleteLiteral(BtLiteral pL, BtData D);
//BtData copyData(BtData D);
//
//void dealSingleClause(BtData D) {	// TODO: return�Ĵ����֡�
//	BtClause pC = D->first; BtLiteral pL;
//	int idx; bool isRev, flag = false;
//	clearGlobalArray(D->varNum + 1);
//	do {
//		clearGlobalArray(D->varNum + 1);
//		while (pC != NULL) {
//			if (isSingleClause(pC)) {
//				flag = true;
//				idx = pC->first->varIdx, isRev = pC->first->isRev;
//				globalArray[idx] = isRev + 1;
//				// TODO: �����Ƿ��Լ���ֵ���෴ֵ�����У�������ͻ�����ڡ�
//				// TODO: trail����ӡ�ͬʱ��trail���D��level
//				deleteClause(pC, D);
//			}
//			pC = pC->next;
//		}
//		pC = D->first;
//		bool status = false;
//		while (pC != NULL) {
//			pL = pC->first;
//			while (pL != NULL) {
//				if (globalArray[pL->varIdx]) {
//					if (globalArray[pL->varIdx] == isRev + 1) {
//						auto tem = pC;
//						pC = pC->next;
//						status = deleteClause(tem, D);	//TODO: �����Ƿ�ɾ���ɹ���Ĵ������
//						break;
//					}
//					else {
//						auto tem = pL;
//						pL = pL->next;
//						status = deleteLiteral(tem, D);	//TODO: �����Ƿ�ɾ���ɹ���Ĵ������������ɾ�������Ӿ��ĳ�ͻ�����֡�
//						continue;
//					}
//				}
//				pL = pL->next;
//			}
//			if(!status)pC = pC->next;
//		}
//	} while (flag);
//	
//}
//int decision(BtData D);	
//void update(BtData D, int dec); /*{
//	int idx = abs(dec); bool isRev = dec < 0;
//	
//}*/
//
////ѧϰ������ݽṹѡ������(���ڿ��ܵ�ɾ��)
//Back btDPLL(BtData D) {
//	// ���Ƚ��е��Ӿ��飬�����ͬʱ��¼���������ֳ�ͻ����ͨ����"������"����Data��Trailָ���������ؽ��(ע��ӿ����滯���Ա�
//	// �������������������鷵��ѧϰ�����Ӿ��ָ��)�����л��ݡ�
//
//	D->level++;
//
//	dealSingleClause(D);	// TODO: �����ز��֣�������Ҫ���ݣ�����UNSAT��
//	// TODO: satisify�Ĵ���
//
//	// Ȼ����о��ߣ�����dpll�ķ���ֵ��������ֵisSolved==true����˵�����ص��ǺϷ�����˳�Ʒ��ؼ��ɡ�����˵�����ص���ѧϰ���Ӿ��ָ��
//	// ��Ҫ��֮���ص�Ŀ����ݲ�(ָѧϰ�����Ӿ�ĵڶ���ľ��߲�)�����Լ�ΪĿ����ݲ㣬�����ѧϰ����ָ�룬��ӵ�ѧϰ���У�������ѡ��
//	// ����ֵ��
//
//	int reDec = decision(D);
//	int decidedIdx = abs(reDec), decidedIsRev = reDec < 0;
//
//	update(D, reDec);
//	Back reDPLL = btDPLL(D);
//	// TODO: ����reDPLL���ж�����Ҫ���ݻ�����Ҫ���㴦�������ײ��ͻUNSAT
//} 