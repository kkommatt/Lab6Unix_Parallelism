# Lab6Unix_Parallelism
Critical Section. Race Condition. Atomic Operations

Use OS mechanisms to parallelize computing and organize interaction with the critical section.
 
1. Multiply the matrices A[n x m] and B[m x k] (automatically filled). For the calculation, create 1..n*k threads of the same type for calculations (multiplication of vectors or a set of vectors).
1.1. (2 points) Demonstrate the parallelism (non sequentiality) of calculations by displaying the result (triples [x,y]=result) "in the course of calculations".
1.2.* (+3 points) Investigate the performance of A*B depending on the number of threads for multiplication parallelization. Demonstrate and explain this dependence. With what number of threads will multiplication be the fastest? Confirm experimentally.
 
2. Model the parallel operation of two threads with a shared memory cell (shared variable):
a) using a critical segment (or atomic, mutex, lock, etc.)
b) without using a critical segment. 
2.1. (2 marks) Demonstrate the difference in the results of calculations in these two cases. (For example, increase the value of a shared cell by 1: v=v+1; 10^9 times in each thread and see the resulting value of v.)
2.2. (+1 mark) Analyze the time difference between different implementation options and explain why it is sometimes possible to obtain an incorrect final result (race condition).
2.3. (+1 mark) Try to achieve the fastest possible result while maintaining the correctness of the calculations (correct final value).
2.3.* (+3 points) Achieve a variant when such parallel addition is performed completely synchronously, that is, for example, 1000 additions are performed in parallel by two threads step by step and increase the value of a common variable from 0 to 1000. That is, not only does each of the two parallel threads increment the value from 0 to 1000, but both threads running in parallel also increment from 0 to 1000 (and not to 2000, as expected).
