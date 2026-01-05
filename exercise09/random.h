#ifndef RANDOM_H
#define RANDOM_H

#ifdef __cplusplus
extern "C" {
#endif

double drandom();
void seed(double low_in, double hi_in);

#ifdef __cplusplus
}
#endif

#endif