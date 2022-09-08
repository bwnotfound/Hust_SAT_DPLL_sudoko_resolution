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
//void dealSingleClause(BtData D) {	// TODO: return的处理部分。
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
//				// TODO: 检验是否以及赋值了相反值，若有，则进入冲突处理环节。
//				// TODO: trail的添加。同时向trail添加D的level
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
//						status = deleteClause(tem, D);	//TODO: 检验是否删除成功后的处理操作
//						break;
//					}
//					else {
//						auto tem = pL;
//						pL = pL->next;
//						status = deleteLiteral(tem, D);	//TODO: 检验是否删除成功后的处理操作，包括删除出空子句后的冲突处理部分。
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
////学习域的数据结构选择链表(便于可能的删除)
//Back btDPLL(BtData D) {
//	// 首先进行单子句检查，处理的同时记录迹。若出现冲突，则通过向"处理函数"传递Data和Trail指针解决并返回结果(注意接口正规化，以便
//	// 便于升级处理函数。建议返回学习到的子句的指针)，进行回溯。
//
//	D->level++;
//
//	dealSingleClause(D);	// TODO: 处理返回部分，可能需要回溯，或者UNSAT。
//	// TODO: satisify的处理
//
//	// 然后进行决策，接收dpll的返回值。若返回值isSolved==true，则说明返回的是合法迹，顺势返回即可。否则说明返回的是学习的子句的指针
//	// 需要将之返回到目标回溯层(指学习到的子句的第二深的决策层)。若自己为目标回溯层，则处理该学习到的指针，添加到学习域中，并依此选择
//	// 文字值。
//
//	int reDec = decision(D);
//	int decidedIdx = abs(reDec), decidedIsRev = reDec < 0;
//
//	update(D, reDec);
//	Back reDPLL = btDPLL(D);
//	// TODO: 处理reDPLL，判断是需要回溯还是需要本层处理还是因首层冲突UNSAT
//} 