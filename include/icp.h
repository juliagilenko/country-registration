#ifndef ICP_H
#define ICP_H

#include "country.h"
#include "kd_tree.h"

/*
* Runs a single iteration of the iterative closest point algorithm.
* For data shape P = {p_i} and model shape Q = {q_j}, minimise the following energy:
*     E = (Σ_{p_i} || s * R * p_i + t - q_{c_i} ||^2)/ s^2
* where
*   - s ∈R (scale)
*   - R ∈R^{2x2} (rotation)
*   - t ∈R^2 (translation)
*   - q_{c_i} ∈ R^2 (corresponding point on Q to p_i)
* 
* Input:
*   model               model country
*   data                data country
*   model_tree          KD tree of model country 
*   data_perimeters     cumulative sum of relative line lengths on perimeter of data country
*   num_samples         number of points to sample on data country
* 
* Output:
*   data                transformed data country
*/
void icp_single_iteration(
    Country* model,
    Country* data,
    KD_Tree* model_tree,
    Eigen::VectorXd& data_perimeters,
    int num_samples);

#endif