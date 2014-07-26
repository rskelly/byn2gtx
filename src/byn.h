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

/**
 * Contains information about the BYN file.
 */
typedef struct {
	//@{
	/**
	 * These properties define the geographic boundaries
	 * of the BYN file.
	 */
	int south_boundary; 
	int north_boundary;
	int west_boundary;
	int east_boundary;
	//@}
	//@{
	/**
	 * These properties give the scale factor
	 * for the boundaries.
	 */
	short ns_spacing;
	short ew_spacing;
	//@}
	/** The model type (local or global) of the BYN file. */
	short model_type;
	/** The data type of the BYN file. */
	short data_type;
	/** The data scaling factor of the BYN file. */
	double data_scaling_factor;
	/** The data size (4 or 8) of the BYN file. */
	short data_size;
	/** The standard deviation of values in the BYN file. */
	short std_dev;
	/** The scaling factor for the standard deviation. */
	double std_dev_scaling_factor;
	/** The datum used by the BYN file (ITRF or NAD83). */
	short datum;
	/** The ellipsoid used by the BYN file. */
	short ellipsoid;
	/** The byte ordering of the BYN file (little, big endian). */
	short byte_order;
	/** Indicates whether the boundaries are scaled. */
	short boundaries_scaled;
	/** Reserved. */
	char reserved;
} BynHeader;

/**
 * Reads the header data from the given file pointer
 * into the BynHeader struct.
 * @param A file pointer to the byn file.
 * @param A pointer to a BynHeader.
 * @returns 1 on success, 0 on failure.
 */
int byn_read_header(FILE*, BynHeader*);

/**
 * Prints the contents of the BynHeader.
 * @param A pointer to a BynHeader.
 */
void byn_print_header(BynHeader*);

/**
 * Returns a string containing the BYN model type
 * for the given value, which should equal one of
 * the BYN_MODEL_* constants.
 * @param A short representing the model type.
 * @returns A string representation of the model type.
 */
char* byn_model_type_str(short);

/**
 * Returns a string containing the BYN data type
 * for the given value, which should equal one of
 * the BYN_DATA_TYPE_* constants.
 * @param A short representing the data type.
 * @returns A string representation of the model type.
 */
char* byn_data_type_str(short);

/**
 * Returns a string contaiing the BYN datum string
 * for the given value, which should equal one of the
 * BYN_DATUM_* constants.
 * @param A short representing the datum type.
 * @returns A string representation of the datum.
 */
char* byn_datum_str(short);

/**
 * Returns a string contaiing the BYN ellipsoid string
 * for the given value, which should equal one of the
 * BYN_ELLIPSOID_* constants.
 * @param A short representing the ellipsoid type.
 * @returns A string representation of the ellipsoid.
 */
char* byn_ellipsoid_str(short);

/**
 * Returns a string contaiing the BYN byte order string
 * for the given value, which should equal one of the
 * BYN_BYTE_ORDER_* constants.
 * @param A short representing the byte order.
 * @returns A string representation of the byte order.
 */
char* byn_byte_order_str(short);

/**
 * Returns a string contaiing the BYN boundary scaling type
 * for the given value, which should equal one of the
 * BYN_BOUNDARY_* constants.
 * @param A short representing the boundary scaling type.
 * @returns A string representation of the boundary scaling type.
 */
char* byn_boundaries_scaled_str(short);

/**
 * Returns a string contaiing the BYN data size
 * for the given value, which should equal one of the
 * BYN_DATA_SIZE_* constants.
 * @param A short representing the data size.
 * @returns A string representation of the data size.
 */
char* byn_data_size_str(short);

#endif


