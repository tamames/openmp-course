#include <iostream>
#include <omp.h>

#define NUM_THREADS 2

int main(int argc, char *argv[])
{

    int num_steps = std::stoi(argv[1]);

    int i;
    double sum = 0.0;
    double step = 1.0 / (double)num_steps;

    omp_set_num_threads(NUM_THREADS);

#pragma omp parallel
    {
        double x;
#pragma omp for reduction(+:sum)
        for (i = 0; i < num_steps; i++)
        {
            x = (i + 0.5) * step;
            sum = sum + 4.0 / (1.0 + x * x);
        }
    }

    double pi = step * sum;

    std::cout << "Our value is: " << pi << std::endl;
}
