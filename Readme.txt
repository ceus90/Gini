Readme file for giniTree and giniNtest

Author: j aleshunas
e-mail: jalesh@webster.edu

Overview:

This application consists of three files:
     
     giniTree.cpp
     giniTree.hpp
     giniNtest.cpp

giniTree.cpp and giniTree.hpp will train a decision tree using the gini index 
to determine split attributes and split points. The application requires a 
training data filename and will accept an optional test data filename.

These files are text files and the format for the files is:
      
Datum Age Income Height Weight YearsAtEmployment YearsAtAddress B::Male Loan?
1     23  12500    60    125         2                  1          1     No
2     47  45900    67    189        12                  7          0     Yes

The datum and final class column are required and the class label must end in 
a ? Any number of data attributes can be included.

Compiling:

The applications can be compiled by invoking make. This produces two executable
files: giniTree and giniNtest.

Running the Applications [giniTree]:

giniTree can be invoked independently with the command:
         
         giniTree trainingFile [testFile]
         
The trainingFile is required. The testFile is optional.

When giniTree is run it will print a text-based model ot the decision tree 
to the screen. If a trainingFile is provided, giniTree will test the data in 
the tree, prnting the number of instances tested, the number of correct 
class perdictions, the number of incorrect predictions, and the classification
accuracy expressed as a percentage. The results of the test are saved in a file 
with the same name as the test data but with a .out file extension.

Running the Applications [giniNtest]:

giniNtest is a wrapper application that conducts n-fold training and testing 
on giniTree. You can invoke GiniNtest with the command:
             
             giniNtest trainingFile #

Where trainingFile is the name of the training data file and # is the number of
n-fold tests you wish to run. Since giniNtest calls giniTree n times, its results
are n screen outputs of the trained trees and n output result [.out files] files.
er

