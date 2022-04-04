#ifndef SAMPLING_H
#define SAMPLING_H

#include <Eigen/Core>

#include "country.h"
#include "line.h"

/*
* Compute cumulative sum of relative line lengths on perimeter of data country. That is, given a 
* vector l with n elements where l[i] is length of the i-th line of the country's border,
* output[i] = (Σ_{i=0}^{i} l[i]) / (Σ_{j=0}^{n} l[j])
*/
Eigen::VectorXd relative_perimeter_cumulative_sum(Country* country);

/*
* Sample points on country's border uniformly randomly.
* 
* Input:
*   country         country to sample
*   perimeter_sum   cumulative sum of relative line lengths on country's perimeter
*   num_samples     number of samples to take
* 
* Returns:
*   2 x num_samples matrix with sampled points 
*/
Eigen::MatrixXd sample_country(Country* country, Eigen::VectorXd& perimeter_sums, int num_samples);

#endif