#ifndef framebuffer_H
#define _framebuffer_H

#include <cstdlib>
#include <cmath>



/*
class RGBType {
	   double r;
	   double g;
	   double b;
};
*/


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
	   double r;
	   double g;
	   double b;
   };


   RGBType *rgbdata; //pointer to our pixel array
   RGBType plotcolor;//color for the pretty lines and dots

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

   /***********************/
   //method functions
   /***********************/

   void draw_point ( int xcoord, int ycoord ){
     pix_iterator = ycoord * bwidth + xcoord;     
     rgbdata[pix_iterator].r = plotcolor.r;       
     rgbdata[pix_iterator].g = plotcolor.g;
     rgbdata[pix_iterator].b = plotcolor.b;

   }

   void draw_point ( int xcoord, int ycoord, RGBType pcol ){
       pix_iterator = xcoord * bwidth + ycoord;     
       rgbdata[pix_iterator].r = pcol.r;       
       rgbdata[pix_iterator].g = pcol.g;
       rgbdata[pix_iterator].b = pcol.b;
   }

   /***********************/

   void draw_circle ( int x_orig, int y_orig, int dia){
     int plot_x = 0;
     int plot_y = 0;

     for (int i = 0; i <=360; i++)
     {  
           plot_x = x_orig + (sin(deg_to_rad(i))*dia) ;
           plot_y = y_orig + (cos(deg_to_rad(i))*dia) ;
           draw_point(plot_x, plot_y); 
     }
   }

   void draw_circle ( int x_orig, int y_orig, int dia, RGBType pcol){
     int plot_x = 0;
     int plot_y = 0;

     for (int i = 0; i <=360; i++)
     {  
           plot_x = x_orig + (sin(deg_to_rad(i))*dia) ;
           plot_y = y_orig + (cos(deg_to_rad(i))*dia) ;
           draw_point(plot_x, plot_y, pcol); 
     }
   }

   /***********************/

   double deg_to_rad ( double deg){
      return deg * DEG_TO_RAD;
   }

   double rad_to_deg ( double rad){
      return rad * RAD_TO_DEG;
   }

   /***********************/ 
   void draw_line(int x1, int y1, int const x2, int const y2)
   {
	    int delta_x(x2 - x1);
	    // if x1 == x2, then it does not matter what we set here
	    signed char const ix((delta_x > 0) - (delta_x < 0));
	    delta_x = std::abs(delta_x) << 1;
            int delta_y(y2 - y1);
	    // if y1 == y2, then it does not matter what we set here
	    signed char const iy((delta_y > 0) - (delta_y < 0));
	    delta_y = std::abs(delta_y) << 1;
	    draw_point( x1, y1 );
	    if (delta_x >= delta_y)
	    {
		// error may go below zero
		int error(delta_y - (delta_x >> 1));
		while (x1 != x2)
		{
		    if ((error >= 0) && (error || (ix > 0)))
		    {
		        error -= delta_x;
		        y1 += iy;
		    }
		    // else do nothing
		    error += delta_y;
		    x1 += ix;
		    draw_point(x1, y1);
		}
	    }
	    else
	    {
		// error may go below zero
		int error(delta_x - (delta_y >> 1));
		while (y1 != y2)
		{
		    if ((error >= 0) && (error || (iy > 0)))
		    {
		        error -= delta_y;
		        x1 += ix;
		    }
		    // else do nothing
		    error += delta_x;
		    y1 += iy;
		    draw_point(x1, y1 );
		}
	    }
   }
   /***********************/ 
   void draw_line(int x1, int y1, int const x2, int const y2, RGBType pcol)
   {
	    int delta_x(x2 - x1);
	    // if x1 == x2, then it does not matter what we set here
	    signed char const ix((delta_x > 0) - (delta_x < 0));
	    delta_x = std::abs(delta_x) << 1;
            int delta_y(y2 - y1);
	    // if y1 == y2, then it does not matter what we set here
	    signed char const iy((delta_y > 0) - (delta_y < 0));
	    delta_y = std::abs(delta_y) << 1;
	    draw_point( x1, y1, pcol );
	    if (delta_x >= delta_y)
	    {
		// error may go below zero
		int error(delta_y - (delta_x >> 1));
		while (x1 != x2)
		{
		    if ((error >= 0) && (error || (ix > 0)))
		    {
		        error -= delta_x;
		        y1 += iy;
		    }
		    // else do nothing
		    error += delta_y;
		    x1 += ix;
		    draw_point(x1, y1, pcol);
		}
	    }
	    else
	    {
		// error may go below zero
		int error(delta_x - (delta_y >> 1));
		while (y1 != y2)
		{
		    if ((error >= 0) && (error || (iy > 0)))
		    {
		        error -= delta_y;
		        x1 += ix;
		    }
		    // else do nothing
		    error += delta_x;
		    y1 += iy;
		    draw_point(x1, y1, pcol);
		}
	    }
   }

};



#endif
