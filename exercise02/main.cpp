#include <iostream>
#include <omp.h>

#define NUM_THREADS 2

int main(int argc, char* argv[])
{

    int num_steps = std::stoi(argv[1]);

    int i, nthreads;
    double pi, sum[NUM_THREADS];
    double step = 1.0 / (double)num_steps;

    omp_set_num_threads(NUM_THREADS);

#pragma omp parallel
    {
        int i, id, nthrds;
        double x;
        id = omp_get_thread_num();
        nthrds = omp_get_num_threads();
        if (id == 0)
        {
            // to check the actual number of threads that we get
            // becasue the program can give us less threads that we ask for
            nthreads = nthrds;
        }

        // each trhead executes certain iterations.
        // for instance thread 0 executes iteration 0 and iteration nthrds, ...
        for (i = id, sum[id] = 0.0; i < num_steps; i = i + nthrds)
        {
            x = (i + 0.5) * step;
            sum[id] += 4.0 / (1.0 + x * x);
        }
    }

    for (i = 0, pi = 0.0; i < nthreads; i++)
    {
        pi += sum[i] * step;
    }

    std::cout << "Our value is: " << pi << std::endl;
}
