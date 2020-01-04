#ifndef NGCMODEL_H    
#define NGCMODEL_H



#include "model.h"


#define MAX_NUM_ARCS 10000
#define MAX_NUM_FACES 10000


void removeCharsFromString( string &str, char* charsToRemove );


class arc_rad
{
    public:

        arc_rad(){

        };

        ~arc_rad(){};
 
    double r;
    double x;
    double y;
    double z;           

};



class ngc_model: public model 
{
    public:
        //Matrix4 m44;

        ngc_model(){
            arc_count = 0;
        };

        ~ngc_model(){};

        int arc_count;
        
        arc_rad arctmp;

        std::vector<arc_rad> arcs;        
        std::vector<double> linesegs[MAX_NUM_FACES];    


        // inherited containers 
        // Vector3 obj_pts[MAX_NUM_VERTICES];          // vertices of model    
        // std::vector<int> lines[MAX_NUM_FACES];      // 2 sided faces 
        // std::vector<int> triangles[MAX_NUM_FACES];  // 3 sided 
        // std::vector<int> quads[MAX_NUM_FACES];      // 4 sided 
        // std::vector<int> faces[MAX_NUM_FACES];      // >4, N sided faces 
        
        // Vector3 bfr_pts[MAX_NUM_VERTICES];          // general point buffer   ( tmp work area )
        // std::vector<int> bfr_faces[MAX_NUM_FACES];  // general polygon buffer ( tmp work area ) 


        void save_ngc( char* filename );
        void load_ngc( char* filename );


};





#endif
