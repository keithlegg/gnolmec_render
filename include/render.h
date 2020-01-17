
#pragma once

#ifndef RENDER_H
#define RENDER_H

#include "Vectors.h"
#include "Matrices.h"

#include "framebuffer.h"




class sceneloader
{

    public: 
        sceneloader(){
            object_path[0]      = 0;
            cam_matrix_path[0]  = 0;
            proj_matrix_path[0] = 0;
        };

        ~sceneloader(){};

        //----------------------
        bool show_vtx   = false;
        bool show_lines = false;

        framebuffer::RGBType bg_color; 
        framebuffer::RGBType vtx_color; 
        framebuffer::RGBType line_color; 
        framebuffer::RGBType fill_color; 
        
        char object_path[100];
        char cam_matrix_path[100];
        char proj_matrix_path[100];

        Vector3 campos; 
        Vector3 lightpos; 
        float lightintensity;
        

        Matrix4 camera_matrix;
        Matrix4 projection_matrix;

        //----------------------
        
        void show( void );

        void load_file( char* filepath );


        
        /*
        void load_camera_matrix();
        void load_projection_matrix();


        void set_linecolor(){
            //vtx_color.r = 255;
            //vtx_color.g = 0;
            //vtx_color.b = 0;
        }

        void set_vtxcolor(){
            //vtx_color.r = 255;
            //vtx_color.g = 0;
            //vtx_color.b = 0;
        }

        void set_fillcolor(){
            //vtx_color.r = 255;
            //vtx_color.g = 0;
            //vtx_color.b = 0;
        }*/


};




void make_image(int width, int height, char *outfile);

// intersect_2d (self, v1s, v1e, v2s, v2e ):
//void intersect_2d (void);

int get_line_intersection(double p0_x, double p0_y, double p1_x, double p1_y, 
    double p2_x, double p2_y, double p3_x, double p3_y, double *i_x, double *i_y);


int raster_clip( double x1  , double y1  , double x2  , double y2  ,   // input line (raster) 
                 double ix1 , double iy1 , double ix2 , double iy2 ,   // line1 to intersect
                 double jx1 , double jy1 , double jx2 , double jy2 ,   // line2 to intersect
                 double *ox1, double *oy1, double *ox2, double *oy2);  // output line, if any 


void draw_triangle( sceneloader* prs, int width, int height, double rscale, 
                    framebuffer* fb, Vector3 p1, Vector3 p2, Vector3 p3, 
                    framebuffer::RGBType fillcolor, framebuffer::RGBType linecolor);

void draw_lines( sceneloader* prs, int width, int height, double rscale, 
                    framebuffer* fb, Vector3 p1, Vector3 p2,  
                    framebuffer::RGBType linecolor);


int poly_clip(int width, int height, double *x1, double *y1, double *x2, double *y2);

//Vector4 rotate_points ( Matrix4 m, Vector4 v);
//Vector3 rotate_points ( Matrix4 m, Vector3 v);

void test_framebuffer(void);

void render_model( int width, int height, char* renderscript, char* outfilename);


//void render_model( int width, int height, char* objfilename ,float RX, float RY, float RZ ,char* outfilename, double RSCALE, int which); 

void really_simple_render_model( int width, int height, char* objfilename, char* matrixfile, float RX, float RY, float RZ, char* outfilename);







#endif

