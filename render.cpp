 
#include <iostream>
#include <stdio.h>

#include "include/Vectors.h"
#include "include/Matrices.h"

#include "include/framebuffer.h"
#include "include/render.h"
#include "include/model.h"


using namespace std;

#define MAX_POLYGON_VERTS 100

int pix_iterator;//index to a pixel on the framebuffer



/*********************************************************/


void make_image(int width, int height, char *outfile)
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

   ptest_draw->draw_point(10, 10, drawcolor);
   ptest_draw->draw_point(11, 11, drawcolor);
   ptest_draw->draw_point(12, 12, drawcolor);

   // instance method 
   ptest_draw->savebmp (outfile, width, height, 300, test_draw.rgbdata) ;

   //static/class method 
   //framebuffer::savebmp (outfile, width, height, 300, test_draw.rgbdata) ;

}



/*********************************************************/
/*
    intersect two lines in 2D
*/

int get_line_intersection(double p0_x, double p0_y, double p1_x, double p1_y, 
    double p2_x, double p2_y, double p3_x, double p3_y, double *i_x, double *i_y)
{
    double s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom, t;
    s10_x = p1_x - p0_x;
    s10_y = p1_y - p0_y;
    s32_x = p3_x - p2_x;
    s32_y = p3_y - p2_y;

    denom = s10_x * s32_y - s32_x * s10_y;
    if (denom == 0)
        return 0; // Collinear
    bool denomPositive = denom > 0;

    s02_x = p0_x - p2_x;
    s02_y = p0_y - p2_y;
    s_numer = s10_x * s02_y - s10_y * s02_x;
    if ((s_numer < 0) == denomPositive)
        return 0; // No collision

    t_numer = s32_x * s02_y - s32_y * s02_x;
    if ((t_numer < 0) == denomPositive)
        return 0; // No collision

    if (((s_numer > denom) == denomPositive) || ((t_numer > denom) == denomPositive))
        return 0; // No collision
    // Collision detected
    t = t_numer / denom;
    if (i_x != NULL)
        *i_x = p0_x + (t * s10_x);
    if (i_y != NULL)
        *i_y = p0_y + (t * s10_y);

    return 1;
}




/*********************************************************/
/*
    placeholder for clipping function 
    crop geom to a 2D rectangle 
    return 1/0 if geom was clipped to test for intersection
*/
int poly_clip(int width, int height, double *x1, double *y1, double *x2, double *y2)
{
    int out = 0;

    if(*x1>= width) { *x1 = (double)width  ;out++;}
    if(*y1>= height){ *y1 = (double)height ;out++;}
    if(*x2>= width) { *x2 = (double)width  ;out++;}
    if(*y2>= height){ *y2 = (double)height ;out++;}     
    if(*x1 < 0.0)   { *x1 = (double)0.0    ;out++;}
    if(*y1 < 0.0)   { *y1 = (double)0.0    ;out++;}
    if(*x2 < 0.0)   { *x2 = (double)0.0    ;out++;}
    if(*y2 < 0.0)   { *y2 = (double)0.0    ;out++;}  

    return out;
}

/*********************************************************/
/*
   take a clipping rectangle and 2 lines
   sets pointes to 2 (x,y) coords (line between the 2 intersections) 
   returns number of failures to intersect. 
      0 = both intersected
      1 = one intersected 
      2 = neither line intersected 
*/


int raster_clip(   double x1  , double y1  , double x2  , double y2  ,   // input line (raster) 
                   double ix1 , double iy1 , double ix2 , double iy2 ,   // line1 to intersect
                   double jx1 , double jy1 , double jx2 , double jy2 ,   // line2 to intersect
                   double *ox1, double *oy1, double *ox2, double *oy2)   // output line, if any  
{
   
    int out = 0;

    // intersect the orthoganal raster line to arbitrary geometry
    out = out + get_line_intersection( x1,  y1 , x2 , y2 , 
                                      ix1, iy1, ix2, iy2,  ox1, oy1) ; 
    // intersect the orthoganal raster line to arbitrary geometry 
    out = out + get_line_intersection( x1,  y1 , x2 , y2 , 
                                      jx1, jy1, jx2, jy2,  ox2, oy2) ; 

    return out;

}

/*********************************************************/


void draw_triangle( double rscale, framebuffer* fb, Vector3 p1, Vector3 p2, Vector3 p3 , framebuffer::RGBType fillcolor)
{
    int RENDER_VTX_PTS      = 1;
    int RENDER_LINES        = 1; // wireframe edges  
    int RENDER_SCANLINE     = 1; // do the scan line render loop 
    int SHOW_SCANLINE       = 1; // show the actual "line" indicating where the scan is happening 
    int SHOW_POLYFILL       = 1; // fill the polygon 

    framebuffer::RGBType poly_color; 
    framebuffer::RGBType vtx_color; 
    framebuffer::RGBType scanline_color; 
    framebuffer::RGBType scanhit_color; 

    //set colors
    poly_color.r = 64;
    poly_color.g = 56;
    poly_color.b = 27;

    vtx_color.r = 255;
    vtx_color.g = 255;
    vtx_color.b = 255;

    scanhit_color.r = 1;
    scanhit_color.g = 155;
    scanhit_color.b = 55;
    scanline_color.r = 75;
    scanline_color.g = 155;
    scanline_color.b = 1;  
  
    // use the center of the screen as the point to draw geom from 
    double cenx = (double)fb->center_x;
    double ceny = (double)fb->center_y;
    double scx1, scy1, ecx1, ecy1, 
           scx2, scy2, ecx2, ecy2, 
           scx3, scy3, ecx3, ecy3;

    //------------------------- 
    // define clipping rectangle 
    double clp_x1  = 5;
    double clp_y1  = 5;
    double clp_x2  = 500;
    double clp_y2  = 500;
    double clipwidth  = abs(clp_x2-clp_x1);
    double clipheight = abs(clp_y2-clp_y1);

    // view clip rectangle 
    if (0)
    {
        fb->draw_line(clp_x1, clp_y1, clp_x2, clp_y1, fillcolor);
        fb->draw_line(clp_x2, clp_y1, clp_x2, clp_y2, fillcolor);
        fb->draw_line(clp_x2, clp_y2, clp_x1, clp_y2, fillcolor);
        fb->draw_line(clp_x1, clp_y2, clp_x1, clp_y1, fillcolor);
    }     
    //------------------------- 

    // int cl_it = 0; //clip iterator ;)
    // for (cl_it=*y1;cl_it<*y2;cl_it++){};//scan clip rectangle 
    
    //store the hits that raster_clip() finds  
    double   h1x,  h1y,  h2x,  h2y,  h3x,  h3y = 0.0; 
    double* ph1x = &h1x; 
    double* ph1y = &h1y; 
    double* ph2x = &h2x; 
    double* ph2y = &h2y;
    double* ph3x = &h3x;
    double* ph3y = &h3y;


    // scale pt1 and translate it to the center of screen  
    scx1 =  ((double)(p1.x * rscale) + cenx );
    scy1 =  ((double)(p1.y * rscale) + ceny );
    ecx1 =  ((double)(p2.x * rscale) + cenx ); 
    ecy1 =  ((double)(p2.y * rscale) + ceny ); 

    // scale pt2 and translate it to the center of screen     
    scx2 =  ((double)(p2.x * rscale) + cenx );
    scy2 =  ((double)(p2.y * rscale) + ceny );
    ecx2 =  ((double)(p3.x * rscale) + cenx ); 
    ecy2 =  ((double)(p3.y * rscale) + ceny ); 

    double y_val = 300.0;
    int num_hits = 0;
    
    num_hits = raster_clip( clp_x1 , y_val  , clp_x2 , y_val ,  // input line (raster) 
                            scx1   , scy1    , ecx1   , ecy1   ,  // line1 to intersect
                            scx2   , scy2    , ecx2   , ecy2   ,  // line2 to intersect
                            ph1x   , ph1y    , ph2x   , ph2y );  // output line, if any 

    fb->draw_line(clp_x1, y_val, clp_x2, y_val, fillcolor);  //full scanline
    
    cout <<" num hits " << num_hits << "\n";

    if (num_hits==2){
        cout << "we have a hit! " << (int)*ph1x <<" "<< (int)*ph1y <<" "<< (int)*ph2x << " " << (int)*ph2y <<"\n";
        fb->draw_line((int)*ph1x, (int)*ph1y, (int)*ph2x, (int)*ph2y, scanline_color); //polygon fill
    }
 

    // scale pt3 and translate it to the center of screen     
    scx3 =  ((double)(p3.x * rscale) + cenx );
    scy3 =  ((double)(p3.y * rscale) + ceny );
    ecx3 =  ((double)(p1.x * rscale) + cenx ); 
    ecy3 =  ((double)(p1.y * rscale) + ceny ); 


    fb->draw_line(scx1, scy1, ecx1, ecy1, fillcolor);
    fb->draw_line(scx2, scy2, ecx2, ecy2, fillcolor);
    fb->draw_line(scx3, scy3, ecx3, ecy3, fillcolor);

    //------------------------- 

    
    
    if (RENDER_SCANLINE == 1)
    {


    }// render scanlines
  
    /*  
   
    // third "virtual" line segment 
    // I call it virtual because it connects the last drawn point to the first 
    // making the polyline periodic 
    
    /*
    // last line segment 
    if (j==numverts-1)
    {
        scoord_x =  ((double)(draw_poly[0].x * RSCALE) + cenx);
        scoord_y =  ((double)(draw_poly[0].y * RSCALE) + ceny);
        ecoord_x =  ((double)(draw_poly[j].x * RSCALE) + cenx );//+lineart.center_x
        ecoord_y =  ((double)(draw_poly[j].y * RSCALE) + ceny );//+lineart.center_y 

        //draw the closing third edge of the polygon 
        poly_clip(width, height, &scoord_x, &scoord_y, &ecoord_x, &ecoord_y);                     
        lineart.draw_line(scoord_x, scoord_y, ecoord_x, ecoord_y, fillcolor);

        //////////////////////////////////

        // run the calc function to check for a line intersection 
        get_line_intersection( 0.0,  (float)sl, (float)width  , (float)sl, 
                               scoord_x, scoord_y, ecoord_x, ecoord_y, p_hit_x, p_hit_y) ; 

        if(RENDER_SCANLINE==1){

            if (hit_x>0 && hit_y>0){
                if (SHOW_SCANLINE_DOTS==1)
                {
                    int c = 0;
                    for(c=0;c<4;c++){

                        lineart.draw_circle(hit_x , hit_y , c, scanhit_color);
                    }
                }

                l3x = hit_x;
                l3y = hit_y;
                                    
            }//if hit detected 
        }
     }//last line segment               
     */

    //draw a dot to show the polygon vertices  
    if (RENDER_VTX_PTS==1)
    {
        int wasclipped = 0;
        wasclipped = poly_clip(clipwidth, clipheight, &scx1, &scy1, &ecx1, &ecy1);                  
        // really big 5 pixel dot 
        if(wasclipped==0)
        {
            fb->draw_point(scx1   , scy1   , fillcolor); //if you only want a tiny point 
            fb->draw_point(scx1+1 , scy1   , fillcolor);
            fb->draw_point(scx1-1 , scy1   , fillcolor);
            fb->draw_point(scx1   , scy1+1 , fillcolor);
            fb->draw_point(scx1   , scy1-1 , fillcolor);
        }
    }

    /*
    // draw a line bewteen the two intersection points 
    if (SHOW_POLYFILL==1){
         //cout << ' ' << l1x << ' ' << l1y <<' '<< l2x << ' ' << l2y <<' ' << l3x << ' ' <<l3y << endl;
         poly_clip(clipwidth, clipheight, &l1x, &l1y, &l2x, &l2y);  
         //draw all three lines if the data exists
         if( l1x != 0 && l2x !=0){
             fb->draw_line(l1x, l1y, l2x, l2y, poly_color);
         }
         if( l1x != 0 && l3x !=0){
             fb->draw_line(l1x, l1y, l3x, l3y, poly_color);
         }
         if( l2x != 0 && l3x !=0){
             fb->draw_line(l2x, l2y, l3x, l3y, poly_color);
         }                

    }*/

    
}
/*********************************************************/


void render_model( int width, int height, char* objfilename, char* matrixfile, 
                                 float RX, float RY, float RZ , char* outfilename)
{

   int dpi = 72; 

   int n = width * height;

   framebuffer::RGBType* output_image;
   framebuffer lineart( width, height );
   framebuffer *p_lineart = &lineart;

   output_image = lineart.rgbdata;

   int which = 1;

    framebuffer::RGBType polyline_color; 
    
    polyline_color.r = 170;
    polyline_color.g = 22;
    polyline_color.b = 170;

   /***********/
   //clear the background  with a solid color
   short flat_color = 0; //background grey color 

   //fill each pixel with a color  
   for (int x = 0; x < width; x++)
   {    //rotate_obj.show();
        for (int y = 0; y < height; y++)
        {  
            pix_iterator = y * width + x;     

            //funky moire pattern 
            //output_image[pix_iterator].r = y/2;       
            //output_image[pix_iterator].g = 255;//-y/3;
            //output_image[pix_iterator].b = x*(y/2);

            output_image[pix_iterator].r = flat_color;       
            output_image[pix_iterator].g = flat_color;
            output_image[pix_iterator].b = flat_color;

        }
   }

   /***********/


   //containers for 3d objects 
   model OBJ;
   OBJ.load_obj(objfilename);

   /***********/
   Vector2 thisedge;      //iterator for edges
   Vector4 poly;          //store vertex id's in a vector4 (4 sided poly) 
   Vector3 poly3;         //store vertex id's in a vector3 (3 sided poly) 
   Vector4 vprj[4];       //3 and 4 sided -  projected point coordinates
   Vector3 v1,v2,v3,v4;   //retrieved point data (xyz)

   int i = 0;
   int j = 0;
   int plycount = 0;
   int pntcountsave_obj = 0;


   //debug - min the process of rendering N sided - converting from Vector4
   //double draw_poly[MAX_POLYGON_VERTS]; //polygon being drawn 
   //Vector3 draw_poly[MAX_POLYGON_VERTS]; //polygon being drawn 

   /***********************/

   Matrix4 rotate_obj;    //4X4 rotation matrix
   
   rotate_obj.identity();

   //rotate_obj.show();
   // printf( "%d", rotate_obj );
   //rotate_obj.scale(1.8);
   //rotate_obj.translate(15,15,15);
   //rotate_obj.show();
  
   //adjust object rotation
   rotate_obj.rotateX( RX );
   rotate_obj.rotateY( RY );
   rotate_obj.rotateZ( RZ );

   //load the camera matrix (via model) and push points around 
   model camera_matrix;
   camera_matrix.load_matrix( matrixfile );
   float RSCALE = 1000.0/abs(camera_matrix.m44[14]); //hack to scale object based on Z xform 
   rotate_obj = rotate_obj * camera_matrix.m44;

   /***********************/

   // set this to number of faces , or 1 if rendering edges
   for (i=0;i<OBJ.face_count;i++)
   {
       int j = 0;
       
       int numverts = OBJ.faces[i].size();
       
       //only draw triangles for now 
       if (numverts==3)
       {
           // look up each face vertex (vector3 X4) 
           //cout << "point looked up is " << OBJ.obj_pts[ int(OBJ.faces[i][j])-1] << endl ;

           Vector3 p1 = rotate_obj * OBJ.obj_pts[ int(OBJ.faces[i][0])-1];
           Vector3 p2 = rotate_obj * OBJ.obj_pts[ int(OBJ.faces[i][1])-1];
           Vector3 p3 = rotate_obj * OBJ.obj_pts[ int(OBJ.faces[i][2])-1];

           // non rotated geom   
           // Vector3 p1 = OBJ.obj_pts[ int(OBJ.faces[i][0])-1];
           // Vector3 p2 = OBJ.obj_pts[ int(OBJ.faces[i][1])-1];
           // Vector3 p3 = OBJ.obj_pts[ int(OBJ.faces[i][2])-1];

           draw_triangle( RSCALE, p_lineart, p1, p2, p3 , polyline_color);
        }


       //done with scanline loop here
       /****************************************************/

       /*
       if (SHOW_SCANLINE==1){
           // show the scanline we are feeding into the calc function 
           //poly_clip(width, height, &l1x, &l1y, &l2x, &l2y);  
           lineart.draw_line(0.0 , (float)sl, (float)width, (float)sl , scanline_color);
       } 

       // line bewteen the two intersection points 
       // draw once more to cover everything else 
       if (SHOW_POLYFILL==1){
            //cout << ' ' << l1x << ' ' << l1y <<' '<< l2x << ' ' << l2y <<' ' << l3x << ' ' <<l3y << endl;
            poly_clip(width, height, &l1x, &l1y, &l2x, &l2y);  
            //draw all three lines if the data exists
            if( l1x != 0 && l2x !=0){
                lineart.draw_line(l1x, l1y, l2x, l2y, vtx_color);
            }
            if( l1x != 0 && l3x !=0){
                lineart.draw_line(l1x, l1y, l3x, l3y, vtx_color);
            }
            if( l2x != 0 && l3x !=0){
                lineart.draw_line(l2x, l2y, l3x, l3y, vtx_color);
            }                

       }
       */

   }//render iterator

   framebuffer::savebmp(outfilename , width, height, dpi, output_image);
   
   //cout << outfilename << " saved to disk. " << endl;
   cout << "finished rendering." << endl;
}







/*********************************************************/
/*********************************************************/


void really_simple_render_model( int width, int height, char* objfilename, char* matrixfile, 
                                 float RX, float RY, float RZ, char* outfilename)
{

   //cout << "# Begin Rendering ..." << endl;

   int dpi    = 72; 
   //int width  = 512; 
   //int height = 512; 
   int res_x = width;
   int res_y = height;

   int n = width * height;

   framebuffer::RGBType* output_image;
   framebuffer lineart( width, height );
   output_image = lineart.rgbdata;

   /***********/
   short flat_color = 0; //background grey color 

   //fill each pixel with a color  
   for (int x = 0; x < width; x++)
   {    //rotate_obj.show();
        for (int y = 0; y < height; y++)
        {  
            pix_iterator = y * width + x;     

            //funky moire pattern 
            // output_image[pix_iterator].r = y/2;       
            // output_image[pix_iterator].g = 255;//-y/3;
            // output_image[pix_iterator].b = x*(y/2);

            output_image[pix_iterator].r = flat_color;       
            output_image[pix_iterator].g = flat_color;
            output_image[pix_iterator].b = flat_color;

        }
   }

   framebuffer::RGBType poly_color; 
   framebuffer::RGBType vtx_color; 

   //containers for 3d objects 
   model OBJ;
   OBJ.load_obj(objfilename);

   /***********/
   Vector2 thisedge;      //iterator for edges
   Vector4 poly;          //store vertex id's in a vector4 (4 sided poly) 
   Vector3 poly3;         //store vertex id's in a vector3 (3 sided poly) 
   Vector4 vprj[4];       //3 and 4 sided -  projected point coordinates
   Vector3 v1,v2,v3,v4;   //retrieved point data (xyz)
   Matrix4 rotate_obj;    //4X4 rotation matrix 


   int i =0;
   int j =0;
   int plycount = 0;
   int pntcountsave_obj = 0;

   int RENDER_VTX_PTS = 1;

   double scoord_x, scoord_y, ecoord_x, ecoord_y;

   
   rotate_obj.identity();

   //rotate_obj.show();
   // printf( "%d", rotate_obj );
   //rotate_obj.scale(1.8);
   //rotate_obj.translate(15,15,15);
   //rotate_obj.show();
  
   //adjust object rotation
   rotate_obj.rotateY( RY );
   rotate_obj.rotateX( RX );
   rotate_obj.rotateZ( RZ );

   /***********************/
   //set colors
   poly_color.r = 170;
   poly_color.g = 22;
   poly_color.b = 170;

   vtx_color.r = 1;
   vtx_color.g = 255;
   vtx_color.b = 255;

   //debug - min the process of rendering N sided - converting from Vector4
   //double draw_poly[MAX_POLYGON_VERTS]; //polygon being drawn 
   Vector3 draw_poly[MAX_POLYGON_VERTS]; //polygon being drawn 

   // -----------------

   //load the camera matrix (via model) and push points around 
   model camera_matrix;
   camera_matrix.load_matrix( matrixfile );
   float RSCALE = 1000.0/abs(camera_matrix.m44[14]);

   rotate_obj = rotate_obj * camera_matrix.m44;

   // framebuffer draws "upside down" flip the Y with a matrix 
   
   // Matrix4 flip_y;    
   // flip_y.identity();
   // flip_y.rotateZ(180);
   // //  |  0  4  8 12 |
   // //  |  1  5  9 13 |
   // //  |  2  6 10 14 |
   // //  |  3  7 11 15 |     
   // rotate_obj = rotate_obj * flip_y;
   // ---------------------

   //set this to number of faces , or 1 if rendering edges
   for (i=0;i<OBJ.face_count;i++)
   {
       int j = 0;
       
       int numverts = OBJ.faces[i].size();

       for (j=0;j<numverts;j++){
           // look up each face vertex (vector3 X4) 
           //cout << "point looked up is " << OBJ.obj_pts[ int(OBJ.faces[i][j])-1] << endl ;
           
           // I did this thinking it was needed , BUT the matrix lib can do it directly. and better ?
           //draw_poly[j] = rotate_points( rotate_obj, OBJ.obj_pts[ int(OBJ.faces[i][j])-1]);
           
           // this will use the matrix library directly 
           draw_poly[j] = rotate_obj * OBJ.obj_pts[ int(OBJ.faces[i][j])-1] ;

           //cout << "rotated point is " << draw_poly[j] << j << endl; 
       }//

       // loop through the vertices for each face           
       for (j=0;j<numverts;j++)
       {
           // cout << vprj[j].x << " "<< vprj[j].x << " " << vprj[j].z << endl;

           //scoord_x =  (draw_poly[j].x * lineart.center_x * ZVAL) + lineart.center_x;
           //scoord_y =  (draw_poly[j].y * lineart.center_y * ZVAL) + lineart.center_y;
           double cenx =  (double)lineart.center_x;
           double ceny =  (double)lineart.center_y;
          
           // cout << "center_x center_y " <<  cenx << " " << ceny;

           scoord_x =  ((double)(draw_poly[j].x * RSCALE) + cenx);
           scoord_y =  ((double)(draw_poly[j].y * RSCALE) + ceny);

           if (j<numverts-1)
           {
               ecoord_x =  ((double)(draw_poly[j+1].x * RSCALE) + cenx );//+lineart.center_x
               ecoord_y =  ((double)(draw_poly[j+1].y * RSCALE) + ceny );//+lineart.center_y 

               // cout << "draw a line " << scoord_x << " " << scoord_y << " " << ecoord_x << " " << ecoord_y << endl;
               //poly_clip(width, height, &scoord_x, &scoord_y, &ecoord_x, &ecoord_y);
               lineart.draw_line(scoord_x, scoord_y, ecoord_x, ecoord_y, poly_color);

               // get_line_intersection

           }

           // last line segment 
           if (j==numverts-1)
           {
               scoord_x =  ((double)(draw_poly[0].x * RSCALE) + cenx);
               scoord_y =  ((double)(draw_poly[0].y * RSCALE) + ceny);
               ecoord_x =  ((double)(draw_poly[j].x * RSCALE) + cenx );//+lineart.center_x
               ecoord_y =  ((double)(draw_poly[j].y * RSCALE) + ceny );//+lineart.center_y 
               
               //poly_clip(width, height, &scoord_x, &scoord_y, &ecoord_x, &ecoord_y);
               lineart.draw_line(scoord_x, scoord_y, ecoord_x, ecoord_y, poly_color);
           }
           
           if (RENDER_VTX_PTS){
           
               // if you want a circle at each point 
               // lineart.draw_circle(scoord_x, scoord_y, 3, vtx_color);
               int wasclipped = poly_clip(width, height, &scoord_x, &scoord_y, &ecoord_x, &ecoord_y);             
               // really big 5 pixel dot 
               if (wasclipped==0){
                   lineart.draw_point(scoord_x   , scoord_y   , vtx_color); //if you only want a tiny point 
                   lineart.draw_point(scoord_x+1 , scoord_y   , vtx_color);
                   lineart.draw_point(scoord_x-1 , scoord_y   , vtx_color);
                   lineart.draw_point(scoord_x   , scoord_y+1 , vtx_color);
                   lineart.draw_point(scoord_x   , scoord_y-1 , vtx_color);
               }
           }

       } 


   }//render iterator

   framebuffer::savebmp(outfilename , width, height, dpi, output_image);
   
   //cout << outfilename << " saved to disk. " << endl;
   //cout << "# Done Rendering ! " << endl;
}




/*********************************************************/

void test_framebuffer(void)
{
    int width = 512;
    int height = 512;
    int dpi    = 72; 

    framebuffer::RGBType poly_color; 
    framebuffer::RGBType vtx_color; 

    //set colors
    poly_color.r = 170;
    poly_color.g = 22;
    poly_color.b = 170;
    vtx_color.r = 1;
    vtx_color.g = 255;
    vtx_color.b = 255;

    framebuffer lineart( width, height );
    framebuffer::RGBType* output_image;

    output_image = lineart.rgbdata;

    lineart.draw_point(1,1);
    lineart.draw_point(5,5);    
    lineart.draw_point(10,10);
    //lineart.draw_line(0, 0, width, 0, poly_color);

    // saveBMP_24bit ( RGBType *data, const char *filename, int w, int h); 
    framebuffer::savebmp("fb_test.bmp" , width, height, dpi, output_image);    
}

