#ifndef MODEL_H
#define _MODEL_H

#include <iostream>
#include <stdio.h>
// #include <stdlib.h> 

#include <vector>




class model {
    public:
        int vertex_count;
        int face_count;

        int vtx_cnt;
        int fac_cnt;
        //int cnt_fids; 


        Vector3 obj_pts[2000];
        //Vector3 faces3[1000];
        //Vector4 faces[1000];  //works but limits us to 4 sided 

        std::vector<double> vtx_tmp;
        std::vector<int> fac_tmp;        
        std::vector<int> faces[1000];


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


