/*
**  PROGRAM: Mandelbrot area
**
**  PURPOSE: Program to compute the area of a  Mandelbrot set.
**           Correct answer should be around 1.510659.
**           WARNING: this program may contain errors
**
**  USAGE:   Program runs without input ... just run the executable
**
**  HISTORY: Written:  (Mark Bull, August 2011).
**           Changed "comples" to "d_comples" to avoid collsion with
**           math.h complex type (Tim Mattson, September 2011)
*/

#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NPOINTS 1000
#define MAXITER 1000

struct d_complex {
  double r;
  double i;
};

void testpoint(struct d_complex);

struct d_complex c;
int numoutside = 0;

int main() {
  int i, j;
  double area, error, eps = 1.0e-5;

  //   Loop over grid of points in the complex plane which contains the Mandelbrot set,
  //   testing each point to see whether it is inside or outside the set.

#pragma omp parallel for default(shared) private(c, j) firstprivate(eps)
  for (i = 0; i < NPOINTS; i++) {
    for (j = 0; j < NPOINTS; j++) {
      c.r = -2.0 + 2.5 * (double)(i) / (double)(NPOINTS) + eps;
      c.i = 1.125 * (double)(j) / (double)(NPOINTS) + eps;
      testpoint(c);
    }
  }

  // Calculate area of set and error estimate and output the results

  area = 2.0 * 2.5 * 1.125 * (double)(NPOINTS * NPOINTS - numoutside) / (double)(NPOINTS * NPOINTS);
  error = area / (double)NPOINTS;

  printf("Area of Mandlebrot set = %12.8f +/- %12.8f\n", area, error);
  printf("Correct answer should be around 1.510659\n");
}

void testpoint(struct d_complex c) {

  // in original code this function pick the complex number from the global scope and all threads
  // where accessing the same point

  // Does the iteration z=z*z+c, until |z| > 2 when point is known to be outside set
  // If loop count reaches MAXITER, point is considered to be inside the set

  struct d_complex z;
  int iter;
  double temp;

  z = c;
  for (iter = 0; iter < MAXITER; iter++) {
    temp = (z.r * z.r) - (z.i * z.i) + c.r;
    z.i  = z.r * z.i * 2 + c.i;
    z.r  = temp;
    if ((z.r * z.r + z.i * z.i) > 4.0) {
// with atomic there won't be multiple threads modifing the variable at once
#pragma omp atomic
      numoutside++;
      break;
    }
  }
}

/** Just a function to explain the different private clauses */
void explanation_function() {
  int tmp_1{0};
  // we can create a private variable for each instance of the loop the problem is that the value
  // can't be initialize with private. The final print will print the global variable so 0

#pragma omp parallel for private(tmp_1)
  for (int j = 0; j < 1000; ++j) {
    tmp_1 += j;
  }
  printf("%d\n", tmp_1);

  // FIRST PRIVATE
  int tmp_2{0};
  // with first private we can initialize the variable to the global variable.
  // the problem is that at the end of the loop we don't have the value so the print is going to
  // print 0 also.

#pragma omp parallel for firstprivate(tmp_2)
  for (int j = 0; j < 1000; ++j) {
    tmp_2 += j;
  }
  printf("%d\n", tmp_2);

  // LAST PRIVATE
  int tmp_3{0};
  // with last private we can take the last value the variable takes inside the loop and copy it
  // into our global variable

#pragma omp parallel for lastprivate(tmp_3)
  for (int j = 0; j < 1000; ++j) {
    tmp_3 += j;
  }
  printf("%d\n", tmp_3);
}
