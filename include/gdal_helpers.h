#ifndef GDAL_HELPERS_H
#define GDAL_HELPERS_H

#include <ogrsf_frmts.h>

/*
* Initializes GDAL, opens the shapefile, and retrieves the layer with the given name.
* Throws std::ios_base::failure if fails to open shapefile.
* Throws std::invalid_argument if layer could not be found. 
*/
OGRLayer* gdal_init(const char* shp_path, const char* layer_name);

/*
* Querys layer for country and returns its geometry. Throws std::runtime_error if country isn't found.
*/
OGRGeometry* get_geometry(OGRLayer* layer, std::string country);

#endif