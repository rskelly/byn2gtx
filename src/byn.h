#ifndef __BYN_H__
#define __BYN_H__

#define BYN_MODEL_LOCAL 0
#define BYN_MODEL_GLOBAL 1

#define BYN_DATA_UNDEFINED 0
#define BYN_DATA_GEOID_HEIGHTS 1
#define BYN_DATA_NS_DEFLECTIONS 2
#define BYN_DATA_EW_DEFLECTIONS 3
#define BYN_DATA_GRAVITY 4
#define BYN_DATA_ELEVATIONS 5
#define BYN_DATA_SEA_SURFACE_HEIGHTS 6
#define BYN_DATA_SEA_SURFACE_TOPOGRAPHY 7
#define BYN_DATA_OTHER 8

#define BYN_DATA_SIZE_SHORT 2
#define BYN_DATA_SIZE_LONG 4

#define BYN_DATUM_ITRF 0
#define BYN_DATUM_NAD83 1

#define BYN_ELLIPSOID_GRS80 0
#define BYN_ELLIPSOID_WGS84 1
#define BYN_ELLIPSOID_TOPEX_EGM96 2
#define BYN_ELLIPSOID_GRS67 3

#define BYN_BYTE_ORDER_BE 0
#define BYN_BYTE_ORDER_LE 1

#define BYN_BOUNDARY_NOSCALE 0
#define BYN_BOUNDARY_SCALED 1

typedef struct {
	int south_boundary;
	int north_boundary;
	int west_boundary;
	int east_boundary;
	short ns_spacing;
	short ew_spacing;
	short model_type;
	short data_type;
	double data_scaling_factor;
	short data_size;
	short std_dev;
	double std_dev_scaling_factor;
	short datum;
	short ellipsoid;
	short byte_order;
	short boundaries_scaled;
	char reserved;
} BynHeader;

int byn_read_header(FILE*, BynHeader*);
int byn_print_header(BynHeader*);
char* byn_model_type_str(short);
char* byn_data_type_str(short);
char* byn_datum_str(short);
char* byn_ellipsoid_str(short);
char* byn_byte_order_str(short);
char* byn_boundaries_scaled_str(short);
char* byn_data_size_str(short);

#endif


