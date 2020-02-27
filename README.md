Question 1 - QuickSort using MPI

Brief -
 Suppose, There are N no of processes to run on some given input array. The first process will find the parition the array based on selected pivot and generate 2 sub-array left and right. The left partion it will keep and based on availability of other processes will give the other partition to some other process. This will continue then the final result is sent back at the first process.

Analyis -
- The running time complexity of algorithm is nlogn.
- When the number of processes is kept fixed, the running time of the process increased when the size of input is increased.
- When the size of input is kept fixed, the running time of the process increased when the number of processes is increased as each process is taking less time by running the quicksort algorithm on each sub arrays but at last the final process is merging all the subparts so the greater the subarrays to be merged the greater the time taken to merge.

Instruction for Run the code:

mpic++ 2018201019_1.cpp -o 2018201019_1
mpirun -np #NP ./2018201019_1 inputfile outputfile

INPUT: 
Array of elements : 1 9 28 4 58 29 1 5 3

Observation:

No of elements :  100000

$ mpirun -np 1 ./a.out test1.txt out.txt
Total time (s): 0.439704
$ mpirun -np 3 ./a.out test1.txt out.txt
Total time (s): 0.365743
$ mpirun -np 10 ./a.out test1.txt out.txt
Total time (s): 0.780569
$ mpirun -np 15 ./a.out test1.txt out.txt
Total time (s): 0.996206
$ mpirun -np 20 ./a.out test1.txt out.txt
Total time (s): 1.163541


Question 2 - Single Source Shortest Path (Dijkstra's)

Brief -
- Every process will find local minimum distance of some set of vertices from source vertex.
- Then will find global minimum distance of each vertex from source vertex.

Analyis -
- for varying input -
  When the number of processes is kept fixed, the running time of the process increased when the size of input is increased.

Instruction for Run the code:

mpic++ 2018201019_2.cpp -o 2018201019_2
mpirun -np #NP ./2018201019_2 inputfile outputfile

INPUT: 
4 5
1 2 1
1 3 1 
1 4 2
2 3 4
3 4 2
1

Observation:
No of nodes : 5

$ mpirun -np 2 ./a.out Input/q2_input out.txt
Total time (s): 0.004517
$ mpirun -np 3 ./a.out Input/q2_input out.txt
Total time (s): 0.215743
$ mpirun -np 4 ./a.out Input/q2_input out.txt
Total time (s): 0.234638
$ mpirun -np 5 ./a.out Input/q2_input out.txt
Total time (s): 0.302795

