#include <iostream>
#include <vector>
#include <fstream>

#ifdef _OPENMP
#include "omp.h"
#endif

struct Result{
  int left = -1;
  int right = -1;
};

struct Environ{
  std::vector<int> pieces = std::vector<int>{};
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

  std::cout << env.num_homes << std::endl;
  std::cout << env.max_cnds << std::endl;
  for (int i = 0; i < env.num_homes; i++){
    std::cout << env.pieces[i] << std::endl;
  }

  // struct Result result = find_seq(env);
  // print(result);
  return 0;
}

void preprocess(Environ& env, const char *filename){
  std::ifstream inp;
  try{
    inp.open(filename);
    if (!inp.good()) throw "File not Found";
    inp >> env.num_homes;
    inp >> env.max_cnds;
    env.pieces.resize(env.num_homes);
    env.pieces.shrink_to_fit();
    size_t i = 0;
    while(i < env.num_homes){
      inp >> env.pieces[i];
      ++i;
    }
    inp.close();
  }
  catch(const std::exception& e){
    if (inp.is_open()) inp.close();
    throw std::runtime_error("Format of the input file is not in expected order / Problem in reading the file");
  }
}

// Result find_seq(const Environ& env){
// #ifdef _OPENMP
//     return find_seqSerial(env);
// #else
//     return find_seqSerial(env);
// #endif
// }
