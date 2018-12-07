#ifndef framebuffer_H
#define _framebuffer_H

class framebuffer {

   int pix_iterator;

   public:
     int bwidth;
     int bheight;
     int center_x; //center of screen
     int center_y; //center of screen
     int n;        //number of pixels
     double DEG_TO_RAD;
     double RAD_TO_DEG;

   
   //RGB color struct
   struct RGBType {
       short r;
       short g;
       short b;
   };

   /*
   //RGBA color struct
   struct RGBAType {
       short r;
       short g;
       short b;
       short a;
   };*/


   RGBType *rgbdata;  //pointer to our pixel array
   RGBType plotcolor; //color for the pretty lines and dots

   framebuffer( int w, int h) {
       bwidth = w;
       bheight = h;
       n = bwidth * bheight;
       rgbdata = new RGBType[n];

       // degree = radian * (180 / PI) // PI = 3.14159265
       // radian = degree * (PI/180) 
       DEG_TO_RAD = 0.0174532925;
       RAD_TO_DEG = 57.29577951;

       center_x = w/2;
       center_y = h/2;

       plotcolor.r = 1;
       plotcolor.g = 1;
       plotcolor.b = 1;
   }

   ~framebuffer() {
       delete rgbdata;  
   }

   // void loadbmp (const char *filename, int w, int h, int dpi, framebuffer::RGBType *data);
   static void savebmp (const char *filename, int w, int h, int dpi, framebuffer::RGBType *data);

   void draw_point ( int xcoord, int ycoord );
   void draw_point ( int xcoord, int ycoord, RGBType pcol );
   void draw_circle ( int x_orig, int y_orig, int dia);
   void draw_circle ( int x_orig, int y_orig, int dia, RGBType pcol);

   double deg_to_rad ( double deg);
   double rad_to_deg ( double rad);

   void draw_line(int x1, int y1, int const x2, int const y2);
   void draw_line(int x1, int y1, int const x2, int const y2, RGBType pcol);

};


#endif
