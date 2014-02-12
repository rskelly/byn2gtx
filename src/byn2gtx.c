#include <stdio.h>
#include <string.h>
#include <math.h>

#include "util.h"
#include "byn2gtx.h"
#include "byn.h"
#include "gtx.h"

#define DATUM_TYPE float

void usage() {
	printf("Usage: byn2gtx <input file> [output file]\n");
	printf("  If one file is given, the header is printed.\n");
	printf("  If two files are given, the input file is converted and written as the output file.\n");
}

int main(int argc, char** argv) {	
	
	if(argc < 2) {
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
	if(argc >= 3)
		output_file = argv[2];
	
	printf("Opening file for reading: %s\n", input_file);
	if((f = fopen(input_file, "rb")) == 0) {
		perror("Failed to open file:");
		return -1;
	}

	if(output_file != 0 && strlen(output_file) > 0) {	
		printf("Opening file for writing: %s\n", output_file);
		if((g = fopen(output_file, "wb")) == 0) {
			perror("Failed to open file:");
			fclose(f);
			return -1;
		}
		byn2gtx(f, g);
	} else {
		char* p = strrchr(input_file, '.');
		if(p && !strcmp(p, ".byn")) {
			BynHeader bhdr;
			byn_read_header(f, &bhdr);
			byn_print_header(&bhdr);
		} else if(p && !strcmp(p, ".gtx")) {
			GtxHeader ghdr;
			gtx_read_header(f, &ghdr);
			gtx_print_header(&ghdr);
		}
	}

	if(f != 0)
		fclose(f);
	if(g != 0)
		fclose(g);
	return 0;
}

int byn2gtx(FILE* bfile, FILE* gfile) {

	printf("Converting...\n");

	int result;
	BynHeader bhdr;
	GtxHeader ghdr;

	// Read the byn header.
	if((result = byn_read_header(bfile, &bhdr)) != 0) {
		printf("Failed to read header (%d).\n", result);
		return -2;
	}

	// Convert the byn header to a gtx header.
	if((result = bynhdr2gtxhdr(&bhdr, &ghdr)) != 0) {
		printf("Failed to convert header (%d).\n", result);
		return -3;
	}

	// PRint the headers.
	byn_print_header(&bhdr);
	gtx_print_header(&ghdr);

	// Write the gtx header to the file.
	printf("Writing GTX Header\n");
	if((result = fwrite(&ghdr, sizeof(GtxHeader), 1, gfile)) != 1) {
		printf("Failed to write GTX header.\n");
		return -4;
	}

	if((result = fseek(gfile, sizeof(GtxHeader), SEEK_SET)) != 0) {
		printf("Failed to seek in GTX file (%d).\n", result);
		return -6;
	}



	int r, c;
	int rv;
	DATUM_TYPE wv;
	int datsize = bhdr.data_size == BYN_DATA_SIZE_SHORT ? 2 : 4;
	int doflip = bhdr.byte_order == BYN_BYTE_ORDER_BE ? 1 : 0;
	double scale = bhdr.data_scaling_factor;
	int undef = datsize == 2 ? 32767 : 9999 ;
	int rows = int_flip(ghdr.num_rows);
	int cols = int_flip(ghdr.num_cols);
	int counter = 0;
	printf("Writing data (%dx%d): ", rows, cols);
	// Loop over each gtx row.
	for(r=rows-1;r>=0;--r) {
			
		// Last row first in GTX.
		if((result = fseek(bfile, sizeof(BynHeader) + (r * cols * sizeof(DATUM_TYPE)), SEEK_SET)) != 0) {
			printf("Failed to seek in BYN file (%d).\n", result);
			return -5;
		} 
	
		// Loop over each gtx column.
		for(c=0;c<cols;++c) {
			if((result = fread(&rv, datsize, 1, bfile)) != 1) {
				printf("Failed to read height from BYN file (%d).\n", result);
				return -7;
			}
			if(rv == undef) {
				wv = -88.8888;
			} else {
				wv = (float) rv / scale;
				if(doflip)
					flip((char *) &wv, sizeof(DATUM_TYPE));
			}
			if((result = fwrite(&wv, sizeof(DATUM_TYPE), 1, gfile)) != 1) {
				printf("Failed to write to GTX file (%d).\n", result);
			}

			if(++counter % 1000000 == 0) 
				printf(".");
		}
	}
	printf("\nDone (%d).\n", counter);
	return 0;
}

int bynhdr2gtxhdr(BynHeader* bhdr, GtxHeader* ghdr) {
	ghdr->num_cols = int_flip((int) ceil((bhdr->east_boundary - bhdr->west_boundary) / bhdr->ew_spacing) + 1);
	ghdr->num_rows = int_flip((int) ceil((bhdr->north_boundary - bhdr->south_boundary) / bhdr->ns_spacing) + 1);
	ghdr->ll_lat = double_flip((double) bhdr->south_boundary / 3600.0);
	ghdr->ll_lon = double_flip((double) bhdr->west_boundary / 3600.0);
	ghdr->delta_lat = double_flip((double) bhdr->ns_spacing / 3600.0);
	ghdr->delta_lon = double_flip((double) bhdr->ew_spacing / 3600.0);
	return 0;
}
