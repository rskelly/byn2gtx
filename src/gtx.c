#include <stdio.h>
#include "gtx.h"

int gtx_print_header(GtxHeader* hdr) {
	printf("Printing GTX Header\n");
	printf("------------------------------------------\n");
	printf("  Lower-left Corner (lat, lon): %f, %f\n", hdr->ll_lat, hdr->ll_lon);
	printf("  Deltas (lat, lon):            %f, %f\n", hdr->delta_lat, hdr->delta_lon);
	printf("  Rows, Columns:                %d, %d\n", hdr->num_rows, hdr->num_cols);
	printf("------------------------------------------\n");
	
}

