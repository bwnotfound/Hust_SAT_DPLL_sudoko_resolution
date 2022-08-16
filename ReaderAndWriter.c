#include <stdio.h>

int readInt(FILE* fp) {
	char c = fgetc(fp);
	int re = 0, f = 1;
	while (c < '0' || c>'9') { if (c == '-')f = -1; c = fgetc(fp);}
	while(c>='0' && c <= '9') {
		re = re * 10 + c - '0';
		c = fgetc(fp);
	};
	return re * f;
}



