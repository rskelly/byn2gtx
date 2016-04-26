#include <stdio.h>
#include "util.h"
#include "gtx.h"

int gtx_read_header(FILE* f, GtxHeader* hdr) {
	int read;
	int size;
	size = sizeof(GtxHeader);
	if(fseek(f, 0, SEEK_SET) != 0)
		return 0;
	read = fread(hdr, 1, size, f);
	if(read != size)
		return 0;
	return 1;
}

void gtx_print_header(GtxHeader* hdr) {
	printf("Printing GTX Header\n");
	printf("------------------------------------------\n");
	printf("  Lower-left Corner (lat, lon): %f, %f\n", double_flip(hdr->ll_lat), double_flip(hdr->ll_lon));
	printf("  Deltas (lat, lon):            %f, %f\n", double_flip(hdr->delta_lat), double_flip(hdr->delta_lon));
	printf("  Rows, Columns:                %d, %d\n", int_flip(hdr->num_rows), int_flip(hdr->num_cols));
	printf("------------------------------------------\n");
	
}

