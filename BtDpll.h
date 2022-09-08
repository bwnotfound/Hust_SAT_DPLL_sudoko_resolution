#define true 1
#define false 0
typedef int bool;

typedef struct BackNp {
	int isSat;
	int* sat;	// return the result of the satisified sample.
	int learnedIdx;
} *Back;
typedef struct LearnLiteralNp {
	int varIdx, isRev;
} *LearnLiteral;
typedef struct LearnClauseNp {
	LearnLiteral* vector;
	int idx, len;
} *LearnClause;
typedef struct LearnDataNp {
	LearnClause first;
	int len;
} *LearnData;



