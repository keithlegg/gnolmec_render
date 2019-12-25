#include <iostream>
using namespace std; 

#include <stdio.h>


#include "math.h"

#include "include/point_ops.h"
#include "include/Vectors.h"



// NOT USED YET 



int polygon_ops::getnum_verts(void){

}



/*
    return a sorted list of polygons by average Z coordinate 
    lowest z value is (closest?) to camera 
    this is pretty much a crappy hack for the renderer 
    
    DEBUG!
        it gives the appearance of faces not being on top of each other
        Will be replaced by a visible faces algorithm someday 
        Be aware, the rendered renders EVERY polygon even when they are not seen
      
*/


//test of inheritance 
void  polygon_ops::hello(void)
{
    cout << "hello from point opszzz !! \n";
}

double  polygon_ops::triangle_mean_z(Vector3 p1, Vector3 p2, Vector3 p3)
{
        double z1 = p1.z;
        double z2 = p2.z;
        double z3 = p3.z;
        return (z1+z2+z3)/3;
}

void triangle_centroid(Vector3 *out, Vector3 p1, Vector3 p2, Vector3 p3)
{

        double x1 = p1.x;
        double x2 = p2.x;
        double x3 = p3.x;

        double y1 = p1.y;
        double y2 = p2.y;
        double y3 = p3.y;

        double z1 = p1.z;
        double z2 = p2.z;
        double z3 = p3.z;

        out->x = (x1+x2+x3)/3;
        out->y = (y1+y2+y3)/3;
        out->z = (z1+z2+z3)/3;

}



