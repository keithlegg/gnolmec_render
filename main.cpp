

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "include/render.h"

using namespace std;





/*
   TO RUN: 

       ./renderthing 500 500 3d_obj/cone.obj 0 60 0 foo.bmp

*/



int main(int argc, char *argv[])
{

    //model MOBJ;
    //MOBJ.make_cube(1);
    //MOBJ.save_obj("mycube.obj");
    
    if (argc < 8){
        cout << "ARGS: xres yres inputfile X Y Z outputfile \n";
        return 0;
    }
   

    //intersect_2d();


    //( int width, int height, char* filename ,float RX, float RY, float RZ ,char* outfilename){
    //render_model( atoi(argv[1]), atoi(argv[2]), argv[3], atof(argv[4]), atof(argv[5]), atof(argv[6]),  argv[7]);
    really_simple_render_model( atoi(argv[1]), atoi(argv[2]), argv[3], atof(argv[4]), atof(argv[5]), atof(argv[6]),  argv[7]);

    //render_model(256,256,"mycube.obj",0,0,0,"foo.bmp");
    //render_model(256,256,"mycube.obj", 0, 90 ,0,"foo.bmp");
  
    return 0;
}




