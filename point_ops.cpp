#include <iostream>
using namespace std; 

#include <stdio.h>


#include "math.h"

#include "include/point_ops.h"
#include "include/Vectors.h"



// NOT USED YET 



int polygon_ops::getnum_verts(void){

}
/**********************************************/


/**********************************************/


//test of inheritance 
void  polygon_ops::hello(void)
{
    cout << "hello from point opszzz !! \n";
}


/**********************************************/

// // create a normal vector (vec3) from 3 points that represent a polygon  
// // take 3 point (positions), convert to vectors 
// // and run three_vec3_to_normal on them  
// Vector3 calc_tripoly_normal( Vector3 v1, Vector3 v2, Vector3 v3, bool unitlen=false): 
// {
//     Vector3 v1;
//     Vector3 v2;
//     Vector3 v3;
// 
//     //v1.insert( three_pts[0] )
//     //v2.insert( three_pts[1] )
//     //v3.insert( three_pts[2] )  
// 
//     return self.three_vec3_to_normal(v1, v2, v3, unitlen=unitlen)
// }


/**********************************************/


/*
    def between(self, pt2):
        """ given 2 points in 3D, create a 3D vector 
            representing the offset between them 

            doesnt get much easier than this, just subtract 

        """
   
        #if isinstance(pt1, tuple):
        #    pt1 = vec3( pt1[0], pt1[1], pt1[2]) 


        if isinstance(pt2, tuple):
            pt2 = vec3( pt2[0], pt2[1], pt2[2])

        return pt2 - self
*/

/**********************************************/

// take 3 vec3 objects and return a face normal 
Vector3 polygon_ops::three_vec3_to_normal( Vector3 v1, Vector3 v2, Vector3 v3, bool unitlen)
{

    // calculate the face normal  
    Vector3 a = v1 - v2;
    Vector3 b = v1 - v3;

    Vector3 f_nrml;

    if(unitlen == true){
        f_nrml = a.cross(b).normalize();
    }
    else {    
        f_nrml = a.cross(b);          
    }

    return f_nrml; 
   
}

/**********************************************/

double  polygon_ops::triangle_mean_z(Vector3 p1, Vector3 p2, Vector3 p3)
{
    double z1 = p1.z;
    double z2 = p2.z;
    double z3 = p3.z;
    return (z1+z2+z3)/3;
}

/**********************************************/

void polygon_ops::triangle_centroid(Vector3 *out, Vector3 p1, Vector3 p2, Vector3 p3)
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



