#ifndef NGCMODEL_H    
#define NGCMODEL_H



#include "model.h"


#define MAX_NUM_ARCS 10000
#define MAX_NUM_FACES 10000


void removeCharsFromString( string &str, char* charsToRemove );
vector<string> split(const char *str, char c);


class param
{
    public:
        param(){

        };

        ~param(){};

       string name;
       string value;         
};


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


class headmove
{
    public: 
        headmove(){
        };

        ~headmove(){};

    int type = 0; //0 - G0 rapid, 1 G1 - Linear , 2/3 - G2/G3 Arc I J K or R, P

    double r;    
    double x;
    double y;
    double z;     

    // allows you to omit some axes 
    bool do_x;
    bool do_y;
    bool do_z; 

};


class ngc_model: public model 
{
    public:
        int linear_unit =    0; //0 inch , 1 mm 
        int spindle_dir =    0; //0 CW   , 1 CCW 
        int spindle_rpm = 3000; 

        ngc_model(){
            arc_count = 0;
            origin.set(0,0,0);
        };

        ~ngc_model(){};

        Vector3 origin;

        int arc_count;
        int move_count;
        
        //store in case it is not explicitly specified on each line 
        int last_move_type; // 0, 1, 2/3 

        arc_rad arctmp;
        headmove headtmp;

        std::vector<arc_rad> arcs;        
        std::vector<headmove> moves; 


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

        void convert_to_3d( void );


};





#endif
