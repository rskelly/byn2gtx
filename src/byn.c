#include <stdio.h>
#include "byn.h"

int byn_read_header(FILE* f, BynHeader* hdr) {
	int read;
	int size;
	size = sizeof(BynHeader);
	read = fread(hdr, 1, size, f);
	if(read != size)
		return -1;
	return 0;
}

int byn_print_header(BynHeader* hdr) {
	printf("Printing BYN Header\n");
	printf("------------------------------------------\n");
	printf("  Boundaries (NSWE):    %d, %d, %d, %d\n", hdr->north_boundary, hdr->south_boundary, hdr->west_boundary, hdr->east_boundary);
	printf("  Spacing (NS, EW):     %d, %d\n", hdr->ns_spacing, hdr->ew_spacing);
	printf("  Model Type:           %s \n", byn_model_type_str(hdr->model_type));
	printf("  Data Type:            %s \n", byn_data_type_str(hdr->data_type));
	printf("  Scaling Factor:       %f \n", hdr->data_scaling_factor);
	printf("  Data Size:            %s\n", byn_data_size_str(hdr->data_size));	
	printf("  Standard Deviation:   %d\n", hdr->std_dev);
	printf("  StdDev. Scale Factor: %f\n", hdr->std_dev_scaling_factor);
	printf("  Datum:                %s\n", byn_datum_str(hdr->datum));
	printf("  Ellipsoid:            %s\n", byn_ellipsoid_str(hdr->ellipsoid));
	printf("  Byte Order:           %s\n", byn_byte_order_str(hdr->byte_order));
	printf("  Scale for Boundaries: %s\n", byn_boundaries_scaled_str(hdr->boundaries_scaled));
	printf("------------------------------------------\n");
}

char* byn_byte_order_str(short v) {
	switch(v) {
		case BYN_BYTE_ORDER_BE: return "big endian";
		case BYN_BYTE_ORDER_LE: return "little endian";
		default: return "invalid";
	}
}

char* byn_boundaries_scaled_str(short v) {
	switch(v) {
		case BYN_BOUNDARY_NOSCALE: return "not scaled";
		case BYN_BOUNDARY_SCALED: return "scaled";
		default: return "invalid";
	}
}

char* byn_model_type_str(short v) {
	switch(v) {
		case BYN_MODEL_LOCAL: return "local";
		case BYN_MODEL_GLOBAL: return "global";
		default: return "invalid";
	}
}

char* byn_data_type_str(short v) {
	switch(v) {
		case BYN_DATA_UNDEFINED: return "undefined";
		case BYN_DATA_GEOID_HEIGHTS: return "geoid heights";
		case BYN_DATA_NS_DEFLECTIONS: return "north/south deflections";
		case BYN_DATA_EW_DEFLECTIONS: return "east/west deflections";
		case BYN_DATA_GRAVITY: return "gravity";
		case BYN_DATA_ELEVATIONS: return "elevations";
		case BYN_DATA_SEA_SURFACE_HEIGHTS: return "sea surface heights";		case BYN_DATA_SEA_SURFACE_TOPOGRAPHY: return "sea surface topography";
		case BYN_DATA_OTHER: return "other";
		default: return "invalid";
	}
}

char* byn_datum_str(short v) {
	switch(v) {
		case BYN_DATUM_ITRF: return "ITRF";
		case BYN_DATUM_NAD83: return "NAD83";
		default: return "invalid";
	}
}

char* byn_ellipsoid_str(short v) {
	switch(v) {
		case BYN_ELLIPSOID_GRS80: return "GRS80";
		case BYN_ELLIPSOID_WGS84: return "WGS84";
		case BYN_ELLIPSOID_TOPEX_EGM96: return "TOPEX/EGM96";
		case BYN_ELLIPSOID_GRS67: return "GRS67";
		default: return "invalid";
	}
}

char* byn_data_size_str(short v) {
	switch(v) {
		case BYN_DATA_SIZE_SHORT: return "short (2)";
		case BYN_DATA_SIZE_LONG: return "long (4)";
		default: return "invalid";
	}
}