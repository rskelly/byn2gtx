byn2gtx
=======

Utility for converting NRCAN's grid shift files to gtx files.

Grid shift files for converting geodata from one vertical reference system to another are distributed in the byn format: http://www.nrcan.gc.ca/earth-sciences/geomatics/geodetic-reference-systems/9054

This tool converts them to the gtx format usable by proj4.

To convert a file, invoke:

./byn2gtx <byn file> <gtx file>


