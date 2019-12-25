#ifndef POINTOPS_H
#define POINTOPS_H


#include <iostream>
#include <stdio.h>
#include <vector>

#include "Vectors.h"
#include "Matrices.h"


 
class point_ops {
    public:

        point_ops(){
        };

        ~point_ops(){};


    
    //double triangle_mean_z(Vector3 pt1, Vector3 pt2, Vector3 pt3){}
        
    // cvt_2d_to_3d( points ){}
    // cvt_3d_to_2d( points ){}
    // locate_pt_along3d( x1, y1, z1, x2, y2, z2, num)


};



class polygon_ops : public point_ops 
{ 
    public: 
      int id_c; 

        polygon_ops(){
        };

        ~polygon_ops(){};

    
    void hello(void); //test of inheritance 

    int getnum_verts(void);

    //void z_sort(void);

    double triangle_mean_z(Vector3 p1, Vector3 p2, Vector3 p3);
  
    void triangle_centroid(Vector3 *out, Vector3 p1, Vector3 p2, Vector3 p3);

    // calc_tripoly_normal( three_pts, unitlen)
    // three_vec3_to_normal( v1, v2, v3, unitlen=False):


}; 





#endif


