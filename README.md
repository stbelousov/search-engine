# Description
A full-text search engine implementation with the Okapi BM25 ranking function.

University of Otago, COSC431, Assignment 1.  
Author: Stepan Belousov.

# Setup for Linux
Requirements:
* cmake (tested with 3.13)
* python3 (tested with 3.7.3)
* C++ compiler with C++11 support (tested with GCC 8.3.0)

1\. Building:  
$ git clone https://github.com/stbelousov/search-engine.git  
$ mkdir search-engine/build  
$ cd search-engine/build  
$ cmake ..  
$ make -j4  
$ cd bin

2\. Running the parser:  
$ ./parser.py < ../../data/example.xml > index.txt

3\. Running the indexer:  
$ ./indexer index.bin < index.txt

4\. Running the search engine:  
$ ./searcher index.bin
