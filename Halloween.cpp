#include <iostream>
#include <vector>
#include <fstream>

#ifdef _OPENMP
#include "omp.h"
#endif

struct Result{
  int left = -1;
  int right = -1;
  unsigned int num_candies = 0;
};

struct Environ{
  std::vector<unsigned int> pieces = std::vector<unsigned int>{};
  size_t num_homes = 0;
  size_t max_cnds = 0;
};

Result find_seq(const Environ& );
Result find_seqSerial(const Environ& );
Result find_seqOMP(const Environ& );
void preprocess(Environ& , const char *);
void print(const Result& );

int main(int argc, char **argv){
  struct Environ env;
  if (argc != 2)
    preprocess(env, (char *)"input.txt");
  else
    preprocess(env, argv[1]);

  struct Result result = find_seq(env);
  print(result);
  return 0;
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


Result find_seqSerial(const Environ& env){
    struct Result result;
    int ptr2 = 0, ptr1 = 0;
    unsigned int temp_sum = 0;
    for (; ptr2 < (int)env.num_homes; ++ptr2){
        temp_sum += env.pieces[ptr2];
        if (temp_sum > result.num_candies && temp_sum <= env.max_cnds){
           result.right = ptr2;
           result.left = ptr1;
           result.num_candies = temp_sum;
        }
        else if(temp_sum > env.max_cnds){
          while(temp_sum > env.max_cnds && ptr1 <= ptr2){
             temp_sum -= env.pieces[ptr1];
             ptr1++;
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

Result find_seqOMP(const Environ& env){
    struct Result result;

    return result;
}

Result find_seq(const Environ& env){
#ifdef _OPENMP
    return find_seqSerial(env);
#else
    return find_seqSerial(env);
#endif
}

void print(const Result& result){
    if (result.left == result.right && result.left == -1){
       std::cout << "Don't go here" << std::endl;
    }
    else {
       printf("Start at home %u and go to home %u getting %u pieces of candy\n", result.left+1, result.right+1, result.num_candies);
    }
}
