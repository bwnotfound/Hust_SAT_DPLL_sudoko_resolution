#include <time.h>
#include <stdio.h>

int start;
int clockIsStart = 0;
int count = 0;
int mod = 1;

void clockStart() {
	if (!clockIsStart) {
		start = clock();
		clockIsStart = 1;
	}
}

int clockStop(int isPrint) {
	if (!clockIsStart)return -1;
	else {
		if (mod == 1 || count % mod == 0) {
			int delta = clock() - start;
			if (isPrint) printf("Time Count%d: %dms\n", count, delta);
			clockIsStart = 0;
			count++;
			return delta;
		}
		count++;
		return -1;
	}
}

int timeStore = 0;
int timeStoreStart;
int clockStoreStart() {
	timeStoreStart = clock();
}
int clockStoreStop() {
	timeStore += clock() - timeStoreStart;
}
int clockStoreOut() {
	printf("Time Stored: %dms\n", timeStore);
}
int clockStoreClear() {
	timeStore = 0;
}