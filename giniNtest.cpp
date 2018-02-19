//***********************************************************************
// giniNtest.cpp
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
// e-mail: jja7w2@umsl.edu
// course: CS 5890
// created on: 23 jul 09
// modified on: 23 jul 09
//
//***********************************************************************

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <sstream>
#include <stdio.h>
//#include <unistd.h>
//#include <cmath>

using namespace std;

//***********************************************************************
// class DataTable declaration
//***********************************************************************
class DataTable {

	// private class variables

	// private methods

public:
	// public class variables
	vector<string> labels;
	vector< vector<float> > data;
	vector<string> dataClasses;

	// public methods

}; // class DataTable

//***********************************************************************
// class DataTable methods
//***********************************************************************

// no methods

//***********************************************************************
// class GiniTest declaration
//***********************************************************************
class GiniTest {

	// private class variables
	DataTable giniData;

	// private methods

public:
	// public class variables

	// public methods
	GiniTest(void); // constructor
	void readData(string infile);
	void saveData(string outfile, int nFold);

}; // class GiniTest

//***********************************************************************
// class GiniTest methods
//***********************************************************************
GiniTest::GiniTest(void) {

	return;
} // GiniTest::GiniTest

//***********************************************************************
void GiniTest::readData(string infile) {
	// local variables
	bool notDone;
	string label;
	float dataValue;
	string classLabel;
	vector<float> inData;
	int attributeCount;
	int indx;

    // declare an input stream to read the data
	ifstream input_stream;

	// open the input stream to read the key
	input_stream.open(infile.c_str());

    // check if the file was OK
    if (input_stream.is_open()){

        // read the data labels
		notDone = true; // set the control flag
		while(notDone){
			// read a label from the input_stream
			input_stream >> label;
			// save the label
			giniData.labels.push_back(label);
			// test if we're done reading labels
			if(label[label.size()-1] == '?') notDone = false;
		} // while

		//set the attributeCount
		attributeCount = (int)giniData.labels.size() - 1;

		//read the data values
		while(!input_stream.eof()){
			// initialize the input data vector
			inData.clear();
			// read a row of data values
			for(indx = 0; indx < attributeCount; indx++){
				input_stream >> dataValue;
				// save the dataValue
				inData.push_back(dataValue);
			} // for
			// save the input data vector
			giniData.data.push_back(inData);
			// read the class label
			input_stream >> classLabel;

			// save the class label
			giniData.dataClasses.push_back(classLabel);
		} // while
	} // if
    else cout << "Error reading the input data file!" << endl << endl; // print error message

	input_stream.close();  // close filestream

	return;
} // GiniTest::readData

//***********************************************************************
void GiniTest::saveData(string outfile, int nFold) {
	// local variables
	int indx, instanceIndx, attributeIndx;
	int startTestIndx, endTestIndx, maxIndx, step;
	int headerIndx, headerMax;
	int attributeCt;
	string trainingfile, testfile;
	string indxVal;
	stringstream out;
	char buffer[100];

	//filestream declaration
	ofstream training_stream;
	ofstream test_stream;

	// initialize the step
	step = ((int)giniData.data.size() - 1) / nFold;
	startTestIndx = 0;
	endTestIndx = step;
	maxIndx = (int)giniData.data.size() - 1;
	attributeCt = (int)giniData.labels.size() - 1;


	// loop nFold times to save the training and test sets
	for(indx = 0; indx < nFold; indx++) {
        // convert the indx to a string
        sprintf(buffer, "%d", indx);
		indxVal = buffer;
		// create the training data filename
		trainingfile = outfile + ".trg" + indxVal;
		//open the training data file stream
		training_stream.open(trainingfile.c_str());

		// create the test data filename
		testfile = outfile + ".tst" + indxVal;
		// open the test data file stream
		test_stream.open(testfile.c_str());

		// output a header into the training data file stream
		headerMax = (int)giniData.labels.size();
		for(headerIndx = 0; headerIndx < headerMax; headerIndx++) {
			training_stream << giniData.labels[headerIndx] << " ";
		} // for
		training_stream << endl;

		// output a header into the test data file stream
		for(headerIndx = 0; headerIndx < headerMax; headerIndx++) {
			test_stream << giniData.labels[headerIndx] << " ";
		} // for
		test_stream << endl;

		// output the first set of training data records
		for(instanceIndx = 0; instanceIndx < startTestIndx; instanceIndx++) {
			// output the data attributes
			for(attributeIndx = 0; attributeIndx < attributeCt; attributeIndx++) {
				training_stream << giniData.data[instanceIndx][attributeIndx] << " ";
			} // for
			// output the class
			training_stream << giniData.dataClasses[instanceIndx] << endl;

		} // for

		// output the set of test data records
		for(instanceIndx = startTestIndx; instanceIndx < endTestIndx; instanceIndx++) {
			// output the data attributes
			for(attributeIndx = 0; attributeIndx < attributeCt; attributeIndx++) {
				test_stream << giniData.data[instanceIndx][attributeIndx] << " ";
			} // for
			// output the class
			test_stream << giniData.dataClasses[instanceIndx] << endl;

		} // for

		// output the last set of training data records
		for(instanceIndx = endTestIndx; instanceIndx < maxIndx; instanceIndx++) {
			// output the data attributes
			for(attributeIndx = 0; attributeIndx < attributeCt; attributeIndx++) {
				training_stream << giniData.data[instanceIndx][attributeIndx] << " ";
			} // for
			// output the class
			training_stream << giniData.dataClasses[instanceIndx] << endl;

		} // for

		// close the training data file stream
		training_stream.close();
		// close the test data file stream
		test_stream.close();

		// increment the indicies
		startTestIndx = startTestIndx + step;
		endTestIndx = endTestIndx + step;
		if(endTestIndx > maxIndx) endTestIndx = maxIndx;

	} // for

	return;
} // GiniTest::saveData

//***********************************************************************
int main(int argc, char* argv[]) {
	// local variables
	string inFilename, outFilename1, outFilename2;
	int numberOfTests;
	GiniTest myTest;
	int indx;
	string commandLine;
	string indxVal;
	stringstream out;
	char buffer[100];

	// check to ensure that we have at least one input argument
	if(argc < 3) {
		// print error message to stdout
		cout << "No input arguments" << endl;
		cout << "command format is: giniNtest inputFileName #" << endl;
		cout << "    the inputFileName is required" << endl;
		cout << "    # represents the integer number of tests" << endl << endl;
	} // if
	else {

		// get the input data filename;
		//inFilename = "iris_test.dat";
		//numberOfTests = 4;
		inFilename = argv[1];
		numberOfTests = atoi(argv[2]);

		// read the training data
		myTest.readData(inFilename);

		// create the training files
		myTest.saveData(inFilename, numberOfTests);

		for(indx = 0; indx < numberOfTests; indx++) {
            // convert the indx to a string
            sprintf(buffer, "%d", indx);
            indxVal = buffer;

			commandLine = "giniTree " + inFilename + ".trg" + indxVal + " " + inFilename + ".tst" + indxVal;

			system (commandLine.c_str());

		} // for
	} // else

	return 0;
}





