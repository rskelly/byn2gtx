#include <stdio.h>
#include <string.h>
#include <math.h>

#include "byn2gtx.h"
#include "byn.h"
#include "gtx.h"

void usage() {
	printf("Usage: byn2gtx <input file> <output file>\n");
}

int main(int argc, char** argv) {	
	char* input_file;
	char* output_file;
	FILE* f = 0;
	FILE* g = 0;
	int result;
	
	if(argc < 3) {
		printf("Error: Too few arguments.\n");
		usage();
		result = -1;
		goto finish;
	}

	if(strlen(argv[1]) < 1 || strlen(argv[2]) < 2) {
		printf("Two non-zero length arguments are required.\n");
		usage();
		result = -1;
		goto finish;
	}

	input_file = argv[1];
	output_file = argv[2];
	printf("Opening file for reading: %s\n", input_file);
	if((f = fopen(input_file, "rb")) == 0) {
		perror("Failed to open file:");
		result = -1;
		goto finish;
	}
	
	printf("Opening file for writing: %s\n", output_file);
	if((g = fopen(output_file, "wb")) == 0) {
		perror("Failed to open file:");
		result = -2;
		goto finish;
	}

	byn2gtx(f, g);

finish:
	if(f != 0)
		fclose(f);
	if(g != 0)
		fclose(g);
	return result;

}

int byn2gtx(FILE* bfile, FILE* gfile) {

	printf("Converting...\n");

	int result;
	BynHeader bhdr;
	GtxHeader ghdr;

	if(!byn_read_header(bfile, &bhdr)) {
		printf("Failed to read header.\n");
		return -2;
	}

	if(!bynhdr2gtxhdr(&bhdr, &ghdr)) {
		printf("Failed to convert header.\n");
		return -3;
	}

	byn_print_header(&bhdr);
	gtx_print_header(&ghdr);

	printf("Writing GTX Header\n");
	if((result = fwrite(&ghdr, sizeof(GtxHeader), 1, gfile)) != 1) {
		printf("Failed to write GTX header.\n");
		return -4;
	}

	if((result = fseek(bfile, sizeof(BynHeader), SEEK_SET)) != 0) {
		printf("Failed to seek in BYN file (%d).\n", result);
		return -5;
	}
	if((result = fseek(gfile, sizeof(GtxHeader), SEEK_SET)) != 0) {
		printf("Failed to seek in GTX file (%d).\n", result);
		return -6;
	}

	int r, c;
	short srv;
	int lrv;
	float wv;
	float l_undef = 9999.0 * bhdr.data_scaling_factor;
	short s_undef = 32767;
	printf("Writing data (%dx%d): ", ghdr.num_rows, ghdr.num_cols);
	for(r=0;r<ghdr.num_rows;++r) {
		if(r % 500 == 0) 
			printf(".");
		for(c=0;c<ghdr.num_cols;++c) {
			if(bhdr.data_size == BYN_DATA_SIZE_SHORT) {
				if((result = fread(&srv, sizeof(int), 1, bfile)) != 1) {
					printf("Failed to read height from BYN file (%d).\n", result);
					return -7;
				}
				if(srv == s_undef) {
					wv = 0.0;
				} else {
					wv = (float) srv / bhdr.data_scaling_factor;
				}
				if((result = fwrite(&wv, sizeof(float), 1, gfile)) != 1) {
					printf("Failed to write to GTX file (%d).\n", result);
				}
			} else if(bhdr.data_size == BYN_DATA_SIZE_LONG) {
				if((result = fread(&lrv, sizeof(int), 1, bfile)) != 1) {
					printf("Failed to read height from BYN file (%d).\n", result);
					return -8;
				}
				wv = (float) lrv / bhdr.data_scaling_factor;
				if(wv == l_undef) {
					wv = 0.0;
				} 
				if((result = fwrite(&wv, sizeof(float), 1, gfile)) != 1) {
					printf("Failed to write to GTX file (%d).\n", result);
					return -9;
				}
			} else {
				printf("Invalid data size: %d (%s)\n", bhdr.data_size, byn_data_size_str(bhdr.data_size));
				return -11;
			}
		}
	}
	printf("\nDone.\n");
	return 0;
}

int bynhdr2gtxhdr(BynHeader* bhdr, GtxHeader* ghdr) {
	ghdr->ll_lat = (double) bhdr->south_boundary / 3600.0;
	ghdr->ll_lon = (double) bhdr->west_boundary / 3600.0;
	ghdr->delta_lat = (double) bhdr->ns_spacing / 3600.0;
	ghdr->delta_lon = (double) bhdr->ew_spacing / 3600.0;
	ghdr->num_cols = (int) ((bhdr->east_boundary - bhdr->west_boundary) / bhdr->ew_spacing);
	ghdr->num_rows = (int) ((bhdr->north_boundary - bhdr->south_boundary) / bhdr->ns_spacing);
	return 1;
}
