#define TRUE 1
#define FALSE 0

typedef struct LiteralSAT {
	int varIdx, isRev;
	struct LiteralSAT* preL, * nextL;
} *LiteralNode;

typedef struct ClauseSAT {
	int nodeNum;
	struct ClauseSAT* preC, * nextC;
	LiteralNode firstL;
} *ClauseNode;

typedef struct DataSAT{
	int clauseNum, varNum;	//clauseNum is not used yet.
	ClauseNode firstC;
} *Data;

Data initByCnf(FILE* fp);
void traverse(Data D, FILE* fp);
int* DPLL(Data D, FILE* fp);
void addClause(ClauseNode T, Data D);
LiteralNode getLiteral();
ClauseNode getClause();

void initCnf(FILE* fpCsv, char* path, char* pathTem);
void getEmptySudokuCnf(FILE* fp);
void representSudoku(int* board);

int updateClauseStack(ClauseNode T, char* stackRev, char* stackIdx, Data D);
int updateDataStack(Data D, char* stackRev, char* stackIdx);

char* intToString(int x);
