#include "country.h"

#include <ogr_geometry.h>
#include <iostream>

Country::Country(OGRGeometry* geometry) 
{
    assert(geometry);
    if (geometry->getGeometryType() == wkbMultiPolygon) 
    {
        collect_coords((OGRMultiPolygon*)geometry, landmasses);
    }
    else if (geometry->getGeometryType() == wkbPolygon)
    {
        collect_coords((OGRPolygon*)geometry, landmasses);
    }
    else 
    {
        throw std::domain_error("Country is not a polygon");
    }

    all_coords = concat_landmasses();
}

std::vector<Eigen::MatrixXd> Country::get_landmasses() const
{
    return landmasses;
}

void Country::set_landmasses(std::vector<Eigen::MatrixXd> landmasses)
{
    this->landmasses.clear();
    this->landmasses = std::move(landmasses);
    this->all_coords = concat_landmasses();
}

Eigen::MatrixXd Country::get_all_coords() const
{
    return all_coords;
}

int Country::get_num_coords() const
{
    return all_coords.cols();
}

void Country::translate_to_vicinity_of(Country* other)
{
    Eigen::MatrixXd V1(2, get_num_coords());
    Eigen::MatrixXd V2(2, other->get_num_coords());

    // overlay this country's centroid with the other's
    Eigen::Vector2d centroid1 = all_coords.rowwise().sum() / V1.cols();
    Eigen::Vector2d centroid2 = other->get_all_coords().rowwise().sum() / V2.cols();
    Eigen::Vector2d t = centroid2 - centroid1;

    for (int i = 0; i < landmasses.size(); i++)
    {
        landmasses.at(i).colwise() += t;
    }

    all_coords = concat_landmasses();
}

Eigen::MatrixXd Country::concat_landmasses() const
{
    int num_cols = 0;
    for (int i = 0; i < landmasses.size(); i++)
    {
        num_cols += landmasses.at(i).cols();
    }

    Eigen::MatrixXd V(2, num_cols);
    int start_col = 0;
    for (int i = 0; i < landmasses.size(); i++)
    {
        V.middleCols(start_col, landmasses.at(i).cols()) = landmasses.at(i);
        start_col += landmasses.at(i).cols();
    }

    return V;
}

Eigen::Vector2d Country::mercator_proj(OGRPoint* point) const
{
    double long_rad = point->getX() * M_PI / 180;
    double lat_rad = point->getY() * M_PI / 180;
    return Eigen::Vector2d(long_rad, std::log(std::abs(1 / std::cos(lat_rad) + std::tan(lat_rad))));
}

Eigen::MatrixXd Country::collect_coords(OGRLinearRing* ring) const
{
    assert(ring);

    Eigen::MatrixXd V(2, ring->getNumPoints());
    for (int i = 0; i < ring->getNumPoints(); i++)
    {
        OGRPoint* point = new OGRPoint();
        ring->getPoint(i, point);
        V.col(i) = mercator_proj(point);
    }

    return V;
}

void Country::collect_coords(OGRPolygon* poly, std::vector<Eigen::MatrixXd>& landmasses) const
{
    assert(poly);
    landmasses.push_back(collect_coords(poly->getExteriorRing()));
}

void Country::collect_coords(OGRMultiPolygon* multipoly, std::vector<Eigen::MatrixXd>& landmasses) const
{
    assert(multipoly);

    for (int i = 0; i < multipoly->getNumGeometries(); i++)
    {
        collect_coords(multipoly->getGeometryRef(i), landmasses);
    }
}