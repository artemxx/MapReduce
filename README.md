# MapReduce
MapReduce is a processing technique and a program model for distributed computing based on C++. The MapReduce algorithm contains two important tasks, namely Map and Reduce. Map takes a set of data and converts it into another set of data, where individual elements are broken down into tuples (key/value pairs).

g++ map.cpp -o map
g++ reduce.cpp -o reduce
g++ mapreduce.cpp -o mapreduce -lboost_system
./mapreduce reduce ./reduce results/mapped.txt results/result.txt
./mapreduce map ./map data/words.txt results/mapped.txt
rm -rf *
