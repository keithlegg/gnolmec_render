#ifndef MODEL_H
#define _MODEL_H


#include <iostream>
#include <stdio.h>
#include <vector>


#define MAX_NUM_VERTICES 5000
#define MAX_NUM_FACES 2000


class model {
    public:
        int vertex_count;
        int face_count;

        int vtx_cnt;
        int fac_cnt;

        Vector3 obj_pts[MAX_NUM_VERTICES]; //okay to limit to 3, never more than that for a point

        std::vector<double> vtx_tmp;
        std::vector<int> fac_tmp;        
        std::vector<int> faces[MAX_NUM_FACES];

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

        void make_cube(double scale);
        void make_circle(int divs, double scale);
        void make_square(double scale);     

};






#endif


