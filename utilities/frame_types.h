#pragma once

struct EnuUnitVector
{
    double azimuth_degrees;
    double elevation_degrees;
};

struct EnuVector : public EnuUnitVector
{
    double range_meters;
};