//***********************************************************************
// giniTree.cpp
//
//   this application reads in text data and builds a decision tree using
//    the gini index as its spliting metric. 
//
// INPUTS: (from disk file)
//        text data input file [formatted data] [REQUIRED]
//        text data test file [formatted data] [optional]
//
// OUTPUTS: (to stdout)
//        text display of the trained tree
//
//        (to disk file) [if test file was given as input]
//        text data output file showing the predicted classes for the 
//          test data [formatted]
//
//***********************************************************************
//  WARNING: none
//
//***********************************************************************
// IMPLEMENTATION NOTE: all files are in the executable working directory
//
//***********************************************************************
// created by: j. aleshunas
// e-mail: jalesh@webster.edu
// created on: 18 jul 09
// modified on: 23 jul 09
//
// © 2009 John Aleshunas
//
//  This file is giniTree.cpp of giniTree.
//
//  giniTree is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; version 2 of the License.
//
//  giniTree is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with K-Means (as gpl.txt); if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
//***********************************************************************

#include "giniTree.hpp"

int main(int argc, char* argv[]) {
	// local variables
	GiniTree theGiniTree;
	string inFilename, testFilename;

	// check to ensure that we have at least one input argument
	if(argc < 2) {
		// print error message to stdout
		cout << "No input data file." << endl;
		cout << "command format is: giniTree inputFileName [testFileName]" << endl;
		cout << "    the inputFileName is required" << endl;
		cout << "    the testFileName is optional" << endl << endl;
	} // if
	else {
	
		// get the input data filename;
		inFilename = argv[1];

		// read the training data into the tree class
		theGiniTree.readTrainingData(inFilename);

		// train the tree
		theGiniTree.trainTree();

		// if we have test data
		if(argc > 2) {
		
			// read test data
			testFilename = argv[2];
			theGiniTree.readTestData(testFilename);

			// test the tree
			theGiniTree.testTree(testFilename);

		} // if
	} // else

	return 0;
}

