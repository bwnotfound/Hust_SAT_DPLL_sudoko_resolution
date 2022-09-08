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
void dpllInit(Data D);
int* DPLL(Data D);
void dpllEnd(Data D);
void addClause(ClauseNode T, Data D);
LiteralNode getLiteral();
ClauseNode getClause();
Data getData();
Data deleteData(Data D);
void deleteClause(ClauseNode T, Data D);

//void initCnf(FILE* fpCsv, char* path, char* pathTem);
void getEmptySudokuCnf(FILE* fp);
//void representSudoku(int* board);

int updateClauseStack(ClauseNode T, char* stackRev, char* stackIdx, Data D);
int updateDataStack(Data D, char* stackRev, char* stackIdx);

void outputDataIntoCnf(Data D, int* res);
void outputParsedCnfIntoCnf(Data D);
void turnDataIntoCnf(Data D, FILE* fpWrite);
void outputResult(int* res, int len, FILE* fpWrite, int time);
int* startDpll(Data D);
Data copyAll(Data D);

int createSingleSudoku(int n, FILE* fpWrite);
void playSingleSudoku(FILE* fpRead);

void playDoubleSudoku(FILE* fpRead);
int createDoubleSudoku(int n, FILE* fpWrite);
void createDoubleSudokuUnique(FILE* fpWrite);

int* BtDPLL(Data D);
int* DPLL3(Data D);

int* verifyDpllResult(Data D, FILE* fpRead);