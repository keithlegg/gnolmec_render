#ifndef MODEL_H
#define MODEL_H


#include <iostream>
#include <stdio.h>
#include <vector>

#include "Vectors.h"
#include "Matrices.h"

#include "point_ops.h"

#define MAX_NUM_VERTICES 10000
#define MAX_NUM_FACES 7000


// experiment to sort faces based on a z value 
struct zindex_faces
{
    double distance;
    std::vector<int> face; 
};



class model: public polygon_ops {
    public:
        int vertex_count;
        int face_count;     // all polygons 
        int nsided_count;   
        int line_count;     // 2 sided polygons (lines)      
        int triangle_count; // 3 sided polygons (triangles )
        int quad_count;     // 4 sided polygons 

        int uv_count;       
        int normal_count;

        // --- 
        int vtx_cnt;
        int fac_cnt;

        Vector3 obj_pts[MAX_NUM_VERTICES];  // vertices of model    
        Vector3 bfr_pts[MAX_NUM_VERTICES];  // vertices of work area (triangulate, etc)
 
        std::vector<double> vtx_tmp;
        std::vector<int>    fac_tmp;  

        std::vector<int> lines[MAX_NUM_FACES];      // >4, N sided faces 
        std::vector<int> triangles[MAX_NUM_FACES];  // 3 sided 
        std::vector<int> quads[MAX_NUM_FACES];      // 4 sided 
        std::vector<int> faces[MAX_NUM_FACES];      // >4, N sided faces 

        std::vector<int> bfr_faces[MAX_NUM_FACES];  // faces of work area 


        Matrix4 m44;

        model(){
            face_count     = 0;
            vertex_count   = 0;
            vtx_cnt        = 1; 
        };

        ~model(){};

        int getnum_verts();
        int getnum_faces();
        //int getnum_edges();
        //int getnum_lines();
        //int getnum_uvs();
        //int getnum_normals();

        void showinfo();
        void show(void);
   
        //void hello(void); //test of inheritance
        
        void flatten_geom(void);

        void sort_faces_dist( Vector3 campos );
        void triangulate(void);

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


