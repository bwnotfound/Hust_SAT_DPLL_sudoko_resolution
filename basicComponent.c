char* intToString(int x) {
	char tems[2], tem;
	char s[10];
	s[0] = '\0';
	while (x != 0) {
		tems[0] = x % 10 + '0'; tems[1] = '\0';
		strcat(s, tems);
		x /= 10;
	}
	int i = 0;
	for (;; i++)if (s[i] == '\0')break;
	i--;
	for (int j = 0; j < i - j; j++) {
		tem = s[j];
		s[j] = s[i - j];
		s[i - j] = tem;
	}
	return s;
}