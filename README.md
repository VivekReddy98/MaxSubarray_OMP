# Trick or Treat

## Problem Statement: 

Find the Maximum Subarray under the constraint of sum(arr[i...j]) <= max. 

## Algorithm

1) Serial  Version is implemented using Two Pointer approach and is of O(N) time complexity.
2) OpenMP: Used another variant of complexity O(NlogN), which was suitable for parellelization
           It includes a Prefix Scan (Cummulative sum) of the array followed by a
           binary search on every element(right index) to find best possible left index.

## Logistics

1) Programming Language Used: C++1x
2) OS: Linux
3) Compiler: GCC 7.5
4) OMP Version: 4.5

## Build Configurations (Serial Version)

      make

## Build Configurations (OpenMP Version)

      make paralell

## Program Execution:

       ./MaxCandy.out {path to the txt file}
                       or
       ./MaxCandy.out (Note: It'll read from input.txt in the same directory, if no argument is specfied)

## Folder Structure:
```
/
├── MaxCandy.hpp (OpenMP Helper functions and other Declarations)
├── MaxCandy.cpp
├── Makefile
├── test (Files used for testing)
├── test_script.sh (Script Used for Testing, 'bash test_script.sh')
├── input.txt (Test File as described)
```
