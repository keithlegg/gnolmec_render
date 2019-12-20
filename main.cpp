

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "include/render.h"
#include "include/model.h"

#include "include/framebuffer.h"


using namespace std;





/*
   TO RUN: 

       ./renderthing 500 500 3d_obj/triangle.obj 0 60 0 foo.bmp 50 2

*/



void test_load_bmp(char *filename){

    /* someday I will be able to load BMP files as well as save them ... sigh */
  
    framebuffer::RGBType* input_image;
    framebuffer loaded_bmp( 1024, 1024 );

    framebuffer::loadbmp(filename , input_image);

}

/***********************************/

void test_intersection(void){

    /* test of the function that calculates a line intersetion */ 

    float xcoord = 0;
    float ycoord = 0;

    float *xpt = &xcoord; 
    float *ypt = &ycoord; 

    get_line_intersection(-2, -3, 1, 2, -3, 2, 2, -2, xpt, ypt);
  
    cout << "test intersection: " << xcoord << ycoord << endl ;

}

/***********************************/


void obj_file_stuff(void){
    /* test of loading and saving an OBJ file with my 3D model class*/

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
/*
   self contained example of creating a BMP file 
   detached from the renderer, just to show how to driectly 
   create a framebuffer and dump it to disk.

   The colors are a total mess. 
   OSX compiles the BMP exporter function differently.
   See framebuffer.cpp line 184 for a better explanation.
*/

void test_image_draw( int width, int height, char *outfile)
{

    framebuffer test_draw( width, height );
    framebuffer *ptest_draw = &test_draw;

    int pix_iterator;

    /***********/
    short flat_color = 128; //background grey color 

    //fill each pixel with a color  
    for (int x = 0; x < width; x++)
    {   
        for (int y = 0; y < height; y++)
        {  
            pix_iterator = y * width + x;     

            test_draw.rgbdata[pix_iterator].r = flat_color;  //= 0x254;      
            test_draw.rgbdata[pix_iterator].g = flat_color;  //= 0x0
            test_draw.rgbdata[pix_iterator].b = flat_color;  //= 0x0

        }
   }


   framebuffer::RGBType  drawcolor;
   drawcolor.r = 0x255;
   drawcolor.g = 0x0;
   drawcolor.b = 0x0;

   ptest_draw->draw_point(2, 2, drawcolor);
   //ptest_draw->draw_point(11, 11, drawcolor);
   //ptest_draw->draw_point(12, 12, drawcolor);

   // instance method 
   ptest_draw->savebmp (outfile, width, height, 300, test_draw.rgbdata) ;

   //static/class method 
   //framebuffer::savebmp (outfile, width, height, 300, test_draw.rgbdata) ;

}


/***********************************/

/*

   TO DO LIST 

   I discovered a bug where it will hang forever if you pass a folder instaed of a file for the input.

*/


int main(int argc, char *argv[])
{

    //test_load_bmp("images/uvmap.bmp");

    //test_image_draw(10, 10, "foeeeeo.bmp");


    //obj_file_stuff();

    /* * * * * * * * * */ 

    /* * * * * * * * * */

    // test_intersection();
    //obj_file_stuff();

    /* * * * * * * * * */
    if (argc < 8){
        cout << "ARGS: xres yres inputfile X Y Z outputfile renderscale which\n";
        return 0;
    }

 

    //renderthing 512 512 3d_obj/monkey.obj ../camera_matrix.olm 0 0 90 render.bmp 100
 

    // really_simple_render_model( int width, int height, char* objfilename,  char* matrixfile, float RX, float RY, float RZ ,
    //                                                     char* outfilename);

    really_simple_render_model( atoi(argv[1]), atoi(argv[2]), argv[3], argv[4], 
                                atof(argv[5]), atof(argv[6]),  atof(argv[7]),  argv[8]);





    // "ARGS: xres yres inputfile X Y Z outputfile renderscale which\n";
    // render_model( atoi(argv[1]), atoi(argv[2]), argv[3], atof(argv[4]), 
    //                              atof(argv[5]), atof(argv[6]),  argv[7],  atof(argv[8]), atof(argv[9]) );
   



    /* * * * * * * * * */

    //render_model(256,256,"mycube.obj",0,0,0,"foo.bmp");
    //render_model(256,256,"mycube.obj", 0, 90 ,0,"foo.bmp");
  
    return 0;
}




