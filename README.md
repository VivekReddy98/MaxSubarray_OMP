# Trick or Treat

## Algorithm

1) Serial  Version is implemented using Two Pointer approach and is of O(N) time complexity.
2) OpenMP: Was not able to parallelize Two-pointer approach. Hence used another version of complexity O(NlogN).
           It includes a Prefix Scan (Cummulative sum) of the array followed by a
           binary search on every element(right index) to find best possible left index.

## Build Configurations (Serial Version)

      make

## Build Configurations (OpenMP Version)

      make paralell

## Program Execution:

       ./MaxCandy.out {path to the txt file}
                       or
       ./MaxCandy.out (Note: It'll read from input.txt in the same directory)

## Folder Structure:

/
├── MaxCandy.hpp (OMP Helper functions and Declarations)
├── MaxCandy.cpp
├── Makefile
├── test (Files used for testing)
├── test_script.sh (Script Used for Testing, 'bash test_script.sh')
├── input.txt (Test File as described)
