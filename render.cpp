 
#include <iostream>
#include <stdio.h>

#include "include/Vectors.h"
#include "include/Matrices.h"

#include "include/framebuffer.h"
#include "include/render.h"
#include "include/model.h"


using namespace std;


int pix_iterator;//index to a pixel on the framebuffer



/*********************************************************/


void make_image(int width, int height)
{

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



/* intersect 2 lines in 2D 
    v1s - line1 start 
    v1e - line1 end 
    v2s - line2 start
    v2e - line2 end 
*/

void intersect_2d (void)
{

    // start and end coords for two 2D lines
    // long p0_x = 0; 
    // long p0_y = 0; 
    // long p1_x = 0; 
    // long p1_y = 0; 
    // long p2_x = 0; 
    // long p2_y = 0; 
    // long p3_x = 0; 
    // long p3_y = 0; 


    long p0_x = -4; 
    long p0_y = -4; 
    long p1_x = -4; 
    long p1_y = 0; 
    long p2_x = 4; 
    long p2_y = 4; 
    long p3_x = -7; 
    long p3_y = -7; 


    // return values
    long i_x = 0;
    long i_y = 0; 

    long s1_x = 0;
    long s1_y = 0;
    long s2_x = 0;
    long s2_y = 0;

    s1_x = p1_x - p0_x;  
    s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;
    s2_y = p3_y - p2_y;

    long s = 0;
    long t = 0;

    //try: 
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);
    //except:
    //    // print('something went wrong in 2D line intersection')
    //    return 0
    
    if (s >= 0 and s <= 1 and t >= 0 and t <= 1){
        // Collision detected
        i_x = p0_x + (t * s1_x);
        i_y = p0_y + (t * s1_y);
        
        //return (i_x,i_y);
        cout << "intersection is " << i_x << " " << i_y << endl;  
    
    }

    //return 0; // No collision

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


Vector4 rotate_points ( Matrix4 m, Vector3 v) {
    /* multiply a vector3 by a matrix44 
       handled by the matrix library 
    */

    Vector4 out;
    out.x = m[0] * v.x + m[4] * v.y + m[8]  * v.z + m[12] * 1;
    out.y = m[1] * v.x + m[5] * v.y + m[9]  * v.z + m[13] * 1;
    out.z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * 1;
    out.w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * 1;

    return out;
}


/*********************************************************/

void really_simple_render_model( int width, int height, char* objfilename, float RX, float RY, float RZ , char* outfilename){

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

   int RENDER_PTS = 1;
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

   //set this to number of faces , or 1 if rendering edges
   for (i=0;i<OBJ.face_count;i++)
   {
       if (OBJ.is_four_sided)
       {
           //RENDER 4 SIDED POLYGONS
           poly=OBJ.faces[i]; //store 4 verts in a vector4
   
           //look up each face vertex (vector3 X4) 
           v1=OBJ.obj_pts[int(poly.x)];
           v2=OBJ.obj_pts[int(poly.y)];
           v3=OBJ.obj_pts[int(poly.z)];
           v4=OBJ.obj_pts[int(poly.w)];
        
           //calculate point rotations
           vprj[0]=rotate_points(rotate_obj,v1);
           vprj[1]=rotate_points(rotate_obj,v2);
           vprj[2]=rotate_points(rotate_obj,v3);   
           vprj[3]=rotate_points(rotate_obj,v4);

           cout << "Rendering 4 sided polygon "<< plycount << endl; plycount++;

           //render four sided polygon (step through 4 verts and connect the dots) 
           for (j=0;j<4;j++)
           {
               double ZVAL = .5 ;
               cout << vprj[j].x << " "<< vprj[j].x << " " << vprj[j].z << endl;

               scoord_x =  (vprj[j].x   *lineart.center_x*ZVAL)+lineart.center_x;
               scoord_y =  (vprj[j].y   *lineart.center_y*ZVAL)+lineart.center_y;

               if (j<3)
               {
                   ecoord_x =  (vprj[j+1].x   *lineart.center_x*ZVAL)+lineart.center_x;
                   ecoord_y =  (vprj[j+1].y   *lineart.center_y*ZVAL)+lineart.center_y;
                   lineart.draw_line(scoord_x, scoord_y, ecoord_x, ecoord_y, poly_color);
               }
               if (RENDER_PTS){
                   //lineart.draw_circle(scoord_x, scoord_y, 3, vtx_color);
                   lineart.draw_point(scoord_x   , scoord_y  , vtx_color);
                   lineart.draw_point(scoord_x+1 , scoord_y  , vtx_color);
                   lineart.draw_point(scoord_x-1 , scoord_y  , vtx_color);
                   lineart.draw_point(scoord_x   , scoord_y+1 , vtx_color);
                   lineart.draw_point(scoord_x   , scoord_y-1 , vtx_color);
               }

           } 

       }//draw 4 sided polygons  

       /***********/
             
       //RENDER THREE SIDED POLYGONS
       if (OBJ.is_three_sided)
       {
           poly3=OBJ.faces3[i]; //store 3 verts in a vector4
    
           //look up each face vertex (vector3 <vtx1,vtx2,vtx3>) 
           v1=OBJ.obj_pts[int(poly3.x)];
           v2=OBJ.obj_pts[int(poly3.y)];
           v3=OBJ.obj_pts[int(poly3.z)];
     
           //calculate point rotationargv[0], argv[1], argv[2], argv[3],argv[4],argv[5]);s
           vprj[0]=rotate_points(rotate_obj,v1);
           vprj[1]=rotate_points(rotate_obj,v2);
           vprj[2]=rotate_points(rotate_obj,v3);

           // cout << "Rendering 3 sided polygon "<< plycount << endl; plycount++;

           //render 3 sided polygon (step through 3 verts and connect the dots) 
           for (j=0;j<3;j++)
           {
               scoord_x =  (vprj[j].x   *lineart.center_x/2)+lineart.center_x;
               scoord_y =  (vprj[j].y   *lineart.center_y/2)+lineart.center_y;
           
               if (j<2)
               {
                   ecoord_x =  (vprj[j+1].x *lineart.center_x/2)+lineart.center_x;
                   ecoord_y =  (vprj[j+1].y *lineart.center_y/2)+lineart.center_y;
                   lineart.draw_line(scoord_x, scoord_y, ecoord_x, ecoord_y, poly_color);
               }
           
               if (RENDER_PTS)
               {
                   //lineart.draw_circle(scoord_x, scoord_y, 3, vtx_color);
                   lineart.draw_point(scoord_x   , scoord_y   , vtx_color);
                   lineart.draw_point(scoord_x+1 , scoord_y   , vtx_color);
                   lineart.draw_point(scoord_x-1 , scoord_y   , vtx_color);
                   lineart.draw_point(scoord_x   , scoord_y+1 , vtx_color);
                   lineart.draw_point(scoord_x   , scoord_y-1 , vtx_color);
               }
           }

       }//draw 3 sided polygons

   }//render iterator

   framebuffer::savebmp(outfilename , width, height, dpi, output_image);
   
   cout << outfilename << " saved to disk. " << endl;
   cout << "# Done Rendering ! " << endl;
}

/*********************************************************/


void render_model( int width, int height, char* objfilename, float RX, float RY, float RZ , char* outfilename)
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

   /***********/
   framebuffer::RGBType poly_color; 
   framebuffer::RGBType vtx_color; 
   /***********/

   //DEMO FUNCTIONS
   //lineart.draw_point(lineart.center_x ,lineart.center_y);
   //lineart.draw_circle( lineart.center_x ,lineart.center_y , 50);
   //lineart.draw_line(lineart.center_x, 100, lineart.center_x, 200);

   /***********/

   //containers for 3d objects 
   model OBJ;
   OBJ.load_obj(objfilename);

   //OBJ.load_obj("other/obj/sphere.obj");



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

   int RENDER_PTS = 1;
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
 


   //camera.rotateY(12);
   //rotate_obj=camera*rotate_obj;

   /***********************/

   //set colors
   poly_color.r = 170;
   poly_color.g = 22;
   poly_color.b = 170;

   vtx_color.r = 1;
   vtx_color.g = 255;
   vtx_color.b = 255;

   //set this to number of faces , or 1 if rendering edges
   for (i=0;i<OBJ.face_count;i++){
       /*
       //RENDER POLYGON EDGES (set i<1 in loop)
       for(j=0;j<12;j++){
          thisedge = OBJ.edges[j];
          v1=OBJ.obj_pts[int( thisedge.x)].set_xyz( 11.1, 22.1, 33.1);;
          v2=OBJ.obj_pts[int( thisedge.y)];

          //project start-end vertecies into 2d and draw
          vtx_rot=rotate_points(rotate_obj,v1);
          scoord_x =  (vtx_rot.x *lineart.center_x/2)+lineart.center_x;
          scoord_y =  (vtx_rot.y *lineart.center_x/2)+lineart.center_y;
       
          vtx_rot=rotate_points(rotate_obj,v2); 
          ecoord_x =  (vtx_rot.x *lineart.center_x/2)+lineart.center_x;
          ecoord_y =  (vtx_rot.y *lineart.center_y/2)+lineart.center_y;
          
          //draw lines in projected screen space
          lineart.draw_line(scoord_x, scoord_y, ecoord_x, ecoord_y, poly_color);
          //throw in some circles for fun
          //lineart.draw_circle(scoord_x, scoord_y, 10, poly_color);
       }
       */

        /***********/
       //RENDER ONLY POINTS  
      /*
       for (j=0;j<OBJ.vertex_count;j++){
            //poly3=OBJ.obj_pts[j]; //store 3 verts in a vector4
            //vprj[0]=rotate_points(rotate_obj,v1);
     
            cout << "Rendering points "<< pntcount << endl; pntcount++;
            //render 3 sided polygon (step through 3 verts and connect the dots) 
        //scoord_x =  (vprj[j].x   *lineart.center_x/2)+lineart.center_x;
      //scoord_y =  (vprj[j].y   *lineart.center_y/2)+lineart.center_y;

        //lineart.draw_circle(scoord_x, scoord_y, 3, vtx_color);
      //lineart.draw_point(scoord_x   , scoord_y  , vtx_color);
      //lineart.draw_point(scoord_x+1 , scoord_y  , vtx_color);
      //lineart.draw_point(scoord_x-1 , scoord_y  , vtx_color);
      //lineart.draw_point(scoord_x   , scoord_y+1 , vtx_color);
      //lineart.draw_point(scoord_x   , scoord_y-1 , vtx_color);
       }
      */
      /***********/
       if (OBJ.is_four_sided)
       {
              
         //RENDER 4 SIDED POLYGONS
         poly=OBJ.faces[i]; //store 4 verts in a vector4
   
         //look up each face vertex (vector3 X4) 
         v1=OBJ.obj_pts[int(poly.x)];
         v2=OBJ.obj_pts[int(poly.y)];
         v3=OBJ.obj_pts[int(poly.z)];
         v4=OBJ.obj_pts[int(poly.w)];
        
         //calculate point rotations
         vprj[0]=rotate_points(rotate_obj,v1);
         vprj[1]=rotate_points(rotate_obj,v2);
         vprj[2]=rotate_points(rotate_obj,v3);   
         vprj[3]=rotate_points(rotate_obj,v4);

         cout << "Rendering 4 sided polygon "<< plycount << endl; plycount++;

         //render four sided polygon (step through 4 verts and connect the dots) 
         for (j=0;j<4;j++){
             double ZVAL = .5 ;

             cout << vprj[j].x << " "<< vprj[j].x << " " << vprj[j].z << endl;

             scoord_x =  (vprj[j].x   *lineart.center_x*ZVAL)+lineart.center_x;
             scoord_y =  (vprj[j].y   *lineart.center_y*ZVAL)+lineart.center_y;

             if (j<3){

                ecoord_x =  (vprj[j+1].x   *lineart.center_x*ZVAL)+lineart.center_x;
                ecoord_y =  (vprj[j+1].y   *lineart.center_y*ZVAL)+lineart.center_y;

                  lineart.draw_line(scoord_x, scoord_y, ecoord_x, ecoord_y, poly_color);
             }
             if (RENDER_PTS){
                 //lineart.draw_circle(scoord_x, scoord_y, 3, vtx_color);
                 lineart.draw_point(scoord_x   , scoord_y  , vtx_color);
                 lineart.draw_point(scoord_x+1 , scoord_y  , vtx_color);
                 lineart.draw_point(scoord_x-1 , scoord_y  , vtx_color);
                 lineart.draw_point(scoord_x   , scoord_y+1 , vtx_color);
                 lineart.draw_point(scoord_x   , scoord_y-1 , vtx_color);
             }

         } 

       }//draw 4 sided polygons  

       /***********/
             
       //RENDER THREE SIDED POLYGONS
       if (OBJ.is_three_sided)
       {
           poly3=OBJ.faces3[i]; //store 3 verts in a vector4
    
           //look up each face vertex (vector3 <vtx1,vtx2,vtx3>) 
           v1=OBJ.obj_pts[int(poly3.x)];
           v2=OBJ.obj_pts[int(poly3.y)];
           v3=OBJ.obj_pts[int(poly3.z)];
     
           //calculate point rotationargv[0], argv[1], argv[2], argv[3],argv[4],argv[5]);s
           vprj[0]=rotate_points(rotate_obj,v1);
           vprj[1]=rotate_points(rotate_obj,v2);
           vprj[2]=rotate_points(rotate_obj,v3);

           // cout << "Rendering 3 sided polygon "<< plycount << endl; plycount++;

           //render 3 sided polygon (step through 3 verts and connect the dots) 
           for (j=0;j<3;j++){
               scoord_x =  (vprj[j].x   *lineart.center_x/2)+lineart.center_x;
               scoord_y =  (vprj[j].y   *lineart.center_y/2)+lineart.center_y;
           
               if (j<2){
                   ecoord_x =  (vprj[j+1].x *lineart.center_x/2)+lineart.center_x;
                   ecoord_y =  (vprj[j+1].y *lineart.center_y/2)+lineart.center_y;
                   lineart.draw_line(scoord_x, scoord_y, ecoord_x, ecoord_y, poly_color);
               }
           
               if (RENDER_PTS){
                   //lineart.draw_circle(scoord_x, scoord_y, 3, vtx_color);
                   lineart.draw_point(scoord_x   , scoord_y   , vtx_color);
                   lineart.draw_point(scoord_x+1 , scoord_y   , vtx_color);
                   lineart.draw_point(scoord_x-1 , scoord_y   , vtx_color);
                   lineart.draw_point(scoord_x   , scoord_y+1 , vtx_color);
                   lineart.draw_point(scoord_x   , scoord_y-1 , vtx_color);
               }
           }

       }//draw 3 sided polygons

   }//render iterator

   framebuffer::savebmp(outfilename , width, height, dpi, output_image);
   
   cout << outfilename << " saved to disk. " << endl;

   cout << "# Done Rendering ! " << endl;
}

