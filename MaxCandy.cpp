#include "MaxCandy.hpp"

int main(int argc, char **argv){
  struct Environ env;

  // Preprocess the Input
  if (argc == 1)
    preprocess(env, (char *)"input.txt");
  else if (argc == 2)
    preprocess(env, argv[1]);
  else
    throw std::runtime_error("Format: Executable (path_to_file){0,1}");

  // Find the Result
  struct Result result = find_seq(env);

  // Print the Output
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


// O(num_homes) Serial Version using Two-pointer approach
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
    std::vector<uint> temp = env.pieces;
    prefix_sum(temp, env);
    return binary_search_omp(temp, env);
}
#endif
