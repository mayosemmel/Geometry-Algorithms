/**
 * @file earClippingTriangulation.h
 * @author Rikil Gajarla (f20170202@hyderabad.bits-pilani.ac.in)
 * @brief This file contains the implementation of the ear clipping triangulation algorithm
 * 
 */

#ifndef __EARCLIPPINGTRIANGULATION_H__
#define __EARCLIPPINGTRIANGULATION_H__

#include <list>
#include <vector>

#include "Geometry.h"
#include "monotoneSubdivision.h"
#include "monotoneTriangulation.h"
#include "Geometry.h"
#include "Tools.h"

using namespace std;

/**
 * @brief This function implements the basic ear clipping triangulation.
 * 
 * The main approach behind this algorithm is that we try to find "ears" which are 
 * basically the vertices of the polygon whose interior angle is less than PI. 
 * We then triangulate these ears and remove them from the Polygon. we perform the 
 * same operation on the polygon until it is completely triangulated
 * 
 * @param points The input vertices of polygon in clockwise order
 * @return vector<Triangle> List of Triangle objects which are the triangulations of the input polygon
 */
vector<Triangle> earClippingTriangulate(vector<Vertex> points) { // the ear clipping algorithm
    vector<Triangle> triangles;
    vector<Vertex> initialPoints = points;

    if(points.size() < 3) return triangles; // can't have a triangle with < 3 points

    bool infiniteLoop = false;

    while (!points.empty()) {
        if (infiniteLoop) {
            cout << "Unable to completely triangulate, likely to be a 8 shape or self intersecting polygon\n";
            return triangles;
        }
        infiniteLoop = true;

        for (unsigned long i=0; i<points.size(); i++) {
            // if triangle is inside poly and no point in poly, 
            Vertex &prev = (i>0)?points[i-1]:points[points.size()-1];
            Vertex &next = points[(i+1)%points.size()];
            if (orient(prev, points[i], next) <= 0) { // let the collinear points be a triangle on their own
                // cout << "Testing: " << prev << " " << points[i] << " " << next << "\n";
                // check if any other point lies inside this triangle
                bool flag = true;
                for (auto &p: initialPoints) {
                    if (pointInTriangle(prev, points[i], next, p)) {
                        flag = false;
                        break;
                    }
                }
                if (flag) { // no point lies inside triangle
                    if(!(prev == points[i] || next == points[i] || prev == next)) {
                        triangles.emplace_back(prev, points[i], next);
                    }
                    points.erase(points.begin()+i);
                    infiniteLoop = false;
                }

                if (points.size() < 3) {
                    if (points.size() > 0) {
                        points.erase(points.begin()+i);
                    }
                    infiniteLoop = false;
                }
                
            }
        }
    }

    return triangles;
}


/**
 * @brief The wrapper method to earClipping Triangulate a polygon
 * 
 * This function internally calls getMonotones() and earClippingTriangulate() 
 * functions to get the triangulations
 * 
 * @param input List of Vertices of Polygons present in clockwise order
 * @return vector<Triangle> List of Triangle objects representing the triangulation of polygon
 */
vector<Triangle> planeSweepEarClippingTriangulate(vector<Vertex> input) {
    vector<Triangle> output;
    list<vector<Vertex>> monotones = getMonotones(input);
    // cout << "no of monotones: " << monotones.size() << "\n";
    // for (auto &monotone: monotones) {
    //     for (auto &x: monotone) {
    //         cout << x << " ";
    //     }cout << "\n";
    // }
    // exit(1);
    for (auto &monotone: monotones) {
        vector<Triangle> temp = earClippingTriangulate(monotone);
        output.insert(output.end(), temp.begin(), temp.end());
    }
    return output;
}



#endif
