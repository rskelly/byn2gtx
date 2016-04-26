#include <stdio.h>
#include <string.h>
#include <math.h>

#include "byn2gtx.h"
#include "byn.h"
#include "gtx.h"
#include "util.h"

void usage() {
	printf("Usage: byn2gtx <input file> <output file>\n");
}

int main(int argc, char** argv) {	
	if(argc < 3) {
		printf("Error: Too few arguments.\n");
		usage();
		return 1;
	}

	if(strlen(argv[1]) < 1 || strlen(argv[2]) < 2) {
		printf("Two non-zero length arguments are required.\n");
		usage();
		return 1;
	}

	char *input_file = argv[1];
	char *output_file = argv[2];
	FILE* f = 0;
	FILE* g = 0;

	printf("Opening file for reading: %s\n", input_file);
	if((f = fopen(input_file, "rb")) == 0) {
		perror("Failed to open file:");
		return 1;
	}
	
	printf("Opening file for writing: %s\n", output_file);
	if((g = fopen(output_file, "wb")) == 0) {
		perror("Failed to open file:");
		fclose(f);
		return 1;
	}

	// Call the conversion routine.
	int result = byn2gtx(f, g);

	fclose(f);
	fclose(g);

	return result;

}

// Converts a byn file to a gtx file.
int byn2gtx(FILE* bfile, FILE* gfile) {

	printf("Converting...\n");

	int result;
	BynHeader bhdr;
	GtxHeader ghdr;

	if(!byn_read_header(bfile, &bhdr)) {
		printf("Failed to read header.\n");
		return 1;
	}

	// Convert the byn header to a gtx header.
	bynhdr2gtxhdr(&bhdr, &ghdr);

	// Print out metadata.
	byn_print_header(&bhdr);
	gtx_print_header(&ghdr);

	printf("Writing GTX Header\n");
	if((result = fwrite(&ghdr, sizeof(GtxHeader), 1, gfile)) != 1) {
		printf("Failed to write GTX header.\n");
		return 1;
	}

	// Seek to the start of the values in the byn file.
	if((result = fseek(bfile, sizeof(BynHeader), SEEK_SET)) != 0) {
		printf("Failed to seek in BYN file (%d).\n", result);
		return 1;
	}

	// Seek to the start of the values in the gtx file.
	if((result = fseek(gfile, sizeof(GtxHeader), SEEK_SET)) != 0) {
		printf("Failed to seek in GTX file (%d).\n", result);
		return 1;
	}

	// Undefined value for long data.
	int l_undef = (int) (9999.0 * bhdr.data_scaling_factor);
	// Undefined value for short data.
	short s_undef = 32767;
	int cols = int_flip(ghdr.num_cols);
	int rows = int_flip(ghdr.num_rows);
	// Values for short and long data.
	short srv;
	int lrv;
	// Output value
	float wv;
	// Col and row indices.
	int c, r;
	printf("Writing data (%dx%d): ", cols, rows);
	for(r = 0; r < rows; ++r) {
		// Seek to the output row. The GTX format places coordinates in the opposite vertical order to BYN.
		fseek(gfile, (rows - r - 1) * cols * sizeof(float) + sizeof(GtxHeader), SEEK_SET);
		for(c = 0; c < cols; ++c) {
			// Read short data
			if(bhdr.data_size == BYN_DATA_SIZE_SHORT) {
				if((result = fread(&srv, sizeof(short), 1, bfile)) != 1) {
					printf("Failed to read height from BYN file (%d).\n", result);
					return 1;
				}
				if(srv == s_undef) {
					wv = -88.8888;
				} else {
					wv = (float) srv / bhdr.data_scaling_factor;
				}
			// Read long data.
			} else if(bhdr.data_size == BYN_DATA_SIZE_LONG) {
				if((result = fread(&lrv, sizeof(int), 1, bfile)) != 1) {
					printf("Failed to read height from BYN file (%d).\n", result);
					return 1;
				}
				if(lrv == l_undef) {
					wv = -88.8888;
				} else {
					wv = (float) lrv / bhdr.data_scaling_factor;
				} 
			} else {
				printf("Invalid data size: %d (%s)\n", bhdr.data_size, byn_data_size_str(bhdr.data_size));
				return 1;
			}
			// Flip and write output data.
			wv = float_flip(wv);	
			if((result = fwrite(&wv, sizeof(float), 1, gfile)) != 1) {
				printf("Failed to write to GTX file (%d).\n", result);
				return 1;
			}
		}
	}
	printf("Done.\n");
	return 0;
}

// Convert the BYN header to a GTX header.
void bynhdr2gtxhdr(BynHeader* bhdr, GtxHeader* ghdr) {
	ghdr->ll_lat = double_flip((double) bhdr->south_boundary / 3600.0);
	ghdr->ll_lon = double_flip((double) bhdr->west_boundary / 3600.0);
	ghdr->delta_lat = double_flip((double) bhdr->ns_spacing / 3600.0);
	ghdr->delta_lon = double_flip((double) bhdr->ew_spacing / 3600.0);
	ghdr->num_cols = int_flip((int) ((bhdr->east_boundary - bhdr->west_boundary) / bhdr->ew_spacing) + 1);
	ghdr->num_rows = int_flip((int) ((bhdr->north_boundary - bhdr->south_boundary) / bhdr->ns_spacing) + 1);
}
