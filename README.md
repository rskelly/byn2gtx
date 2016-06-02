byn2gtx
=======

[![Build Status](https://travis-ci.org/rskelly/byn2gtx.svg?branch=master)](https://travis-ci.org/rskelly/byn2gtx)

Utility for converting NRCAN's grid shift files to gtx files.

Grid shift files for converting geodata from one vertical reference system to another are distributed in the [byn format](http://www.nrcan.gc.ca/earth-sciences/geomatics/geodetic-reference-systems/9054). 

This tool converts them to the [gtx format usable by proj4](https://trac.osgeo.org/proj/wiki/VerticalDatums).

To convert a file, invoke:

    ./byn2gtx CGG2013n83.byn CGG2013n83.gtx



