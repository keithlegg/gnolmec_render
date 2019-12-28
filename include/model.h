#ifndef MODEL_H
#define MODEL_H


#include <iostream>
#include <stdio.h>
#include <vector>

#include "Vectors.h"
#include "Matrices.h"

#include "point_ops.h"

#define MAX_NUM_VERTICES 10000
#define MAX_NUM_FACES 10000


// experiment to sort faces based on a z value 
struct zindex_faces
{
    double distance;
    std::vector<int> face; 
};



class model: public polygon_ops {
    public:
        bool geom_edited  = false; // semaphore to mark internal edits 

        int vertex_count;
        int line_count;     // 2 sided polygons (lines)      
        int triangle_count; // 3 sided polygons (triangles )
        int quad_count;     // 4 sided polygons 
        int nsided_count;   // all polygons that are not 1,2,3 sided 
        //int face_count;     // sum of ALL faces combined  

        int uv_count;       
        int normal_count;

        // --- 
        int vtx_cnt;
        int fac_cnt;

        std::vector<double> vtx_tmp;
        std::vector<int>    fac_tmp;  

        // --- 
        Vector3 obj_pts[MAX_NUM_VERTICES];          // vertices of model    
        // --- 
        std::vector<int> lines[MAX_NUM_FACES];      // 2 sided faces 
        std::vector<int> triangles[MAX_NUM_FACES];  // 3 sided 
        std::vector<int> quads[MAX_NUM_FACES];      // 4 sided 
        std::vector<int> faces[MAX_NUM_FACES];      // >4, N sided faces 
        // ---
        Vector3 bfr_pts[MAX_NUM_VERTICES];          // general point buffer   ( tmp work area )
        std::vector<int> bfr_faces[MAX_NUM_FACES];  // general polygon buffer ( tmp work area ) 

        // ---
        Matrix4 m44;

        model(){
            vtx_cnt        = 1; //? why 1?

            vertex_count   = 0;
            line_count     = 0;  // 2 sided polygons (lines)      
            triangle_count = 0;  // 3 sided polygons (triangles )
            quad_count     = 0;  // 4 sided polygons 
            nsided_count   = 0;  // all polygons that are not 1,2,3 sided 

        };

        ~model(){};

        //void hello(void); //test of inheritance

        void showinfo(void);
        void show(void);

        void reset_buffers(void);

        /***********************************/

        int getnum_verts();
        int getnum_faces();
        // int getnum_edges();
        // int getnum_lines();
        // int getnum_uvs();
        // int getnum_normals();

        /***********************************/        

        void add_tri(int vid1, int vid2, int vid3);
        void append_tri(Vector3 pt1, Vector3 pt2, Vector3 pt3, int vid1, int vid2, int vid3);
        
        // void copy_tri(int index);
        // void del_tri(int index);
        


        // void copy_quad
        // void del_quad
        // void add_quad

        // void copy_face
        // void del_face
        // void add_face

        void op_zsort( Vector3 campos );
        void op_triangulate(void);

        // get_pt_ids(self, fids=None):
        // get_face_pts(self, fid):
        // get_face_normal(self, fid=None, unitlen=False ):

        // void op_extrude(int fid, double dist);
        // void op_calc_bbox(int fid, double dist);


        void flatten_edits(void);

        /***********************************/  

        void prep_render(void);

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


