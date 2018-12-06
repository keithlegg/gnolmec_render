
#include <cstdlib>
#include <cmath>

#include "include/framebuffer.h"


void framebuffer::draw_point ( int xcoord, int ycoord ){
   pix_iterator = ycoord * bwidth + xcoord;     
   rgbdata[pix_iterator].r = plotcolor.r;       
   rgbdata[pix_iterator].g = plotcolor.g;
   rgbdata[pix_iterator].b = plotcolor.b;
}

void framebuffer::draw_point ( int xcoord, int ycoord, RGBType pcol ){
   pix_iterator = xcoord * bwidth + ycoord;     
   rgbdata[pix_iterator].r = pcol.r;       
   rgbdata[pix_iterator].g = pcol.g;
   rgbdata[pix_iterator].b = pcol.b;
}

/***********************/

void framebuffer::draw_circle ( int x_orig, int y_orig, int dia){
   int plot_x = 0;
   int plot_y = 0;

   for (int i = 0; i <=360; i++)
   {  
       plot_x = x_orig + (sin(deg_to_rad(i))*dia) ;
       plot_y = y_orig + (cos(deg_to_rad(i))*dia) ;
       draw_point(plot_x, plot_y); 
   }
}

void framebuffer::draw_circle ( int x_orig, int y_orig, int dia, RGBType pcol){
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

double framebuffer::deg_to_rad ( double deg){
    return deg * DEG_TO_RAD;
}

double framebuffer::rad_to_deg ( double rad){
    return rad * RAD_TO_DEG;
}

/***********************/ 
void framebuffer::draw_line(int x1, int y1, int const x2, int const y2)
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
void framebuffer::draw_line(int x1, int y1, int const x2, int const y2, RGBType pcol)
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





