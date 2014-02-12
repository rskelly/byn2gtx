#include <stdio.h>

int main(){
printf("Double: %d\n", (int) sizeof(double));
printf("Float: %d\n", (int) sizeof(float));
printf("Long: %d\n", (int) sizeof(long));
printf("Int: %d\n", (int) sizeof(int));
printf("Short: %d\n", (int) sizeof(short));

short x = 0xff;
char* y = (char *) &x;
printf("Bytes: %d, %d\n", y[0], y[1]);
double z = 1243.0;
char* q = (char *) &z;
z = (double) *q;
printf("%h\n", q);
printf("%f\n", z);
return 0;
}
