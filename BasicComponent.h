int timeLimit;

void clockStart();
int clockStop(int isPrint);
int clockStoreStart();
int clockStoreStop();
int clockStoreOut();
int clockStoreClear();
int getClockStored();
int timeOut();

char* intToString(int x);
FILE* openCnf(char name[], char method[]);
FILE* openPath(char file[], char name[], char method[]);
