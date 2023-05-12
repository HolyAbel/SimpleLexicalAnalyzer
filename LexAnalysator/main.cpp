#include "globals.h"
#include "LexAnalysator.h"

void main() {
	try {
		LexAnalysator la;

		ifstream iFile("inputFile.txt");
		if (!iFile.is_open()) {
			throw exception("Could not open inputFile.");
		}

		ofstream oFile("outputFile.txt", ios::in | ios::out | ios::trunc);
		if (!oFile.is_open()) {
			throw exception("Could not open outputFile.");
		}

		la.AnalyseFile(&iFile, &oFile);

		iFile.close();
		oFile.close();
	}
	catch (exception& exc) {
		cerr << "Error: " << exc.what() << endl;
	}
}