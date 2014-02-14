#include <liblas/liblas.hpp>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;
using namespace liblas;

#define SAMPLES 20
// If STEP is 0, a random index will be used to select points,
// otherwise a point is selected every count/SAMPLES places.
#define STEP 1

/**
 * Returns the number of points in a las file.
 */
int get_num_points(char* file) {
	ifstream ifs;
	ifs.open(file, ios::in|ios::binary);
	
	ReaderFactory f;
	liblas::Reader r = f.CreateWithStream(ifs);
	
	Header const& header = r.GetHeader();
	int count = header.GetPointRecordsCount();
	ifs.close();
	return count;
}

/**
 * This program takes a sample of points from one or more las files. The files
 * should be derived from one another -- they should have the same number of
 * points, and a point at the same index in each file should correspond to the
 * same real-world location.
 */ 
int main(int argc, char** argv) {
	
	if(argc < 2) {
		cout << "File name(s) required.\n";
		return -1;
	}		

	int i;
	int j;
	
	// Get an array of the files.
	int numFiles = argc-1;
	char* files[numFiles];
	for(i=1;i<argc;++i) {
		files[i-1] = argv[i];
	}
	
	// We don't want scientific notation in the output stream.
	cout.setf(ios::fixed);

	// Seed the random number generator.
	srand(time(NULL));
	
	// This is the number of samples, and an array to store the index of
	// each sample (because it'll come from the same place in each of the
	// given files).
	int sampleIndices[SAMPLES];
	int rnd = 0;
	int count = get_num_points(files[0]);
	// Get a bunch of random indices.
	for(i=0;i<SAMPLES;++i) {
		rnd = !STEP ? rand() % count : rnd + (int) (count/SAMPLES);
		sampleIndices[i] = rnd;
	}

	// A table of values.
	double values[SAMPLES][numFiles][3];
	
	// Output the spreadsheet header.
	for(i=0;i<numFiles;++i) {
		if(i > 0)
			cout << ",";
		cout << "x" << i << ",y" << i << ",z" << i;
	}
	cout << '\n';
	

	ifstream ifs;
	ReaderFactory f;

	// Iterate over each file, open it...
	for(i=0;i<numFiles;++i) {
		
		ifs.open(files[i], ios::in|ios::binary);
		Reader r = f.CreateWithStream(ifs);

		// Grab samples from the file.		
		for(j=0;j<SAMPLES;++j) {
			r.ReadPointAt(sampleIndices[j]);
			Point p = r.GetPoint();
			values[j][i][0] = p.GetX();
			values[j][i][1] = p.GetY();
			values[j][i][2] = p.GetZ();
		} 

		// Close the stream.
		ifs.close();
		
	}

	// Iterate over the values and print them out.
	for(i=0;i<SAMPLES;++i) {
		for(j=0;j<numFiles;++j) {
			if(j > 0)
				cout << ',';
			cout << values[i][j][0] << ',';
			cout << values[i][j][1] << ',';
			cout << values[i][j][2];
		}
		cout << '\n';
	}
}
