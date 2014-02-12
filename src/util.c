#include "util.h"

double double_flip(double v) {
	double out;
	char* d1 = (char *) &v;
	char* d2 = (char *) &out;
	int i;
	for(i=0;i<8;++i)
		d2[i] = d1[7-i];
	return out;
}

float float_flip(float v) {
	float out;
	char* d1 = (char *) &v;
	char* d2 = (char *) &out;;
	int i;
	for(i=0;i<4;++i)
		d2[i] = d1[3-i];
	return out;
}

int int_flip(int v) {
	int out;
	char* d1 = (char *) &v;
	char* d2 = (char *) &out;
	int i;
	for(i=0;i<4;++i)
		d2[i] = d1[3-i];
	return out;
}

short short_flip(short v) {
	short out;
	char* d1 = (char *) &v;
	char* d2 = (char *) &out;
	int i;
	for(i=0;i<2;++i)
		d2[i] = d1[1-i];
	return out;

}

