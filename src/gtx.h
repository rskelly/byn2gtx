#ifndef __GTX_H__
#define __GTX_H__

typedef struct {
	double ll_lat;
	double ll_lon;
	double delta_lat;
	double delta_lon;
	int num_rows;
	int num_cols;
} GtxHeader;

int gtx_print_header(GtxHeader*);

#endif

