#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>

#ifdef _OPENMP
#include "omp.h"
#endif
// If not specified, Set threads to Number of Cores
#ifndef OMP_NUM_THREADS
#define OMP_NUM_THREADS omp_get_num_procs()
#endif

typedef unsigned int uint;

// POD Structures For Output
struct Result{
  int left = -1; // left index
  int right = -1; // Right Index
  uint num_candies = 0; // Num Candies
};

// POD Structures For Input
struct Environ{
  std::vector<uint> pieces = std::vector<uint>{};
  size_t num_homes = 0; //
  size_t max_cnds = 0;
};


// Just a Wrapper Method
Result find_seq(const Environ& );
// Serial Version
Result find_seqSerial(const Environ& );
// OpenMP Version
Result find_seqOMP(const Environ& );
// Preprocess input
void preprocess(Environ& , const char *);
void print(const Result& ); // Print output

// OMP Helper Function to find Prefix_sum in Paralell
void prefix_sum(std::vector<uint>&, const Environ&);
// OMP Helper Function to perform a binary search in Paralell
Result binary_search_omp(std::vector<uint>&, const Environ&);

inline void search_helper(const std::vector<uint>& prefix_sums, Result& ans, const Environ& env);


// ----------------------------------  Helper Function Definitions ----------------------------------------

// Read the Input file into the Environ Structure
void preprocess(Environ& env, const char *filename){
  std::ifstream inp;
  try{
    inp.open(filename);
    if (!inp.good()) throw std::invalid_argument("File not Found");
    inp >> env.num_homes;
    inp >> env.max_cnds;
    env.pieces.resize(env.num_homes);
    env.pieces.shrink_to_fit();
    size_t i = 0;
    while(i < env.num_homes){
      if (!inp.eof()) inp >> env.pieces[i];
      else{
        throw std::invalid_argument("Number of Homes is Greater than expected");
      }
      ++i;
    }
    inp.close();
  }
  catch(const std::exception& e){
    if (inp.is_open()) inp.close();
    std::cout << e.what() << " / Format of the input file is not in expected order " << std::endl;
    throw e;
  }
}

// Helper Print Function
void print(const Result& result){
    if (result.right == -1 || result.left == -1){
       std::cout << "Don't go here" << std::endl;
    }
    else {
       printf("Start at home %u and go to home %u getting %u pieces of candy\n", result.left+1, result.right+1, result.num_candies);
    }
}

#ifdef _OPENMP

void prefix_sum(std::vector<uint>& temp, const Environ& env){
    for (size_t i = 1; i < env.num_homes; i++){
        temp[i] = temp[i-1] + env.pieces[i];
    }
    /*
    Simple Prefix Scan in Paralell in O((num_homes/NumThreads)*log(num_homes)) complexity
    O(num_homes) space complexity -- This Version isn't efficient, so scrapped
        size_t numthreads = OMP_NUM_THREADS;
        std::vector<uint> read_only(env.num_homes, 0);

        for (size_t stride = 1; stride <= env.num_homes; stride*=2) {
            // Maintain a Temp Array to remove intra-array dependency
            #pragma omp parallel for shared(env, stride, temp) num_threads(numthreads)
            for (size_t i = 0; i < env.num_homes; i++){
                read_only[i] = temp[i];
            }

            #pragma omp parallel for shared(env, stride, temp) num_threads(numthreads)
            for (size_t i = stride; i < env.num_homes; i++) {
                temp[i] += read_only[i-stride];
            }
        }
    */
}

/*
Run a binary search on every element (in paralell) to find a potetial subarray ending with that element
*/
Result binary_search_omp(std::vector<uint>& prefix_sums, const Environ& env){
    struct Result result;
    size_t numthreads = OMP_NUM_THREADS;

    #pragma omp parallel for num_threads(numthreads)
    for (size_t i = 0; i < env.num_homes; i++){
        struct Result thread_priv{0, (int)i, prefix_sums[i]};

        // If the value in the piece itself is greater than max limit, no point doing anything
        if (env.pieces[i] > env.max_cnds){
            continue;
        }
        // If the Prefix_sum ending at index i is > max limit, try checking for a precence of a subarray.
        else if (prefix_sums[i] > env.max_cnds){
            search_helper(prefix_sums, thread_priv, env);
        }
        // Update the Global Result
        if (result.num_candies < thread_priv.num_candies ||
           (result.num_candies == thread_priv.num_candies
              && result.left > thread_priv.left && result.left != -1)){
            // Critical Section and hence require synchronization
            #pragma omp critical
            {
                result = thread_priv;
            }
        }
    }
    return result;
}

// Standard Binary Search.
// Right Index is fixed, Target is to search for the left most index for which the sum[left...right] <= max limit
inline void search_helper(const std::vector<uint>& prefix_sums, Result& ans, const Environ& env) {
    int temp_left = ans.left;
    int temp_right = ans.right;
    int optim_left = -1, optim_val = 0;
    while(temp_left <= temp_right){
        int mid = temp_left + (temp_right - temp_left)/2;
        uint sum_for_this_subarray = ans.num_candies;
        if (mid > 0)  sum_for_this_subarray -= prefix_sums[mid-1];
        if (sum_for_this_subarray > env.max_cnds){
            temp_left = mid + 1;
        }
        else if (sum_for_this_subarray < env.max_cnds){
            temp_right = mid - 1;
            optim_val = sum_for_this_subarray;
            optim_left = mid;
        }
        else{
            optim_val = sum_for_this_subarray;
            optim_left = mid;
            break;
        }
    }
    ans.left = optim_left;
    ans.num_candies = optim_val;
}
#endif
