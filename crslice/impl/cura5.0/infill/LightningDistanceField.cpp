//Copyright (c) 2022 Ultimaker B.V.
//CuraEngine is released under the terms of the AGPLv3 or higher.

#include "infill/LightningDistanceField.h" //Class we're implementing.
#include "infill/infill.h"
#include "utils/polygonUtils.h" //For spreadDotsArea helper function.
#include "types/EnumSettings.h"

namespace cura52
{
    /*!
    * Generate a grid of dots inside of the area of the \p polygons.
    */
    std::vector<Point> spreadDotsArea(const Polygons& polygons, coord_t grid_size)
    {
        std::vector<VariableWidthLines> dummy_toolpaths;
        Settings dummy_settings;
        Infill infill_gen(EFillMethod::LINES, false, false, polygons, 0, grid_size, 0, 1, 0, 0, 0, 0, 0);
        Polygons result_polygons;
        Polygons result_lines;
        infill_gen.generate(dummy_toolpaths, result_polygons, result_lines, dummy_settings);
        std::vector<Point> result;
        for (PolygonRef line : result_lines)
        {
            assert(line.size() == 2);
            Point a = line[0];
            Point b = line[1];
            assert(a.X == b.X);
            if (a.Y > b.Y)
            {
                std::swap(a, b);
            }
            for (coord_t y = a.Y - (a.Y % grid_size) - grid_size; y < b.Y; y += grid_size)
            {
                if (y < a.Y)
                    continue;
                result.emplace_back(a.X, y);
            }
        }
        return result;
    }

constexpr coord_t radius_per_cell_size = 6;  // The cell-size should be small compared to the radius, but not so small as to be inefficient.

LightningDistanceField::LightningDistanceField
(
    const coord_t& radius,
    const Polygons& current_outline,
    const Polygons& current_overhang
)
: cell_size(radius / radius_per_cell_size)
, grid(cell_size)
, supporting_radius(radius)
, current_outline(current_outline)
, current_overhang(current_overhang)
{
    std::vector<Point> regular_dots = spreadDotsArea(current_overhang, cell_size);
    for (const auto& p : regular_dots)
    {
        const ClosestPolygonPoint cpp = PolygonUtils::findClosest(p, current_outline);
        const coord_t dist_to_boundary = vSize(p - cpp.p());
        unsupported_points.emplace_back(p, dist_to_boundary);
    }
    unsupported_points.sort
    (
        [&radius](const UnsupCell& a, const UnsupCell& b)
        {
            constexpr coord_t prime_for_hash = 191;
            return
                std::abs(b.dist_to_boundary - a.dist_to_boundary) > radius ?
                a.dist_to_boundary < b.dist_to_boundary :
                (std::hash<Point>{}(a.loc) % prime_for_hash) < (std::hash<Point>{}(b.loc) % prime_for_hash);
        }
    );
    for (auto it = unsupported_points.begin(); it != unsupported_points.end(); ++it)
    {
        UnsupCell& cell = *it;
        unsupported_points_grid.emplace(grid.toGridPoint(cell.loc), it);
    }
}

bool LightningDistanceField::tryGetNextPoint(Point* p) const
{
    if (unsupported_points.empty())
    {
        return false;
    }
    *p = unsupported_points.front().loc;
    return true;
}

void LightningDistanceField::update(const Point& to_node, const Point& added_leaf)
{
    auto process_func = 
        [added_leaf, this](const SquareGrid::GridPoint& grid_loc)
        {
            auto it = unsupported_points_grid.find(grid_loc);
            if (it != unsupported_points_grid.end())
            {
                std::list<UnsupCell>::iterator& list_it = it->second;
                UnsupCell& cell = *list_it;
                if (shorterThen(cell.loc - added_leaf, supporting_radius))
                {
                    unsupported_points.erase(list_it);
                    unsupported_points_grid.erase(it);
                }
            }
            return true;
        };
    const Point a = to_node;
    const Point b = added_leaf;
    Point ab = b - a;
    Point ab_T = turn90CCW(ab);
    Point extent = normal(ab_T, supporting_radius);
    // TODO: process cells only once; make use of PolygonUtils::spreadDotsArea
    grid.processLineCells(std::make_pair(a + extent, a - extent),
                          [this, ab, extent, &process_func] (const SquareGrid::GridPoint& p)
                          {
                              grid.processLineCells(std::make_pair(p, p + ab), process_func);
                              return true;
                          }
    );
    grid.processNearby(added_leaf, supporting_radius, process_func);
}

}