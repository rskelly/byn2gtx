byn2gtx
=======

Utility for converting NRCAN's grid shift files to gtx files.

Grid shift files for converting geodata from one vertical reference system to another are distributed in the [byn format](http://www.nrcan.gc.ca/earth-sciences/geomatics/geodetic-reference-systems/9054). 

This tool converts them to the [gtx format usable by proj4](https://trac.osgeo.org/proj/wiki/VerticalDatums).

To convert a file, invoke:

    ./byn2gtx CGG2013n83.byn CGG2013n83.gtx


# Testing

You can test the results of a LAS vertical datum transformation (using liblas' [las2las](http://www.liblas.org/utilities/las2las.html) program, for example) by running

    ./pointsampler file.las file_transformed.las

This will perform a random sampling of the the same locations in both files, which you can then check against the results from [GPS-H](http://webapp.geod.nrcan.gc.ca/geod/tools-outils/gpsh.php?locale=en).

