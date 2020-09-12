#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <math.h>

#ifdef _OPENMP
#include "omp.h"
#endif


// If not specified Set threads to Number of Cores
#ifndef OMP_NUM_THREADS
#define OMP_NUM_THREADS omp_get_num_procs()
#endif

typedef unsigned int uint;

// POD Structures For Output
struct Result{
  int left = -1;
  int right = -1;
  uint num_candies = 0;
};

// POD Structures For Input
struct Environ{
  std::vector<uint> pieces = std::vector<uint>{};
  size_t num_homes = 0;
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
Result binary_search(std::vector<uint>&, const Environ&);


int main(int argc, char **argv){
  struct Environ env;
  if (argc == 1)
    preprocess(env, (char *)"input.txt");
  else if (argc == 2)
    preprocess(env, argv[1]);
  else
    throw std::runtime_error("Format: Executable (path_to_file){0,1}");

  struct Result result = find_seq(env);
  print(result);
  return 0;
}

Result find_seq(const Environ& env){
#ifdef _OPENMP
    return find_seqOMP(env);
#else
    return find_seqSerial(env);
#endif
}

// Helper Print Function
void print(const Result& result){
    if (result.left == result.right && result.left == -1){
       std::cout << "Don't go here" << std::endl;
    }
    else {
       printf("Start at home %u and go to home %u getting %u pieces of candy\n", result.left+1, result.right+1, result.num_candies);
    }
}

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
#ifdef DEBUG
  std::cout << env.num_homes << std::endl;
  std::cout << env.max_cnds << std::endl;
  for (size_t i = 0; i < env.num_homes; i++){
    std::cout << env.pieces[i] << std::endl;
  }
#endif
}

/* O(num_homes) Serial Version using Two-pointer approach */
Result find_seqSerial(const Environ& env){
    struct Result result;
    int ptr2 = 0, ptr1 = 0;
    uint temp_sum = 0;
    for (; ptr2 < (int)env.num_homes; ++ptr2){
        // Keep Track of Temporary Sum
        temp_sum += env.pieces[ptr2];
        // If temp_sum is (strictly) greater than what was previously
        //                seen and is less than the upper limit
        if (temp_sum > result.num_candies && temp_sum <= env.max_cnds){
           result.right = ptr2;
           result.left = ptr1;
           result.num_candies = temp_sum;
        }
        // In Case the Temp Sum crosses the acceptable boundary,
        //               Increment the first pointer, till it is inside the boundary
        else if(temp_sum > env.max_cnds){
          while(temp_sum > env.max_cnds && ptr1 <= ptr2){
             temp_sum -= env.pieces[ptr1];
             ++ptr1;
             if (temp_sum > result.num_candies && temp_sum <= env.max_cnds){
               result.right = ptr2;
               result.left = ptr1;
               result.num_candies = temp_sum;
             }
          }
        }
    }
    return result;
}

#ifdef _OPENMP
/* O(num_homes*log(num_homes)) version parallelized*/
Result find_seqOMP(const Environ& env){
    struct Result result;
    // std::vector<uint> temp(env.num_homes, 0);
    std::vector<uint> temp = env.pieces;
    prefix_sum(temp, env);
    return binary_search(temp, env);
}

void prefix_sum(std::vector<uint>& temp, const Environ& env){
    size_t numthreads = OMP_NUM_THREADS;

    std::vector<uint> read_only(env.num_homes, 0);
    for (int stride = 1; stride <= env.num_homes; stride*=2) {

        // Maintain a Temp Array to remove intra-array dependency
        #pragma omp parallel for shared(env, stride, temp) num_threads(numthreads)
        for (int i = 0; i < env.num_homes; i++){
            read_only[i] = temp[i];
        }

        #pragma omp parallel for shared(env, stride, temp) num_threads(numthreads)
        for (int i = stride; i <= env.num_homes; i++) {
            temp[i] += read_only[i-stride];
        }
    }

    #ifdef DEBUG
      for (size_t i = 0; i < env.num_homes; i++){
        std::cout << temp[i] << " ";
      }
      std::cout << std::endl;
    #endif
}

Result binary_search(std::vector<uint>&, const Environ&){
    struct Result result;
    return result;
}

#endif
