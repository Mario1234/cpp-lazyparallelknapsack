# cpp-lazyparallelknapsack
C++ solution to the 0/1 Knapsack problem with lazy evaluation, this is a parallel execution of 4 Knapsack problems simultaneously.

This code pretends to show that C++ is more efficient than Scala when resolving difficult problems where lazy evaluation matters.
Those problems are usually solved with functional languages because that kind of languages have implicit lazy evaluation, and 
also have easier and more efficient way to implement parallel execution. However, I don't think so and that's why I coded this project,
I coded it to show people how easy is solving difficult problems with C++.

If you execute [this solution](https://rosettacode.org/wiki/Knapsack_problem/0-1#C.2B.2B) with maximumWeight=10000 you will get
stackoverflow error, but in this project I avoid that error using lazy evaluation.
