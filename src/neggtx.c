#include <stdio.h>
#include <string.h>
#include <math.h>

#include "util.h"
#include "gtx.h"

#define DATUM_TYPE float

void usage() {
	printf("Usage: byn2gtx <input file> [output file]\n");
	printf("  If one file is given, the header is printed.\n");
	printf("  If two files are given, the input file is converted and written as the output file.\n");
}

int main(int argc, char** argv) {	
	
	if(argc < 3) {
		printf("Error: Too few arguments.\n");
		usage();
		return -1;
	}

	char* input_file = 0;
	char* output_file = 0;
	FILE* f = 0;
	FILE* g = 0;
	int result;

	input_file = argv[1];
	output_file = argv[2];
	
	if(!strcmp(input_file, output_file)) {
		printf("The file names must differ.\n");
		return -1;
	}
	
	printf("Opening file for reading: %s\n", input_file);
	if((f = fopen(input_file, "rb")) == 0) {
		perror("Failed to open file:");
		return -1;
	}

	printf("Opening file for writing: %s\n", output_file);
	if((g = fopen(output_file, "wb")) == 0) {
		perror("Failed to open file:");
		fclose(f);
		return -1;
	}

	neggtx(f, g);

	if(f != 0)
		fclose(f);
	if(g != 0)
		fclose(g);
	return 0;
}

int neggtx(FILE* bfile, FILE* gfile) {

	printf("Converting...\n");

	int result;
	GtxHeader ghdr;

	// Read the gtx header.
	if((result = gtx_read_header(bfile, &ghdr)) != 0) {
		printf("Failed to read header (%d).\n", result);
		return -2;
	}

	// PRint the header.
	gtx_print_header(&ghdr);

	// Write the gtx header to the file.
	printf("Writing GTX Header\n");
	if((result = fwrite(&ghdr, sizeof(GtxHeader), 1, gfile)) != 1) {
		printf("Failed to write GTX header.\n");
		return -4;
	}

	// Seek to the right place in the dest file.
	if((result = fseek(gfile, sizeof(GtxHeader), SEEK_SET)) != 0) {
		printf("Failed to seek in GTX file (%d).\n", result);
		return -6;
	}

	// Seek to the right place in the source file.
	if((result = fseek(bfile, sizeof(GtxHeader), SEEK_SET)) != 0) {
		printf("Failed to seek in destination file (%d).\n", result);
		return -5;
	} 

	int r, c;
	DATUM_TYPE wv;
	int rows = int_flip(ghdr.num_rows);
	int cols = int_flip(ghdr.num_cols);
	int counter = 0;
	int datsize = sizeof(DATUM_TYPE);
	printf("Writing data (%dx%d): ", rows, cols);
	// Loop over each gtx row.
	for(r=0;r<rows;++r) {
		// Loop over each gtx column.
		for(c=0;c<cols;++c) {
			if((result = fread(&wv, sizeof(DATUM_TYPE), 1, bfile)) != 1) {
				printf("Failed to read from GTX file (%d).\n", result);
				return -20;
			}
			
			flip((char *) &wv, datsize);
			wv = -wv;
			flip((char *) &wv, datsize);
			
			if((result = fwrite(&wv, sizeof(DATUM_TYPE), 1, gfile)) != 1) {
				printf("Failed to write to GTX file (%d).\n", result);
				return -21;
			}
			if(++counter % 1000000 == 0) 
				printf(".");
		}
	}
	printf("\nDone (%d).\n", counter);
	return 0;
}
