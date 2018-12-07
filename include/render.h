
#ifndef RENDER_H
#define _RENDER_H

#include "Vectors.h"
#include "Matrices.h"


/*
class render {
    public:

        // constructor  
        render(){};
        
        // destructors  
        ~render(){};

        // public methods 
        Vector4 rotate_points ( Matrix4 m, Vector4 v);
        Vector4 rotate_points ( Matrix4 m, Vector3 v);
        void render_model( int width, int height, char* objfilename ,float RX, float RY, float RZ ,char* outfilename);
};
*/


void make_image(int width, int height);

// intersect_2d (self, v1s, v1e, v2s, v2e ):
void intersect_2d (void);

Vector4 rotate_points ( Matrix4 m, Vector4 v);
Vector4 rotate_points ( Matrix4 m, Vector3 v);

void render_model( int width, int height, char* objfilename ,float RX, float RY, float RZ ,char* outfilename);
void really_simple_render_model( int width, int height, char* objfilename ,float RX, float RY, float RZ ,char* outfilename);        


#endif

