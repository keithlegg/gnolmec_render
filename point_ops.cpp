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



