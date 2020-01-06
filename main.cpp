

#include <iostream>
#include <stdio.h>
#include <string.h>

#include "include/render.h"
#include "include/model.h"

#include "include/framebuffer.h"

#include "include/BMP.h"

using namespace std;


#include "include/ngc_model.h"


/*
   TO RUN: 

       ./renderthing 500 500 3d_obj/triangle.obj 0 60 0 foo.bmp 50 2
*/




void test_load_save_bmp(char *infile, char *outfile)
{
    /* someday I will be able to load BMP files as well as save them ... sigh */  
    framebuffer::RGBType* imagedata;
    framebuffer loaded_bmp( 512, 512 );

    framebuffer::loadbmp(infile , imagedata);

    framebuffer::savebmp(outfile, 512, 512, 72, imagedata);

}



void test_load_save_ngc(char *infile, char *outfile)
{

    ngc_model foo;

    //foo.load_obj("3d_obj/cone.obj");
    //foo.load_ngc("3d_obj/arcspiral.ngc");
    foo.load_ngc("3d_obj/3dtest.ngc");

    //foo.load_ngc("3d_obj/3D_Chips.ngc");

    //foo.convert_to_3d();
    //foo.show();
    //foo.showinfo();

    foo.save_ngc("3d_obj/generated.ngc");

    //foo.save_obj("arcspiral.obj");

}



/***********************************/

void test_intersection(void){

    /* test of the function that calculates a line intersetion */ 

    double xcoord = 0;
    double ycoord = 0;

    double *xpt = &xcoord; 
    double *ypt = &ycoord; 

    get_line_intersection(-2, -3, 1, 2, -3, 2, 2, -2, xpt, ypt);
  
    cout << "test intersection: " << xcoord << ycoord << endl ;

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

            test_draw.rgbdata[pix_iterator].r = 254;  //= 0x254;      
            test_draw.rgbdata[pix_iterator].g = 0;  //= 0x0
            test_draw.rgbdata[pix_iterator].b = 255;  //= 0x0

        }
   }


   framebuffer::RGBType drawcolor;
   drawcolor.r = 0x255;
   drawcolor.g = 0x0;
   drawcolor.b = 0x0;

   /*
   ptest_draw->draw_point(2, 2, drawcolor);
   //ptest_draw->draw_point(11, 11, drawcolor);
   //ptest_draw->draw_point(12, 12, drawcolor);

   // instance method 
   ptest_draw->savebmp (outfile, width, height, 300, test_draw.rgbdata) ;
   // static/class method 
   //framebuffer::savebmp (outfile, width, height, 300, test_draw.rgbdata) ;
   */


   BMP new_outfile( width, height);
   new_outfile.dump_rgba_data(0, 0, width, height, test_draw.rgbdata);
   new_outfile.write( outfile) ;


} 

/***********************************/


void test_obj_stuff(void)
{
    /* test of loading and saving an OBJ file with my 3D model class*/

    model MOBJ;
    
    // MOBJ.make_cube(1.1);
    // MOBJ.make_circle( 10, 5.2);
    // MOBJ.make_line( .2);   
    // MOBJ.make_triangle( .2);
    // MOBJ.make_square(2);

    MOBJ.load_obj("3d_obj/cone.obj");

    //--------------
    // Vector3 nrml = MOBJ.get_triface_normal(0);
    // cout << "normal is " << nrml << "\n";

    //--------------

    
    //MOBJ.showinfo();
    //MOBJ.show();

    //MOBJ.op_triangulate();
    //MOBJ.flatten_edits();

    //MOBJ.showinfo();
    //MOBJ.show();

    //MOBJ.save_obj("3d_obj/triangle.obj");
    MOBJ.save_obj("gnocore.obj");

}


void test_point_ops(void)
{
    
    model* MOBJ = new(model);
    model* MOBJ2 = new(model);

    double scale = .5;

    // raw geom modeling 
    // MOBJ->obj_pts[0].set( -scale ,  0    , 0 );
    // MOBJ->obj_pts[1].set( 0      ,  scale, 0 );
    // MOBJ->obj_pts[2].set( scale  ,  0    , 0 );
    // MOBJ->vertex_count = 3;
    // MOBJ->add_tri(1,2,3);

    //-------------

    Vector3 p1 = Vector3(1 ,0 ,0);
    Vector3 p2 = Vector3(0 ,1 ,0);
    Vector3 p3 = Vector3(-1,0 ,0);
    MOBJ->add_tri(p1,p2,p3);

    //MOBJ2->vec3_as_line(p1);
    //MOBJ2->vec3_as_line(p2);

    //MOBJ2->vec3_as_line( );
 
    // triangle_centroid(Vector3 *out, Vector3 p1, Vector3 p2, Vector3 p3)
    
    MOBJ->vec3_as_line_atpos( MOBJ->triangle_pt_vec3(p1,p2,p3, Vector3(2,2,2) ), 
                              MOBJ->centroid( p1, p2, p3) );

    MOBJ->vec3_as_line_atpos( MOBJ->three_vec3_to_normal(p1,p2,p3, true ), 
                              MOBJ->centroid( p1, p2, p3) );

    //MOBJ->between_2vecs_as_line( p1, p2 );



    MOBJ->save_obj("point_ops.obj");
    //MOBJ2->save_obj("point_ops2.obj");


    delete(MOBJ);
    delete(MOBJ2);    

}


/***********************************/

/*

   TO DO LIST 

   I discovered a bug where it will hang forever if you pass a folder instaed of a file for the input.

*/


int main(int argc, char *argv[])
{

    //test_point_ops(); return 0;

    // test_load_save_ngc("3d_obj/arcspiral.ngc",""); return 0;

    /* * * * * * * * * */ 

 
    // --------------------------

    //test_framebuffer();

    //make_image(512, 512, "testimage.bmp");

    // --------------------------

    // ARGS: really_simple_render_model( int width, int height, char* objfilename, char* matrixfile, 
    //                                   float RX, float RY, float RZ, char* outfilename)
    
    int render_simple = 0;
   
    if(render_simple){ 
        really_simple_render_model( atoi(argv[1]), atoi(argv[2]), argv[3], argv[4], 
                                    atof(argv[5]), atof(argv[6]),  atof(argv[7]),  argv[8]);
    }else{
        // ARGS:  render_model( int width, int height, char* renderscript, char* outfilename)

        render_model( atoi(argv[1]), atoi(argv[2]), argv[3], argv[4]);
   
    }


    return 0;
}




