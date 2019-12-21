#ifndef MODEL_H
#define _MODEL_H


#include <iostream>
#include <stdio.h>
#include <vector>

#include "Vectors.h"
#include "Matrices.h"


#define MAX_NUM_VERTICES 10000
#define MAX_NUM_FACES 7000


class model {
    public:
        int vertex_count;
        int face_count;

        int vtx_cnt;
        int fac_cnt;

        Vector3 obj_pts[MAX_NUM_VERTICES]; //okay to limit to 3, never more than that for a point
        
        //std::vector<double> m44_elem;  
        std::vector<double> vtx_tmp;
        std::vector<int> fac_tmp;        
        std::vector<int> faces[MAX_NUM_FACES];

        Matrix4 m44;

        model(){
            face_count     = 0;
            vertex_count   = 0;
            vtx_cnt        = 1; 
            fac_cnt        = 1; 
        };

        ~model(){};

        int getnum_verts();
        int getnum_edges();
        int getnum_faces();

        void showinfo();
        void show();

        void save_obj( char* filename );
        void load_obj( char* filename );
        void load_matrix(char* filename);

        void make_line(double scale); 
        void make_triangle(double scale); 
        void make_circle(int divs, double scale);
        
        void make_square(double scale);  
        void make_cube(double scale);
   


};






#endif


