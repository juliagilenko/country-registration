#include "icp.h"

#include <Eigen/Core>
#include <Eigen/SVD>

#include "sampling.h"

Eigen::MatrixXd compute_correspondences(Eigen::MatrixXd& sampled_data, KD_Tree* model_tree)
{
    Eigen::MatrixXd sampled_model(sampled_data.rows(), sampled_data.cols());
    for (int i = 0; i < sampled_data.cols(); i++)
    {
        sampled_model.col(i) = model_tree->find_closest_point(sampled_data.col(i));
    }

    return sampled_model;
}

Eigen::Vector2d compute_centroid(Eigen::MatrixXd V)
{
    return V.rowwise().sum() / V.cols();
}

Eigen::MatrixXd compute_rotation(Eigen::MatrixXd centred_model, Eigen::MatrixXd centred_data)
{
    Eigen::MatrixXd weights(centred_model.cols(), centred_model.cols());
    weights.diagonal() = Eigen::VectorXd::Ones(centred_model.cols());

    Eigen::MatrixXd covariance_mat = centred_data * weights * centred_model.transpose();
    Eigen::BDCSVD<Eigen::MatrixXd> svd(covariance_mat, Eigen::ComputeThinU | Eigen::ComputeThinV);

    return svd.matrixV() * svd.matrixU().transpose();
}

// Σ_{i} (A.col(i).dot(B.col(i))
double sum_colwise_dot_product(Eigen::MatrixXd A, Eigen::MatrixXd B)
{
    return (A.transpose() * B).diagonal().sum();
}

double compute_scale(Eigen::MatrixXd centred_model, Eigen::MatrixXd centred_data, Eigen::MatrixXd R)
{
    double num = sum_colwise_dot_product(centred_model, centred_model);
    double denom = sum_colwise_dot_product(centred_data, R * centred_model);
    double s = num / denom;
    return s;
}

void transform_country(Country* country, Eigen::Matrix2d R, double s, Eigen::Vector2d t)
{
    std::vector<Eigen::MatrixXd> transformed_landmasses;
    for (int i = 0; i < country->get_landmasses().size(); i++)
    {
        Eigen::MatrixXd landmass = country->get_landmasses().at(i);
        transformed_landmasses.push_back((s * R * landmass).colwise() + t);
    }

    country->set_landmasses(transformed_landmasses);
}

void icp_single_iteration(Country* model, Country* data, KD_Tree* model_tree, Eigen::VectorXd& data_perimeters, int num_samples)
{
    Eigen::MatrixXd sampled_data = sample_country(data, data_perimeters, num_samples);
    Eigen::MatrixXd sampled_model = compute_correspondences(sampled_data, model_tree);

    Eigen::Vector2d centroid_model = compute_centroid(sampled_model);
    Eigen::Vector2d centroid_data = compute_centroid(sampled_data);

    Eigen::MatrixXd centred_model = sampled_model.colwise() - centroid_model;
    Eigen::MatrixXd centred_data = sampled_data.colwise() - centroid_data;

    Eigen::MatrixXd R = compute_rotation(centred_model, centred_data);
    double s = compute_scale(centred_model, centred_data, R);
    Eigen::VectorXd t = centroid_model - s * R * centroid_data;

    transform_country(data, R, s, t);
}