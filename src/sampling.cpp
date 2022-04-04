#include "sampling.h"

Eigen::VectorXd relative_perimeter_cumulative_sum(Country* country)
{
    Eigen::VectorXd perimeters(country->get_num_coords());
    double cur_perimeter = 0;

    for (Eigen::MatrixXd landmass : country->get_landmasses())
    {
        for (int i = 0; i < landmass.cols(); i++)
        {
            Eigen::Vector2d p1 = landmass.col(i);
            Eigen::Vector2d p2 = i > 0 ? landmass.col(i - 1) : landmass.col(landmass.cols() - 1);
            cur_perimeter += (p2 - p1).norm();
            perimeters(i) = cur_perimeter;
        }
    }

    return perimeters / cur_perimeter;
}

// Length-weighted random sample of line segments of country's border.
Line* random_line(Country* country, Eigen::VectorXd& perimeter_sums)
{
    double p = (double)rand() / RAND_MAX;
    int l = 0;
    int r = perimeter_sums.size() - 1;
    while (l != r)
    {
        int m = (l + r) / 2;
        if (perimeter_sums(m) <= p)
        {
            l = m + 1;
        }
        else
        {
            r = m;
        }
    }

    Eigen::Vector2d p1 = country->get_all_coords().col(r);
    Eigen::Vector2d p2 = r > 0 ? country->get_all_coords().col(r - 1) : country->get_all_coords().col(country->get_all_coords().cols() - 1);
    return new Line(p1, p2);
}

// Uniform random sampling of a single line segment.
Eigen::Vector2d random_point_on_line(Line* line)
{
    double t = (double)rand() / RAND_MAX;
    return t * line->get_p1() + (1 - t) * line->get_p2();
}

Eigen::MatrixXd sample_country(Country* country, Eigen::VectorXd& perimeter_sums, int num_samples)
{
    Eigen::MatrixXd random_points(2, num_samples);
    for (int i = 0; i < num_samples; i++)
    {
        Line* line = random_line(country, perimeter_sums);
        random_points.col(i) = random_point_on_line(line);
        free(line);
    }

    return random_points;
}