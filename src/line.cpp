#include "line.h"

Line::Line(Eigen::Vector2d p1, Eigen::Vector2d p2)
{
    this->p1 = p1;
    this->p2 = p2;
}

Eigen::Vector2d Line::get_p1() const
{
    return p1;
}

Eigen::Vector2d Line::get_p2() const
{
    return p2;
}

Eigen::Vector2d Line::get_centre() const
{
    return (p1 + p2) / 2;
}

double Line::get_length() const
{
    return (p2 - p1).norm();
}

void Line::compute_closest_point(Eigen::Vector2d point, Eigen::Vector2d& closest_point, double& dist) const
{
    Eigen::Vector2d projection = project(point);

    if (is_point_on_line(projection))
    {
        closest_point = projection;
    }
    else
    {
        double dist1 = (p1 - point).norm();
        double dist2 = (p2 - point).norm();
        closest_point = dist1 < dist2 ? p1 : p2;
    }

    dist = (point - closest_point).norm();
}

Eigen::Vector2d Line::project(Eigen::Vector2d point) const
{
    Eigen::Vector2d line_segment = p2 - p1;
    Eigen::Vector2d point_to_p1 = point - p1;
    double cos_theta = line_segment.dot(point_to_p1) / (line_segment.norm() * point_to_p1.norm());
    double dist = cos_theta * point_to_p1.norm();
    return p1 + dist * line_segment / line_segment.norm();
}

bool Line::is_point_on_line(Eigen::Vector2d point) const
{
    double min_x = std::min(p1[0], p2[0]);
    double min_y = std::min(p1[1], p2[1]);
    double max_x = std::max(p1[0], p2[0]);
    double max_y = std::max(p1[1], p2[1]);

    return point[0] >= min_x && point[0] <= max_x && point[1] >= min_y && point[1] <= max_y;
}