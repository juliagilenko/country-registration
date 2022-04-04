#include "gdal_helpers.h"

#include <assert.h>

#include <iostream>

#include <ogr_geometry.h>
#include <ogrsf_frmts.h>

OGRLayer* gdal_init(const char* shp_path, const char* layer_name)
{
    char err_buf[1024];
    GDALAllRegister();
    GDALDataset* dataset = (GDALDataset*) GDALOpenEx(shp_path, GDAL_OF_VECTOR, NULL, NULL, NULL);
    if (!dataset)
    {
        sprintf(err_buf, "Failed to open shapefile: %s", shp_path);
        throw std::ios_base::failure(err_buf);
    }

    OGRLayer* layer = dataset->GetLayerByName(layer_name);
    if (!layer)
    {
        sprintf(err_buf, "Invalid layer name: %s", layer_name);
        throw std::invalid_argument(err_buf);
    }

    return layer;
}

OGRGeometry* get_geometry(OGRLayer* layer, std::string country)
{
    assert(layer);
    layer->ResetReading();
    std::string filter = "TYPE IN ('Sovereign country', 'Disputed', 'Country') AND NAME = '" + country + "'"; 
    layer->SetAttributeFilter(filter.c_str());
    OGRFeature* feature = layer->GetNextFeature();

    if (!feature)
    {
        char err_buf[256];
        sprintf(err_buf, "No such country: %s", country.c_str());
        throw std::runtime_error(err_buf);
    }

    return feature->GetGeometryRef();
}