#include <iostream>
#include <omp.h>

#define NUM_THREADS 2

int main(int argc, char* argv[]) {

  int num_steps = std::stoi(argv[1]);

  int nthreads;
  double pi;
  double step = 1.0 / (double)num_steps;

  omp_set_num_threads(NUM_THREADS);

#pragma omp parallel
  {
    int i, id, nthrds;
    double x;
    double sum; // now each thread has its own sum variable
    id     = omp_get_thread_num();
    nthrds = omp_get_num_threads();
    if (id == 0) {
      // to check the actual number of threads that we get
      // becasue the program can give us less threads that we ask for
      nthreads = nthrds;
    }

    for (i = id, sum = 0.0; i < num_steps; i = i + nthreads) {
      x    = (i + 0.5) * step;
      sum += 4.0 / (1.0 + x * x);
    }

    // now only one thread will modify the pi variable
#pragma omp critical
    pi += sum * step;
  }

  std::cout << "Our value is: " << pi << std::endl;
}
