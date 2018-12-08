

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "include/render.h"
#include "include/model.h"

using namespace std;





/*
   TO RUN: 

       ./renderthing 500 500 3d_obj/cone.obj 0 60 0 foo.bmp

*/


void test_intersection(){

    float xcoord = 0;
    float ycoord = 0;

    float *xpt = &xcoord; 
    float *ypt = &ycoord; 

    get_line_intersection(-2, -3, 1, 2, -3, 2, 2, -2, xpt, ypt);
  
    cout << "test intersection: " << xcoord << ycoord << endl ;

}


/***********************************/


int main(int argc, char *argv[])
{

    model MOBJ;
    //MOBJ.make_cube(1.1);
    //MOBJ.make_circle( 4, 5.2);

    
    MOBJ.load_obj("3d_obj/monkey.obj");
    MOBJ.save_obj("newmonkey.obj");
   


    // if (argc < 8){
    //     cout << "ARGS: xres yres inputfile X Y Z outputfile \n";
    //     return 0;
    // }
   

    // test_intersection();
  



    // really_simple_render_model( atoi(argv[1]), atoi(argv[2]), argv[3], atof(argv[4]), atof(argv[5]), atof(argv[6]),  argv[7]);



    //render_model(256,256,"mycube.obj",0,0,0,"foo.bmp");
    //render_model(256,256,"mycube.obj", 0, 90 ,0,"foo.bmp");
  
    return 0;
}




