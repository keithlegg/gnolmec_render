

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "include/render.h"
#include "include/model.h"

#include "include/framebuffer.h"


using namespace std;





/*
   TO RUN: 

       ./renderthing 500 500 3d_obj/triangle.obj 0 60 0 foo.bmp 50

*/



void test_load_bmp(char *filename){

    framebuffer::RGBType* input_image;
    framebuffer loaded_bmp( 1024, 1024 );

    framebuffer::loadbmp(filename , input_image);

}

/***********************************/

void test_intersection(void){

    float xcoord = 0;
    float ycoord = 0;

    float *xpt = &xcoord; 
    float *ypt = &ycoord; 

    get_line_intersection(-2, -3, 1, 2, -3, 2, 2, -2, xpt, ypt);
  
    cout << "test intersection: " << xcoord << ycoord << endl ;

}

/***********************************/


void obj_file_stuff(void){
    
    model MOBJ;
    
    MOBJ.make_cube(1.1);
    
    //MOBJ.make_circle( 10, 5.2);
    //MOBJ.make_line( .2);   
    //MOBJ.make_square(2);
    
    //MOBJ.make_triangle( .2);  

    // MOBJ.load_obj("3d_obj/monkey.obj");

    //MOBJ.save_obj("3d_obj/triangle.obj");
    MOBJ.save_obj("cube.obj");

}

/***********************************/


int main(int argc, char *argv[])
{

    //test_load_bmp("images/uvmap.bmp");

    //obj_file_stuff();

    /* * * * * * * * * */ 

    /* * * * * * * * * */

    // test_intersection();
    //obj_file_stuff();

    /* * * * * * * * * */
    if (argc < 9){
        cout << "ARGS: xres yres inputfile X Y Z outputfile renderscale which\n";
        return 0;
    }

    // really_simple_render_model( atoi(argv[1]), atoi(argv[2]), argv[3], atof(argv[4]), 
    //                             atof(argv[5]), atof(argv[6]),  argv[7],   atof(argv[8]) );

    /* * * * * * * * * */
    render_model( atoi(argv[1]), atoi(argv[2]), argv[3], atof(argv[4]), 
                                atof(argv[5]), atof(argv[6]),  argv[7],  atof(argv[8]), atof(argv[9]) );

    /* * * * * * * * * */

    //render_model(256,256,"mycube.obj",0,0,0,"foo.bmp");
    //render_model(256,256,"mycube.obj", 0, 90 ,0,"foo.bmp");
  
    return 0;
}




