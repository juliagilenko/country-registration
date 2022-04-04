#ifndef COUNTRY_H
#define COUNTRY_H

#include <vector>

#include <Eigen/Core>
#include <ogr_geometry.h>

/*
* A country that's been projected to the plane using the Mercator projection.
* Coordinates are stored in 2xN matrices. Landmasses are stored in a vector of matrices.
*/
class Country 
{
public:
    Country(OGRGeometry* geometry);
    std::vector<Eigen::MatrixXd> get_landmasses() const;
    void set_landmasses(std::vector<Eigen::MatrixXd> landmasses);
    Eigen::MatrixXd get_all_coords() const;
    int get_num_coords() const;
    void Country::translate_to_vicinity_of(Country* other);

private:
    std::vector<Eigen::MatrixXd> landmasses;
    Eigen::MatrixXd all_coords;
    Eigen::MatrixXd concat_landmasses() const;
    Eigen::Vector2d mercator_proj(OGRPoint* point) const;
    Eigen::MatrixXd collect_coords(OGRLinearRing* ring) const;
    void collect_coords(OGRPolygon* poly, std::vector<Eigen::MatrixXd>& landmasses) const;
    void collect_coords(OGRMultiPolygon* poly, std::vector<Eigen::MatrixXd>& landmasses) const;
};

#endif