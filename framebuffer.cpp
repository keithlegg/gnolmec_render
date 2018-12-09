
#include <iostream>
#include <fstream>

#include <stdio.h>

#include <cstdlib>
#include <cmath>

#include "include/framebuffer.h"


using namespace std; 

/***********************/

/*
 //RGB color struct
 struct pixelrgb {
     double r;
     double g;
     double b;
 };
*/

   
/*

//        unsigned char bmpfileheader[14] = {'B','M', 0,0,0,0 ,0,0,0,0 , 54,0,0,0};

0h  2   42 4D   "BM"  ID field (42h, 4Dh)
2h  4   46 00 00 00   70 Bytes  Size of the BMP file
6h  2   00 00   Unused  Application specific
8h  2   00 00   Unused  Application specific
Ah  4   36 00 00 00   54 bytes  Offset where the pixel array (bitmap data) can be found  DIB Header

//        unsigned char bmpinfoheader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0,24,0};

Eh  4   28 00 00 00   40 bytes  Number of bytes in the DIB header (from this point)
12h   4   02 00 00 00   2 pixels (left to right order)  Width of the bitmap in pixels
16h   4   02 00 00 00   2 pixels (bottom to top order)  Height of the bitmap in pixels. Positive for bottom to top pixel order. Negative for top to bottom pixel order.
1Ah   2   01 00   1 plane   Number of color planes being used
1Ch   2   18 00   24 bits   Number of bits per pixel
1Eh   4   00 00 00 00   0   BI_RGB, no pixel array compression used
22h   4   10 00 00 00   16 bytes  Size of the raw data in the pixel array (including padding)
26h   4   13 0B 00 00   2,835 pixels/meter  Horizontal resolution of the image
2Ah   4   13 0B 00 00   2,835 pixels/meter  Vertical resolution of the image
2Eh   4   00 00 00 00   0 colors  Number of colors in the palette
32h   4   00 00 00 00   0 important colors  0 means all colors are important Start of pixel array (bitmap data)

//

36h   3   00 00 FF  0 0 255   Red, Pixel (0,1)
39h   3   FF FF FF  255 255 255   White, Pixel (1,1)
3Ch   2   00 00   0 0   Padding for 4 byte alignment (could be a value other than zero)
3Eh   3   FF 00 00  255 0 0   Blue, Pixel (0,0)
41h   3   00 FF 00  0 255 0   Green, Pixel (1,0)
44h   2   00 00   0 0   Padding for 4 byte alignment (could be a value other than zero)

*/

/***********************/


  
void framebuffer::loadbmp (const char *filename, framebuffer::RGBType *data)
{

   //http://www.cplusplus.com/doc/tutorial/files/ 

   // output_image[pix_iterator].b = flat_color;
   //framebuffer::RGBType poly_color; 
   //framebuffer::RGBType vtx_color; 
 
   streampos size;
   char * memblock;

   ifstream file (filename, ios::in|ios::binary|ios::ate);
  
   if (file.is_open())
   {
       size = file.tellg();
       memblock = new char [size];
       file.seekg (0, ios::beg);
       file.read (memblock, size);
       file.close();
   
       cout << "the entire file content is in memory" << endl; 
       delete[] memblock;
   }

   else cout << "Unable to open file";
   //return 0;

}
      


/***********************/

void framebuffer::savebmp (const char *filename, int w, int h, int dpi, framebuffer::RGBType *data) {

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

void framebuffer::draw_point ( int xcoord, int ycoord ){
   pix_iterator = ycoord * bwidth + xcoord;  

   rgbdata[pix_iterator].r = plotcolor.r;       
   rgbdata[pix_iterator].g = plotcolor.g;
   rgbdata[pix_iterator].b = plotcolor.b;
}

/***********************/

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

// degree to radian 
double framebuffer::deg_to_rad ( double deg){
    return deg * DEG_TO_RAD;
}

// radian to degree
double framebuffer::rad_to_deg ( double rad){
    return rad * RAD_TO_DEG;
}

/***********************/ 

/* 
   draw_line:

   args:
       x1 - x coord of line start
       y1 - y coord of line start 
       x2 - x coord of line end 
       y2 - y coord of line end 

   use bresenhams algorithm to dram a line using the default color 

*/

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





