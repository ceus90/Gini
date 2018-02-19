//***********************************************************************
// giniTree.h
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
//  This file is giniTree.h of giniTree.
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

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
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
// class WorkingData declaration
//***********************************************************************
class WorkingData {

	// private class variables
	
	// private methods

public:
	// public class variables
	float dataValue;
	string classLabel;

	// public methods
	
}; // class WorkingData

//***********************************************************************
// class WorkingData methods
//***********************************************************************

// no methods

//***********************************************************************
// class TreeNode declaration
//***********************************************************************
class TreeNode {

	// private class variables

	// private methods

public:
	// public class variables
	string nodeType; // Node or Leaf
	int level; 
	bool leftChildComplete;
	bool rightChildComplete;
	int splitAttribute;
	float splitThreshold;
	TreeNode* leftChildNode;
	TreeNode* rightChildNode;
	TreeNode* parentNode;
	vector<int> dataPoints; // indices of data instances in this node

	// public methods
	TreeNode(void); // constructor
	
}; // class TreeNode

//***********************************************************************
// class TreeNode methods
//***********************************************************************
TreeNode::TreeNode(void){
	
	// initialize the class variables
	nodeType = "Node";
	level = 0;
	splitAttribute = 99;
	splitThreshold = 0;
	leftChildComplete = false;
	rightChildComplete = false;
	leftChildNode = NULL;
	rightChildNode = NULL;
	parentNode = NULL;

	return;
} // TreeNode::TreeNode

//***********************************************************************
// class GiniTree declaration
//***********************************************************************
class GiniTree {

	// private class variables
	TreeNode* theTree;
	DataTable trainingData;
	DataTable testData;
	TreeNode* currentNode;
	vector<WorkingData> workData;
	vector<string> classLabelSet;
	vector<string> predictedClass;
	
	// private methods
	void splitNode(TreeNode* node);
	bool isLeafNode(TreeNode* node);
	void sortSet(void);
	void resetTree(void);
	void showTree(void);
	
public:
	// public class variables

	// public methods
	GiniTree(void); // constructor
	void readTrainingData(string infile);
	void trainTree(void);
	void readTestData(string testfile);
	void testTree(string testfile);
	
}; // class GiniTree

//***********************************************************************
// class GiniTree methods
//***********************************************************************
GiniTree::GiniTree(void){

	// create the root node
	theTree = new TreeNode;
	
	return;
} // GiniTree::GiniTree

//***********************************************************************
void GiniTree::readTrainingData(string infile){
	// local variables
	bool notDone;
	string label;
	float dataValue;
	string classLabel;
	vector<float> inData;
	int attributeCount;
	int indx;
	int classLabels;
	bool labelFound;

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
			trainingData.labels.push_back(label);
			// test if we're done reading labels
			if(label[label.size()-1] == '?') notDone = false;
		} // while

		// set the attributeCount
		attributeCount = (int)trainingData.labels.size() - 1;

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
			trainingData.data.push_back(inData);
			// read the class label
			input_stream >> classLabel;

			// check if this is a new class label
			labelFound = false;
			
			// debug entry
			classLabels = (int)classLabelSet.size();
			
			for(indx = 0; indx < (int)classLabelSet.size(); indx++) {
				if(classLabel == classLabelSet[indx]) labelFound = true;
			} // for
			// if this is a new label, inset it into classLabelSet
			if(!labelFound) classLabelSet.push_back(classLabel);
			
			// debug entry
			classLabels = (int)classLabelSet.size();
			
			// save the class label
			trainingData.dataClasses.push_back(classLabel);
		} // while
	} // if
    else cout << "Error reading the input data file!" << endl << endl; // print error message

	input_stream.close();  // close filestream

	return;
} // GiniTree::readTrainingData

//***********************************************************************
void GiniTree::trainTree(void){
	// local variables
	TreeNode* currentNode;
	bool notDone;
	int indx;
	int maxIndx;

	// assign the root node to the currentNode
	currentNode = theTree;
	
	// load the training instance indexes into the root node
	//maxIndx = (int)trainingData.data.size() - 1;
	maxIndx = (int)trainingData.data.size();
	for(indx = 0; indx < maxIndx; indx++) {
		currentNode->dataPoints.push_back(indx);
	} // for

	//set the loop control flag
	notDone = true;

	while(notDone) {
		// split the currentNode
		splitNode(currentNode);
	      
		//check classes in leftChild
		if(isLeafNode(currentNode->leftChildNode)) {
			currentNode->leftChildNode->nodeType = "Leaf";
			currentNode->leftChildComplete = true;
		} // if
	      
		//check classes in rightNode
		if(isLeafNode(currentNode->rightChildNode)) {
			currentNode->rightChildNode->nodeType = "Leaf";
			currentNode->rightChildComplete = true;
		} // if
	      
		//set the new currentNode
		if(currentNode->leftChildComplete && currentNode->rightChildComplete) {
	         
			while(currentNode->leftChildComplete 
						&& currentNode->rightChildComplete 
						&& currentNode->level > 0) {

				currentNode = currentNode->parentNode;
			} // while

			if(currentNode->leftChildComplete 
						&& currentNode->rightChildComplete 
						&& currentNode->level == 0) {
				notDone = false; // stop splitting
				continue; // break to while loop
			} // if
		} // if
	      
		if(!currentNode->leftChildComplete) {
			currentNode->leftChildComplete = true;
			//go down to leftChild 
			currentNode = currentNode->leftChildNode;
			continue;
		} // if
	      
		if(currentNode->leftChildComplete 
						&& !currentNode->rightChildComplete) {
			currentNode->rightChildComplete = true;
			//go down to rightChild
			currentNode = currentNode->rightChildNode;
			continue;
		} // if
	} // while

	// output the tree to stdout
	resetTree();
	showTree();

	return;
} // GiniTree::trainTree

//***********************************************************************
void GiniTree::splitNode(TreeNode* node) {
	// local variables
	int indx;
	int attributeIndx;
	int instanceIndx;
	int classIndx;
	int readIndx;
	int leftSplit, rightSplit;
	float leftSplitRatio, rightSplitRatio;
	float sum;
	float classValue;
	float leftScore, rightScore;
	int splitAttribute;
	float splitThreshold;
	int attributeCt;
	int instanceCt;
	int classCt;
	int counter;
	float newGiniScore;
	float giniScore;
	float upperValue, lowerValue;
	WorkingData dataInstance;
	TreeNode* workNode; 

	// initialize the scoring metrics
	splitAttribute = 99;
	splitThreshold = 999;
	giniScore = 999;

	// get the attributeCt, instanceCt, classCt
	attributeCt = (int)trainingData.labels.size() - 1;
	instanceCt = (int)node->dataPoints.size();
	classCt = (int)classLabelSet.size();

	// loop for each attribute
	for(attributeIndx = 1; attributeIndx < attributeCt; attributeIndx++) {
		// clear the workData vector
		workData.clear();
		//load attribute data and classLabels into a workingData vector
		for(indx = 0; indx < instanceCt; indx++) {
			readIndx = node->dataPoints[indx];
			dataInstance.dataValue = trainingData.data[readIndx][attributeIndx];
			dataInstance.classLabel = trainingData.dataClasses[readIndx];
			workData.push_back(dataInstance);
		} // for

		//sort the workingData vector by the attribute data
		sortSet();

		// loop through every instance for this attribute
		for(instanceIndx = 1; instanceIndx < instanceCt; instanceIndx++) {

			//skip comparing attributes with the same values - no split
			if(workData[instanceIndx].dataValue != workData[instanceIndx-1].dataValue) {

				//get leftSplit
				leftSplit = instanceIndx;
				//get rightSplit
				rightSplit = instanceCt - instanceIndx;
	                
				//compute the left split ratio 
				leftSplitRatio = (float)leftSplit / (float)instanceCt;
	                        
				//initialize sum 
				sum = 0;
				// count the number of class instances in this side of the split
				for(classIndx = 0; classIndx < classCt; classIndx++) {
					counter = 0; //initialize
					for(indx = 0; indx < instanceIndx; indx++) {
						if(workData[indx].classLabel == classLabelSet[classIndx]) 
							counter++;
					} // for
					// compute class gini ratio squared
					classValue = ((float)counter/(float)leftSplit)
									*((float)counter/(float)leftSplit);
					//add to sum
					sum = sum + classValue;
				} // for
				leftScore = 1 - sum;
				leftScore = leftSplitRatio * leftScore;
	                  
				//compute the right split ratio 
				rightSplitRatio = (float)rightSplit / (float)instanceCt;
	            
				//initialize sum 
				sum = 0;
				// count the number of class instances in this side of the split
				for(classIndx = 0; classIndx < classCt; classIndx++) {
					counter = 0; //initialize
					for(indx = instanceIndx; indx < instanceCt; indx++) {
						if(workData[indx].classLabel == classLabelSet[classIndx]) 
							counter++;
					} // for
					// compute class gini ratio squared
					classValue = ((float)counter/(float)rightSplit)
									*((float)counter/(float)rightSplit);
					//add to sum
					sum = sum + classValue;
				} // for
				rightScore = 1 - sum;
				rightScore = rightSplitRatio * rightScore;
	            
				newGiniScore = leftScore + rightScore;
	            
				if(newGiniScore < giniScore) {// save the gini metrics
					giniScore = newGiniScore;
					splitAttribute = attributeIndx;
					// calculate the split threshold
					upperValue = workData[instanceIndx].dataValue;
					lowerValue = workData[instanceIndx-1].dataValue;
					splitThreshold = ((upperValue - lowerValue)/2) + lowerValue;
				} //  if
			} // if
		} // for instanceIndx
	} // for attributeIndx
	    
	//set currentNode: splitAttribute, splitThreshold
	node->splitAttribute = splitAttribute;
	node->splitThreshold = splitThreshold;
	    
	//create newNode
	workNode = new TreeNode;
	//set parent leftChild = newNode
	node->leftChildNode = workNode;
	//set leftChild parent = currentNode
	workNode->parentNode = node;
	//set leftChild level = currentNode level + 1
	workNode->level = node->level + 1;

	//create newNode
	workNode = new TreeNode;
	//set parent rightChild = newNode
	node->rightChildNode = workNode;
	//set rightChild parent = currentNode
	workNode->parentNode = node;
	//set rightChild level = currentNode level + 1
	workNode->level = node->level + 1;
	    
	instanceCt = (int)node->dataPoints.size();
	
	//for each data instance
	for(indx = 0; indx < instanceCt; indx++) {
		if(trainingData.data[node->dataPoints[indx]][splitAttribute] <= node->splitThreshold) {
			//put data instance index into leftChild
			node->leftChildNode->dataPoints.push_back(node->dataPoints[indx]);
		} // if
		else {// put data instance index into rightChild
			node->rightChildNode->dataPoints.push_back(node->dataPoints[indx]);
		} // else
	} // for
	return;
} // GiniTree::splitNode

//***********************************************************************
bool GiniTree::isLeafNode(TreeNode* node) {
	// local variables
	bool result = false;
	int dataPointCt;
	int indx;
	string baseClass;
	int loopCount = 0;

	dataPointCt = (int)node->dataPoints.size();

	if(dataPointCt == 1) result = true; // only one data point in the node
	else {
		// get the class of the first data point in the node
		baseClass = trainingData.dataClasses[node->dataPoints[0]];
		loopCount = 1; // save the count of this class

		for(indx = 1; indx < dataPointCt; indx++) {
			//compare every data point class to the base class
			if(baseClass == trainingData.dataClasses[node->dataPoints[indx]])
					loopCount++; // count the matches
		} // for

		// all of the data points belong to the same class
		if((loopCount) == dataPointCt) result = true;
		// else the data points dona' belong to the same class
		else result = false; 

	} // else

	return result;
} // GiniTree::isLeafNode

//***********************************************************************
void GiniTree::sortSet(void) {
	// local variables
	bool notDone;
	int indx;
	int maxIndx;
	int smallIndx;
	vector<WorkingData>::iterator pos;
	vector<WorkingData>::iterator posMin;
	vector<WorkingData> sortedData;

	notDone = true;

	while(notDone) {
		// initialize the iterators
		pos = workData.begin();
		posMin = pos;

		// get the maxIndx
		maxIndx = (int)workData.size();
		smallIndx = 0;

		for(indx = 1; indx < maxIndx; indx++) {
			pos++;
			if(workData[indx].dataValue < workData[smallIndx].dataValue) {
				smallIndx = indx;
				posMin = pos;
			} // if
		} // for

		// save the min value
		sortedData.push_back(workData[smallIndx]);
		workData.erase(posMin);

		// set stopping flag
		if((int)workData.size() == 0) notDone = false;
	} // while

	maxIndx = (int)sortedData.size();

	for(indx = 0; indx < maxIndx; indx++) {
		workData.push_back(sortedData[indx]);
	} // for
	return;
}// GiniTree::sortSet

//***********************************************************************
void GiniTree::resetTree(void) {
	// local variables
	bool notDone;
	TreeNode* currentNode;

	// set the currentNode
	currentNode = theTree;

	//set the loop control flag
	notDone = true;

	while(notDone) {
	      
		//check classes in leftChild
		if(isLeafNode(currentNode->leftChildNode)) {
			currentNode->leftChildNode->nodeType = "Leaf";
			currentNode->leftChildComplete = false;
		} // if
	      
		//check classes in rightNode
		if(isLeafNode(currentNode->rightChildNode)) {
			currentNode->rightChildNode->nodeType = "Leaf";
			currentNode->rightChildComplete = false;
		} // if
	      
		//set the new currentNode
		if(!currentNode->leftChildComplete && !currentNode->rightChildComplete) {
	         
			while(!currentNode->leftChildComplete 
						&& !currentNode->rightChildComplete 
						&& currentNode->level > 0) {

				currentNode = currentNode->parentNode;
			} // while

			if(!currentNode->leftChildComplete 
						&& !currentNode->rightChildComplete 
						&& currentNode->level == 0) {
				notDone = false; // stop splitting
				continue; // break to while loop
			} // if
		} // if
	      
		if(currentNode->leftChildComplete) {
			currentNode->leftChildComplete = false;
			//go down to leftChild 
			currentNode = currentNode->leftChildNode;
			continue;
		} // if
	      
		if(!currentNode->leftChildComplete 
						&& currentNode->rightChildComplete) {
			currentNode->rightChildComplete = false;
			//go down to rightChild
			currentNode = currentNode->rightChildNode;
			continue;
		} // if
	} // while
	return;
} // GiniTree::resetTree

//***********************************************************************
void GiniTree::showTree(void) {
	// local variables
	TreeNode* currentNode;
	bool notDone;
	int indx, indx2;

	// assign the root node to the currentNode
	currentNode = theTree;
	
	//set the loop control flag
	notDone = true;

	while(notDone) {
		// output the currentNode
		if((currentNode->nodeType == "Node") && (!currentNode->leftChildComplete)) {
			// insert spaces for the tree level
			for(indx = 0; indx < currentNode->level; indx++) {
				cout << " ";
			} // for
			// output the split attribute label
			cout << trainingData.labels[currentNode->splitAttribute];
			// output the split threshold
			cout << " <= " << currentNode->splitThreshold; 
			// ouput the number of points at this node
			cout << " Data = " << (int)currentNode->dataPoints.size();
			cout << " : ";
			// output the instance ID
			for(indx2 = 0; indx2 < ((int)currentNode->dataPoints.size() - 1); indx2++) {
				cout << trainingData.data[currentNode->dataPoints[indx2]][0] << " ";
			} // for
			cout << endl;
		} // if

		if(currentNode->nodeType == "Leaf") {
			// insert spaces for the tree level
			for(indx = 0; indx < currentNode->level; indx++) {
				cout << " ";
			} // for
			// output the leaf class label
			cout << trainingData.dataClasses[currentNode->dataPoints[0]];
			// output the count of instances in the leaf
			cout << " : " << (int)currentNode->dataPoints.size();
			cout << endl;
		} // if

		// check if currentNode is a Leaf
		if(currentNode->nodeType == "Leaf") {
			// go up to parentNode
			currentNode = currentNode->parentNode;
		} // if

		if(!currentNode->leftChildComplete) {
			currentNode->leftChildComplete = true;
			//go down to leftChild 
			currentNode = currentNode->leftChildNode;
			continue;
		} // if
	      
		if(currentNode->leftChildComplete 
						&& !currentNode->rightChildComplete) {
			currentNode->rightChildComplete = true;
			//go down to rightChild
			currentNode = currentNode->rightChildNode;
			continue;
		} // if

		//set the new currentNode
		if(currentNode->leftChildComplete && currentNode->rightChildComplete) {
	         
			while(currentNode->leftChildComplete 
						&& currentNode->rightChildComplete 
						&& currentNode->level > 0) {

				currentNode = currentNode->parentNode;
			} // while

			if(currentNode->leftChildComplete 
						&& currentNode->rightChildComplete 
						&& currentNode->level == 0) {
				notDone = false; // stop splitting
				continue; // break to while loop
			} // if
		} // if
	} // while
	cout << endl << endl << endl;
	// output the tree to stdout
	resetTree();

	return;
} // GiniTree::showTree

//***********************************************************************
void GiniTree::readTestData(string testfile) {
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
	input_stream.open(testfile.c_str());

    // check if the file was OK
    if (input_stream.is_open()){

        // read the data labels
		notDone = true; // set the control flag
		while(notDone){
			// read a label from the input_stream
			input_stream >> label;
			// save the label
			testData.labels.push_back(label);
			// test if we're done reading labels
			if(label[label.size()-1] == '?') notDone = false;
		} // while

		attributeCount = (int)testData.labels.size() - 1;

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
			testData.data.push_back(inData);
			
			// read the class label
			input_stream >> classLabel;
			// save the class label
			testData.dataClasses.push_back(classLabel);
		} // while
	} // if
    else cout << "Error reading the input data file!" << endl << endl; // print error message

	input_stream.close();  // close filestream

	return;
} // GiniTree::readTestData

//***********************************************************************
void GiniTree::testTree(string testfile) {
	// local variables
	int indx, indx2;
	int maxIndx, maxIndx2;
	int attributeIndx;
	float splitThreshold;
	string outfile;
	TreeNode* currentNode;
	bool notDone;
	int classIndx;
	string predClass;
	int instanceCt;
	int correctCt;

	// clear the predicted class storage
	predictedClass.clear();

	// get the maximum number of instances
	maxIndx = (int)testData.data.size();
	for(indx = 0; indx < maxIndx; indx++) {

		// initialize the currentNode
		currentNode = theTree;

		notDone = true;
		while(notDone) {

			if(currentNode->nodeType == "Leaf") {
				// find the node class
				classIndx = currentNode->dataPoints[0];
				predClass = trainingData.dataClasses[classIndx];
				// save the predicted class
				predictedClass.push_back(predClass);
				notDone = false;
			} // if

			//baseClass = trainingData.dataClasses[node->dataPoints[0]];

			if(currentNode->nodeType == "Node") {
				// get the split attribute index
				attributeIndx = currentNode->splitAttribute;
				// get the split threshold
				splitThreshold = currentNode->splitThreshold;
				// test the data and go down a level
				if(testData.data[indx][attributeIndx] <= splitThreshold) {
					// go to the left node
					currentNode = currentNode->leftChildNode;
					//currentNode = currentNode->rightChildNode;
				} // if
				else { // go to the right node
					currentNode = currentNode->rightChildNode;
					//currentNode = currentNode->leftChildNode;
				} // else
			} // if
		} // while
	} // for
	
	// modify the filename
	outfile = testfile + ".out";
	//Open a file stream for writing
	// declare an output stream 
	ofstream results_stream;
	// open the stream to write the output plaintext
	results_stream.open(outfile.c_str());

	// count the correct classificatons
	correctCt = 0;
	instanceCt = (int)testData.data.size() - 1;
	for(indx = 0; indx < instanceCt; indx++) {
		if(testData.dataClasses[indx] == predictedClass[indx]) {
			correctCt++;
		} // if
	} // for

	// output the test accuracy
	results_stream << "This test session tested ";
	results_stream << instanceCt;
	results_stream << " test data instances" << endl;

	results_stream << correctCt << " were classified corectly" << endl;
	results_stream << instanceCt - correctCt << " were classified incorrectly" << endl;
	results_stream << "for an accuracy of ";
	results_stream << ((float)correctCt / (float)instanceCt) * 100 << "%" << endl << endl;
	
	//output the data header
	maxIndx = (int)testData.labels.size();
	for(indx = 0; indx < maxIndx; indx++) {
		results_stream << testData.labels[indx] << " ";
	} // for
	// add a new column for the predicted values
	results_stream << "predicted" << endl;

	maxIndx = (int)testData.data.size() - 1;
	for(indx = 0; indx < maxIndx; indx++) { // for each instance
		maxIndx2 = (int)testData.data[indx].size();
		// output the data
		for(indx2 = 0; indx2 < maxIndx2; indx2++) { // for each attribute
			// output data
			results_stream << testData.data[indx][indx2] << " ";
		} // for
		// add the instance class
		results_stream << testData.dataClasses[indx] << " ";
		// add the instance predicted class
		predClass = predictedClass[indx];
		results_stream << predictedClass[indx] << endl;
	} // for
	results_stream.close();

	return;
} // GiniTree::testTree

//***********************************************************************


