 
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


void make_image(int width, int height)
{
    /* 
        UNFINISHED - STAND ALONE GENERATE AN IMAGE 
    */

    int dpi    = 72; 
    //int width  = 512; 
    //int height = 512; 
    int res_x = width;
    int res_y = height;

    int n = width * height;

    framebuffer::RGBType* output_image;
    framebuffer myimage( width, height );
    output_image = myimage.rgbdata;
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

    //framebuffer::savebmp(outfilename , width, height, dpi, output_image);
    //cout << outfilename << " saved to disk. " << endl;
    //cout << "# Done Rendering ! " << endl;

}



/*********************************************************/

int get_line_intersection(float p0_x, float p0_y, float p1_x, float p1_y, 
    float p2_x, float p2_y, float p3_x, float p3_y, float *i_x, float *i_y)
{
    float s02_x, s02_y, s10_x, s10_y, s32_x, s32_y, s_numer, t_numer, denom, t;
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


Vector4 rotate_points ( Matrix4 m, Vector4 v) {
    /* multiply a vector4 by a matrix44 
       handled by the matrix library 
    */

    Vector4 out;
    out.x = m[0] * v.x + m[4] * v.y + m[8]  * v.z + m[12] * v.w;
    out.y = m[1] * v.x + m[5] * v.y + m[9]  * v.z + m[13] * v.w;
    out.z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w;
    out.w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w;
 
    return out;
}

/*********************************************************/


Vector3 rotate_points ( Matrix4 m, Vector3 v) {
    /* multiply a vector3 by a matrix44 
       handled by the matrix library 
    */

    Vector3 out;
    out.x = m[0] * v.x + m[4] * v.y + m[8]  * v.z + m[12] * 1;
    out.y = m[1] * v.x + m[5] * v.y + m[9]  * v.z + m[13] * 1;
    out.z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * 1;
    //out.w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * 1;

    return out;
}



/*********************************************************/

void really_simple_render_model( int width, int height, char* objfilename, 
                                 float RX, float RY, float RZ , 
                                 char* outfilename, double RSCALE)
{

   cout << "# Begin Rendering ..." << endl;

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
   rotate_obj.rotateY( RX );
   rotate_obj.rotateX( RY );
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
   


   // one problem is that rotate_points function returns a Vector4!! debug 
   //Vector4 tmp_vtx;

   Vector3 draw_poly[MAX_POLYGON_VERTS]; //polygon being drawn 

   //set this to number of faces , or 1 if rendering edges
   for (i=0;i<OBJ.face_count;i++)
   {
       int j = 0;
       
       int numverts = OBJ.faces[i].size();

       for (j=0;j<numverts;j++){
           // look up each face vertex (vector3 X4) 
           //cout << "point looked up is " << OBJ.obj_pts[ int(OBJ.faces[i][j])-1] << endl ;
           draw_poly[j] = rotate_points( rotate_obj, OBJ.obj_pts[ int(OBJ.faces[i][j])-1]);
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
               lineart.draw_line(scoord_x, scoord_y, ecoord_x, ecoord_y, poly_color);
           }
           
           if (RENDER_VTX_PTS){
           
               // if you want a circle at each point 
               // lineart.draw_circle(scoord_x, scoord_y, 3, vtx_color);
             
               // really big 5 pixel dot 
               lineart.draw_point(scoord_x   , scoord_y   , vtx_color); //if you only want a tiny point 
               lineart.draw_point(scoord_x+1 , scoord_y   , vtx_color);
               lineart.draw_point(scoord_x-1 , scoord_y   , vtx_color);
               lineart.draw_point(scoord_x   , scoord_y+1 , vtx_color);
               lineart.draw_point(scoord_x   , scoord_y-1 , vtx_color);
              
           }

       } 


   }//render iterator

   framebuffer::savebmp(outfilename , width, height, dpi, output_image);
   
   cout << outfilename << " saved to disk. " << endl;
   cout << "# Done Rendering ! " << endl;
}

/*********************************************************/

void draw_scanline( framebuffer* fb, 
                    float sx, float sy, float ex, float ey,  
                    float *phit_x , float *phit_y )
{

    framebuffer::RGBType scanline_color; 
    framebuffer::RGBType scanhit_color; 

    scanhit_color.r = 1;
    scanhit_color.g = 155;
    scanhit_color.b = 55;

    scanline_color.r = 75;
    scanline_color.g = 155;
    scanline_color.b = 1;

    get_line_intersection( 0.0     , 0.0    , 400.0    , 400.0, 
                           sx, sy, ex, ey, phit_x, phit_y) ; 

    fb->draw_line(0.0, 0.0,  400.0 ,  400.0 , scanline_color);
    fb->draw_circle(*phit_x , *phit_x   , 5, scanhit_color);

    cout << "hahaha line intersection at " << *phit_x <<" " << *phit_x << endl;  

}


void render_model( int width, int height, char* objfilename, 
                                 float RX, float RY, float RZ , char* outfilename, double RSCALE)
{

   cout << "# Begin Rendering ..." << endl;

   int RENDER_VTX_PTS   = 1;
   int RENDER_SCANLINE  = 1;

   int dpi    = 72; 
   //int width  = 512; 
   //int height = 512; 
   int res_x = width;
   int res_y = height;

   int n = width * height;

   framebuffer::RGBType* output_image;
   framebuffer lineart( width, height );
   framebuffer *p_lineart = &lineart;

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


   int i = 0;
   int j = 0;
   int plycount = 0;
   int pntcountsave_obj = 0;

   double scoord_x, scoord_y, ecoord_x, ecoord_y;

   
   rotate_obj.identity();

   //rotate_obj.show();
   // printf( "%d", rotate_obj );
   //rotate_obj.scale(1.8);
   //rotate_obj.translate(15,15,15);
   //rotate_obj.show();
  
   //adjust object rotation
   rotate_obj.rotateY( RX );
   rotate_obj.rotateX( RY );
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

   // one problem is that rotate_points function returns a Vector4!! debug 
   //Vector4 tmp_vtx;

   Vector3 draw_poly[MAX_POLYGON_VERTS]; //polygon being drawn 

   float    hit_x, hit_y;    
   float *p_hit_x = &hit_x; //line intersection hit X
   float *p_hit_y = &hit_y; //line intersection hit Y 

   //set this to number of faces , or 1 if rendering edges
   for (i=0;i<OBJ.face_count;i++)
   {
       int j = 0;
       
       int numverts = OBJ.faces[i].size();

       for (j=0;j<numverts;j++){
           // look up each face vertex (vector3 X4) 
           //cout << "point looked up is " << OBJ.obj_pts[ int(OBJ.faces[i][j])-1] << endl ;
           draw_poly[j] = rotate_points( rotate_obj, OBJ.obj_pts[ int(OBJ.faces[i][j])-1]);
           //cout << "rotated point is " << draw_poly[j] << j << endl; 

       }//

       // loop through the vertices for each face           
       for (j=0;j<numverts;j++)
       {
           // use the center of the screen as the point to draw geom from 
           double cenx =  (double)lineart.center_x;
           double ceny =  (double)lineart.center_y;
          
           // scale the geometry and translate it to the center of screen  
           scoord_x =  ((double)(draw_poly[j].x * RSCALE) + cenx);
           scoord_y =  ((double)(draw_poly[j].y * RSCALE) + ceny);

           if (j<numverts-1)
           {
               ecoord_x =  ((double)(draw_poly[j+1].x * RSCALE) + cenx ); 
               ecoord_y =  ((double)(draw_poly[j+1].y * RSCALE) + ceny ); 
               // cout << "draw a line " << scoord_x << " " << scoord_y << " " << ecoord_x << " " << ecoord_y << endl;
               lineart.draw_line(scoord_x, scoord_y, ecoord_x, ecoord_y, poly_color);
               
               /*******************/ 
               if (RENDER_SCANLINE){
                   // get_line_intersection( 0.0     , 0.0    , 400.0    , 400.0, 
                   //                        scoord_x, scoord_y, ecoord_x, ecoord_y, p_hit_x, p_hit_y) ; 
                   // lineart.draw_line(0.0, 0.0,  400.0 ,  400.0 , scanline_color);
                   // lineart.draw_circle(hit_x , hit_y   , 5, scanhit_color);
                   // cout << "line intersection at " << hit_x <<" " << hit_y << endl;
                  
                   draw_scanline( p_lineart, scoord_x, scoord_y, 
                                  ecoord_x, ecoord_y, p_hit_x , p_hit_y );

               }
               /*******************/ 

           }


           // last line segment 
           if (j==numverts-1)
           {
               scoord_x =  ((double)(draw_poly[0].x * RSCALE) + cenx);
               scoord_y =  ((double)(draw_poly[0].y * RSCALE) + ceny);
               ecoord_x =  ((double)(draw_poly[j].x * RSCALE) + cenx );//+lineart.center_x
               ecoord_y =  ((double)(draw_poly[j].y * RSCALE) + ceny );//+lineart.center_y 
               lineart.draw_line(scoord_x, scoord_y, ecoord_x, ecoord_y, poly_color);
           }


           if (RENDER_VTX_PTS){
               // if you want a circle at each point 
               //lineart.draw_circle(scoord_x, scoord_y, 3, vtx_color);
               
               // really big 5 pixel dot 
               lineart.draw_point(scoord_x   , scoord_y   , vtx_color); //if you only want a tiny point 
               lineart.draw_point(scoord_x+1 , scoord_y   , vtx_color);
               lineart.draw_point(scoord_x-1 , scoord_y   , vtx_color);
               lineart.draw_point(scoord_x   , scoord_y+1 , vtx_color);
               lineart.draw_point(scoord_x   , scoord_y-1 , vtx_color);
           }

       } 


   }//render iterator

   framebuffer::savebmp(outfilename , width, height, dpi, output_image);
   
   cout << outfilename << " saved to disk. " << endl;
   cout << "# Done Rendering ! " << endl;
}
