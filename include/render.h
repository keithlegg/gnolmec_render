
#pragma once

#ifndef RENDER_H
#define _RENDER_H

#include "Vectors.h"
#include "Matrices.h"

#include "framebuffer.h"





void make_image(int width, int height, char *outfile);

// intersect_2d (self, v1s, v1e, v2s, v2e ):
//void intersect_2d (void);

int get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y, 
    float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y);


void draw_scanline( framebuffer* fb, 
                    float whichline, float res_x, float res_y, 
                    float sx, float sy, float ex, float ey,  
                    float *phit_x , float *phit_y );


//Vector4 rotate_points ( Matrix4 m, Vector4 v);
//Vector3 rotate_points ( Matrix4 m, Vector3 v);

void test_framebuffer(void);

void render_model( int width, int height, char* objfilename ,float RX, float RY, float RZ ,char* outfilename, double RSCALE, int which); 

void really_simple_render_model( int width, int height, char* objfilename, char* matrixfile, float RX, float RY, float RZ, char* outfilename);





#endif

