#include "util.h"

double double_flip(double v) {
	flip((char *) &v, sizeof(double));
	return v;
}

float float_flip(float v) {
	flip((char *) &v, sizeof(float));
	return v;
}

int int_flip(int v) {
	flip((char *) &v, sizeof(int));
	return v;
}

short short_flip(short v) {
	flip((char *) &v, sizeof(short));
	return v;
}

void flip_noop(char* f, int len) { }

void flip(char* f, int len) {
	char t;
	int i;
	for(i=0;i<(int)(len/2);++i) {
		t = f[len-i-1];
		f[len-i-1] = f[i];
		f[i] = t;
	}
}

