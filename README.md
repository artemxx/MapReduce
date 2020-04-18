# MapReduce
MapReduce is a processing technique and a program model for distributed computing based on C++. The MapReduce algorithm contains two important tasks, namely Map and Reduce. Map takes a set of data and converts it into another set of data, where individual elements are broken down into tuples (key/value pairs).

Map operation appends to each word the value of its quantity equal to 1.

Reduce operation takes a list of pairs <word, quantity> and sums up the quantity, leaving only one unique word.

When MapReduce is started with the "map" flag, the map is executed.

When MapReduce is started with the "reduce" flag, the input data is sorted by key and divided into groups of identical keys. On each such group, the reduce script is run, each writes the result to its own file, at the end the files are combined.

To use, you need to run the following set of commands:

g++ map.cpp -o map

g++ reduce.cpp -o reduce

g++ mapreduce.cpp -o mapreduce -lboost_system

./mapreduce reduce ./reduce results/mapped.txt results/result.txt

./mapreduce map ./map data/words.txt results/mapped.txt

rm -rf *
