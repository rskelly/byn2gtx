#ifndef __GTX_H__
#define __GTX_H__

/**
 * Contains information about the GTX file.
 */
typedef struct __attribute__((__packed__)) {
	/** 
	 * These properties define the coordinates
	 * of the lower-left corner of the region.
	 */
	double ll_lat;
	double ll_lon;

	/**
	 * These properties define the north-south
	 * and east-west span of the region.
	 */
	double delta_lat;
	double delta_lon;

	/**
	 * Number of rows in the grid.
	 */
	int num_rows;
	/**
	 * Number of rows in the grid.
	 */
	int num_cols;
} GtxHeader;

/**
 * Prints the contents of the GtxHeader.
 * @param A pointer to a GtxHeader.
 */
void gtx_print_header(GtxHeader*);

/**
 * Reads the header data from the given file pointer
 * into the GtxHeader struct.
 * @param A file pointer to the gtx file.
 * @param A pointer to a GtxHeader.
 * @returns 1 on success, 0 on failure.
 */
int gtx_read_header(FILE*, GtxHeader*);

#endif

