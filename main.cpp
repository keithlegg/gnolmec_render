#include <iostream>
#include <stdio.h>

#include "framebuffer.h"
#include "Vectors.h"
#include "Matrix4.cpp"
#include "model.h"

#include "node.h"


using namespace std;

int pix_iterator;//index to a pixel on the framebuffer

/***********************/


 //RGB color struct
 struct pixelrgb {
	   double r;
	   double g;
	   double b;
 };
   
   
/*

//        unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0 ,0,0,0,0 , 54,0,0,0};

0h 	2 	42 4D 	"BM" 	ID field (42h, 4Dh)
2h 	4 	46 00 00 00 	70 Bytes 	Size of the BMP file
6h 	2 	00 00 	Unused 	Application specific
8h 	2 	00 00 	Unused 	Application specific
Ah 	4 	36 00 00 00 	54 bytes 	Offset where the pixel array (bitmap data) can be found  DIB Header

//        unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

Eh 	4 	28 00 00 00 	40 bytes 	Number of bytes in the DIB header (from this point)
12h 	4 	02 00 00 00 	2 pixels (left to right order) 	Width of the bitmap in pixels
16h 	4 	02 00 00 00 	2 pixels (bottom to top order) 	Height of the bitmap in pixels. Positive for bottom to top pixel order. Negative for top to bottom pixel order.
1Ah 	2 	01 00 	1 plane 	Number of color planes being used
1Ch 	2 	18 00 	24 bits 	Number of bits per pixel
1Eh 	4 	00 00 00 00 	0 	BI_RGB, no pixel array compression used
22h 	4 	10 00 00 00 	16 bytes 	Size of the raw data in the pixel array (including padding)
26h 	4 	13 0B 00 00 	2,835 pixels/meter 	Horizontal resolution of the image
2Ah 	4 	13 0B 00 00 	2,835 pixels/meter 	Vertical resolution of the image
2Eh 	4 	00 00 00 00 	0 colors 	Number of colors in the palette
32h 	4 	00 00 00 00 	0 important colors 	0 means all colors are important Start of pixel array (bitmap data)

//

36h 	3 	00 00 FF 	0 0 255 	Red, Pixel (0,1)
39h 	3 	FF FF FF 	255 255 255 	White, Pixel (1,1)
3Ch 	2 	00 00 	0 0 	Padding for 4 byte alignment (could be a value other than zero)
3Eh 	3 	FF 00 00 	255 0 0 	Blue, Pixel (0,0)
41h 	3 	00 FF 00 	0 255 0 	Green, Pixel (1,0)
44h 	2 	00 00 	0 0 	Padding for 4 byte alignment (could be a value other than zero)

*/


  
void loadbmp (const char *filename, int w, int h, int dpi, framebuffer::RGBType *data) {


}
      


/***********************/

void savebmp (const char *filename, int w, int h, int dpi, framebuffer::RGBType *data) {

    FILE *f;
    int k = w*h;
    int s = 4*k;
    int filesize = 54 +s;


    double factor = 39.375;
    int m = static_cast<int>(factor);

    int ppm = dpi*m;
    unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0 ,0,0,0,0 , 54,0,0,0};
    unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

    bmpfileheader[2] = (unsigned char) (filesize);
    bmpfileheader[3] = (unsigned char) (filesize>>8);
    bmpfileheader[4] = (unsigned char) (filesize>>16);
    bmpfileheader[5] = (unsigned char) (filesize>>24);

    bmpinfoheader[4] = (unsigned char) (w);
    bmpinfoheader[5] = (unsigned char) (w>>8);
    bmpinfoheader[6] = (unsigned char) (w>>16);
    bmpinfoheader[7] = (unsigned char) (w>>24);

    bmpinfoheader[8]  = (unsigned char) (h);
    bmpinfoheader[9]  = (unsigned char) (h>>8);
    bmpinfoheader[10] = (unsigned char) (h>>16);
    bmpinfoheader[11] = (unsigned char) (h>>24);

    bmpinfoheader[21] = (unsigned char) (s);
    bmpinfoheader[22] = (unsigned char) (s>>8);
    bmpinfoheader[23] = (unsigned char) (s>>16);
    bmpinfoheader[24] = (unsigned char) (s>>24);

    bmpinfoheader[25] = (unsigned char) (ppm);
    bmpinfoheader[26] = (unsigned char) (ppm>>8);
    bmpinfoheader[27] = (unsigned char) (ppm>>16);
    bmpinfoheader[28] = (unsigned char) (ppm>>24);

    bmpinfoheader[29] = (unsigned char) (ppm);
    bmpinfoheader[30] = (unsigned char) (ppm>>8);
    bmpinfoheader[31] = (unsigned char) (ppm>>16);
    bmpinfoheader[32] = (unsigned char) (ppm>>24);

    f = fopen( filename,"wb");
    fwrite( bmpfileheader, 1, 14, f);
    fwrite( bmpinfoheader, 1, 40, f);

    for (int i = 0; i < k;i++){
       framebuffer::RGBType rgb = data[i];

       double red   = (data[i].r)*255;
       double green = (data[i].g)*255;
       double blue  = (data[i].b)*255;

       unsigned char color[3] = { (int)floor(blue),(int)floor(green),(int)floor(red) };
       fwrite (color, 1,3,f);
    }
    fclose(f);

}

/***********************/

Vector4 rotate_points ( Matrix4 m, Vector4 v) {

    Vector4 out;
     out.x = m[0] * v.x + m[4] * v.y + m[8]  * v.z + m[12] * v.w;
     out.y = m[1] * v.x + m[5] * v.y + m[9]  * v.z + m[13] * v.w;
     out.z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w;
     out.w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w;

    return out;
}

Vector4 rotate_points ( Matrix4 m, Vector3 v) {

    Vector4 out;
     out.x = m[0] * v.x + m[4] * v.y + m[8]  * v.z + m[12] * 1;
     out.y = m[1] * v.x + m[5] * v.y + m[9]  * v.z + m[13] * 1;
     out.z = m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * 1;
     out.w = m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * 1;

    return out;
}


/*
void setup_frustum (float project_scale, float SX, float SY)
// * project_scale is the projection scaling factor used in the perspective
//   projection. It's the value you multiply x and y with before you divide
//   them by z. (usually I use 256 for this value).
//
//  * SX and SY are the size of the viewport you want to draw at (320,200 anyone?)
{
  float angle_horizontal =  atan2(SX/2,project_scale)-0.0001;
  float angle_vertical   =  atan2(SY/2,project_scale)-0.0001;
  float sh               =  sin(angle_horizontal);
  float sv               =  sin(angle_vertical);
  float ch               =  cos(angle_horizontal);
  float cv               =  cos(angle_vertical);
  // left
  sides[0].normal.x=ch;
  sides[0].normal.y=0;
  sides[0].normal.z=sh;
  sides[0].distance = 0;
  // right
  sides[1].normal.x=-ch;
  sides[1].normal.y=0;
  sides[1].normal.z=sh;
  sides[1].distance = 0;
  // top
  sides[2].normal.x=0;
  sides[2].normal.y=cv;
  sides[2].normal.z=sv;
  sides[2].distance = 0;
  // bottom
  sides[3].normal.x=0;
  sides[3].normal.y=-cv;
  sides[3].normal.z=sv;
  sides[3].distance = 0;
  // z-near clipping plane
  znear.normal.x=0;
  znear.normal.y=0;
  znear.normal.z=1;
  znear.distance = -10;
}

*/

/*


  //http://www.cubic.org/docs/3dclip.htm


	struct tvector
	{
	  float x,y,z;             // standard vector
	};

	struct plane
	{
	  tvector  normal;         // normalized Normal-Vector of the plane
	  float    distance;       // shortest distance from plane to Origin
	};

	struct frustum
	{
	  plane sides[4];          // represent the 4 sides of the frustum
	  plane znear;             // the z-near plane
	}

/*
Just as an example: If you want to construct a plane from 3 points do it this way:

plane p;                      // plane to construct from a,b and c
tvector a,b,c;                // points to build a plane from
{
   // build normal vector
   tvector q,v;
   q.x = b.x - a.x;    v.x = b.x - c.x;
   q.y = b.y - a.y;    v.x = b.y - c.y;
   q.z = b.y - a.y;    v.x = b.z - c.z;
   p.normal = crossproduct (q,v);
   normalize_vector (q.normal);

   // calculate distance to origin
   p.distance = dotproduct (p.normal, a);  // you could also use b or c
}

*/

/*
//http://www.scratchapixel.com/lessons/3d-advanced-lessons/perspective-and-orthographic-projection-matrix/perspective-projection-matrix/

template<typename T> 
void setperspectivepmat(const T &near, const T &far, const T &fov, Matrix4<T> &mat) { 

   T scale = T(1) / tan(degtorad(fov * 0.5)); 
    mat[0][0] = mat[1][1] = scale; 
    mat[2][2] = - far / (far - near); 
    mat[3][2] = - far * near / (far - near); 
    mat[2][3] = - 1; mat[3][3] = 0; 
   }



template<typename T> void projectverts(const Matrix4<T> &mat, const Point3<T> *verts, const unsigned &nverts) { 
   // create an image unsigned width = 640; 
   unsigned char *buffer = new unsigned char [width * width]; 
   memset(buffer, 0x0, width * width); for (int i = 0; i < nverts; ++i) { Point3<T> ps = verts[i] * mat; if (ps.x < -1 || ps.x > 1 || ps.y < -1 || ps.y > 1) continue; // convert projected point coordinates to pixel coordinates unsigned px = std::min(unsigned((ps.x + 1) * 0.5 * width), width - 1); unsigned py = std::min(unsigned((1-(ps.y + 1) * 0.5) * width), width - 1); buffer[py * width + px] = 255; } // save to ppm std::ofstream ofs;eee ofs.open("./untitled.ppm"); ofs << "P5\n" << width << " " << width << "\n255\n"; ofs.write((char*)buffer, width * width); ofs.close(); delete [] buffer; }



Point3 operator * (const Matrix4<T> &mat) const { Point3<T> pt( x * mat[0][0] + y * mat[1][0] + z * mat[2][0] + mat[3][0], x * mat[0][1] + y * mat[1][1] + z * mat[2][1] + mat[3][1], x * mat[0][2] + y * mat[1][2] + z * mat[2][2] + mat[3][2]); T w = x * mat[0][3] + y * mat[1][3] + z * mat[2][3] + mat[3][3]; return (w != 1) ? pt / w : pt; }

*/


void render_model( int width, int height, char* filename ,float RX, float RY, float RZ ,char* outfilename){


/***********************/


   /***********************/




   cout << "# Begin Rendering ..." << endl;

   int dpi    = 72; 
   //int width  = 512; 
   //int height = 512; 
   int res_x = width;
   int res_y = height;

   int n = width * height;

   framebuffer::RGBType* pixels;
   framebuffer lineart( width, height );
   pixels = lineart.rgbdata;

   /***********/

   //fill each pixel with a color (negative color?)
   for (int x = 0; x < width; x++)
   {     //rotate_obj.show();
       for (int y = 0; y < height; y++)
       {  
            pix_iterator = y * width + x;     

            pixels[pix_iterator].r = y/2;       
            pixels[pix_iterator].g = 255;//-y/3;
            pixels[pix_iterator].b = x*(y/2);

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
   OBJ.load_obj(filename);

   //OBJ.load_obj("other/obj/sphere.obj");



   /***********/
   Vector2 thisedge;      //iterator for edges
   Vector4 poly;          //store vertex id's in a vector4 (4 sided poly) 
   Vector3 poly3;         //store vertex id's in a vector3 (3 sided poly) 
   Vector4 vprj[4];       //3 and 4 sided -  projected point coordinates
   Vector3 v1,v2,v3,v4;   //retrieved point data (xyz)
   Matrix4 rotate_obj;    //4X4 rotation matrix 


   //Matrix4 camera;       //4X4 camera matrix



   int i =0;
   int j =0;
   int plycount = 0;
   int pntcountsave_obj = 0;

   int RENDER_PTS = 1;
   double scoord_x, scoord_y, ecoord_x, ecoord_y;

   
   
   rotate_obj.show();
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

	       cout << "Rendering polygon "<< plycount << endl; plycount++;

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
       }//4 sided   

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

	       cout << "Rendering polygon "<< plycount << endl; plycount++;

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
       }


   }//render iterator

   savebmp(outfilename , width, height, dpi, pixels);
 
   cout << "# Done Rendering ! " << endl;
}


int main(int argc, char *argv[])
{

  /*
   if(argc < 2) {
       cout<< "  USAGE\n";
       cout<< "  \n";
       cout << "  int width, int height, char* filename ,float RX, float RY, float RZ ,char* outfilename\n";
       exit(0);
   }
   */
   
   //Node M;
  /*
   Node N;
   N.setNodeName("frank");
   N.set_xyz( 11, 12, 13);
   Node X(N);
   X.set_xyz( 11, 12, 13);
   X.setNodeName("bob");

   //cout << X.get_x();
 
   //cout << N.GetNodeName() ;

   Node M = N.GetParentNode();
   cout << M.get_x();
 
   //cout<< N.GetParentNode();

  */


    //model MOBJ;
    //MOBJ.make_cube(1);
    //MOBJ.save_obj("mycube.obj");
    
   
  
    //( int width, int height, char* filename ,float RX, float RY, float RZ ,char* outfilename){
    //render_model(atoi(argv[1]), atoi(argv[2]), argv[3], atof(argv[4]), atof(argv[5]), atof(argv[6]) ,  argv[7]);


     //render_model(256,256,"mycube.obj",0,0,0,"foo.bmp");
     render_model(256,256,"mycube.obj", 0, 90 ,0,"foo.bmp");



    //READ BMP DATA  
    //int w, h, i, j;
    //pixelrgb* img = read_bmp("foo.bmp", &w, &h);
    

   return 0;
}




