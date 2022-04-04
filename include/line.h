#ifndef LINE_H
#define LINE_H

#include <Eigen/Core>

/*
* A line segment in R^2.
*/
class Line
{
public:
    Line(Eigen::Vector2d p1, Eigen::Vector2d p2);
    Eigen::Vector2d get_p1() const;
    Eigen::Vector2d get_p2() const;
    Eigen::Vector2d get_centre() const;
    double Line::get_length() const;

    /*
    * Find the closest point on this line to the query point.
    * 
    * Input:
    *   point           query point
    * 
    * Output
    *   closest_point   the closest point on this line
    *   dist            the distance from the query point to closest_point
    */
    void compute_closest_point(Eigen::Vector2d point, Eigen::Vector2d& closest_point, double& dist) const;

private:
    Eigen::Vector2d p1;
    Eigen::Vector2d p2;
    Eigen::Vector2d project(Eigen::Vector2d point) const;
    bool Line::is_point_on_line(Eigen::Vector2d point) const;
};

#endif