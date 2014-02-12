#include <stdio.h>
#include <string.h>

#include "util.h"
#include "byn2gtx.h"
#include "byn.h"
#include "gtx.h"

#define DATUM_TYPE double

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

	if((result = byn_read_header(bfile, &bhdr)) != 0) {
		printf("Failed to read header (%d).\n", result);
		return -2;
	}

	if((result = bynhdr2gtxhdr(&bhdr, &ghdr)) != 0) {
		printf("Failed to convert header (%d).\n", result);
		return -3;
	}

	byn_print_header(&bhdr);
	gtx_print_header(&ghdr);

	printf("Writing GTX Header\n");
	GtxHeader g2;
	g2.ll_lat = double_flip(ghdr.ll_lat);
	g2.ll_lon = double_flip(ghdr.ll_lon);
	g2.delta_lat = double_flip(ghdr.delta_lat);
	g2.delta_lon = double_flip(ghdr.delta_lon);
	g2.num_rows = int_flip(ghdr.num_rows);
	g2.num_cols = int_flip(ghdr.num_cols);
	if((result = fwrite(&g2, sizeof(GtxHeader), 1, gfile)) != 1) {
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
					wv = float_flip((float) srv / bhdr.data_scaling_factor);
				}
				if((result = fwrite(&wv, sizeof(DATUM_TYPE), 1, gfile)) != 1) {
					printf("Failed to write to GTX file (%d).\n", result);
				}
			} else if(bhdr.data_size == BYN_DATA_SIZE_LONG) {
				if((result = fread(&lrv, sizeof(int), 1, bfile)) != 1) {
					printf("Failed to read height from BYN file (%d).\n", result);
					return -8;
				}
				wv = float_flip((float) lrv / bhdr.data_scaling_factor);
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
	return 0;
}
