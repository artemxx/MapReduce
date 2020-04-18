# MapReduce
MapReduce is a processing technique and a program model for distributed computing based on C++. The MapReduce algorithm contains two important tasks, namely Map and Reduce. Map takes a set of data and converts it into another set of data, where individual elements are broken down into tuples (key/value pairs).

Here is the implementation code for this technology for the word counting task.

**Map** operation appends to each word the value of its quantity equal to 1.

**Reduce** operation takes a list of pairs *<word, quantity>* and sums up the quantity, leaving only one unique word.

When **MapReduce** is started with the *"map"* flag, the map is executed.

When **MapReduce** is started with the *"reduce"* flag, the input data is sorted by key and divided into groups of identical keys. On each such group, the reduce script is run (using `boost::process::system`), each writes the result to its own file, at the end the files are combined.

The *mapreduce* script works as follows:
```
./mapreduce type script src_file, dst_file
```
Where `type` is a *map* or *reduce*, and `script`, `src_file`, `dst_file` are the paths to the script to be run, input and output data, respectively.

To get data from the *results* folder, I used the following set of commands:

```
g++ map.cpp -o map
g++ reduce.cpp -o reduce
g++ mapreduce.cpp -o mapreduce -lboost_system

./mapreduce map ./map data/words.txt results/mapped.txt
./mapreduce reduce ./reduce results/mapped.txt results/result.txt

rm -rf /
```
