#include <iostream>
using namespace std; 

#include <stdio.h>

#include <fstream>
#include <cstring>
#include <vector>
#include <stdlib.h> 
#include <algorithm>


#include "math.h"
#include "include/Vectors.h"
#include "include/model.h"

#include "include/point_ops.h"

using namespace std;


const int MAX_CHARS_PER_LINE = 512;
const int MAX_TOKENS_PER_LINE = 20;
const char* const DELIMITER = " ";

// tokens to parse out of .OBJ file
const char* v_chr = "v";  // vertex identifier in OBJ file 
const char* f_chr = "f";  // face identifier in OBJ file 



/******************************/

// FIND A BETTER PLACE FOR THIS 
// FRAMEBUFFER HAS A COPY AND NEEDS TO BE MOVED 

#define DEG_TO_RAD 0.0174532925
#define RAD_TO_DEG 57.29577951

double deg_to_rad ( double deg){
   return deg * DEG_TO_RAD;
}

double rad_to_deg ( double rad){
   return rad * RAD_TO_DEG;
}

/******************************/



int model::getnum_verts(){
   return sizeof( obj_pts ) / sizeof( obj_pts[0] );
}

int model::getnum_faces(){
   return sizeof( faces ) / sizeof( faces[0] );
}


/**********************/
void model::showinfo()
{
   cout << " loded vertex data  "<< model::vertex_count << endl;
   cout << " loded face data  "<< face_count << endl;
}


/**********************/
void model::show()
{
  
    int dnum = 100;

    int numv = getnum_verts();
    for (int xx=0;xx<numv;xx++){
        if (xx<dnum){
            cout << obj_pts[xx] << endl;
        }
    }


    int numf = getnum_faces();
    for (int xx=0;xx<numf;xx++){
        if (xx<dnum){
            // cout << faces[xx] << endl;
        }

    }

}


/**********************/


bool sort_by_zdist(const zindex_faces &lzif, const zindex_faces &rzif){ 

    // if (lzif.distance == rzif.distance){
    //     cout << "same! " << lzif.distance;
    //     return true;
    // }
    return lzif.distance > rzif.distance; 
}

void model::sort_faces_dist(Vector3 campos)
{
    
    // if (face_count==0){
    //     cout << " error - no faces to export ";
    // }


    //sortfaces.clear()

    // obj_pts[vcnt].set( sin(deg_to_rad(a))*scale, cos(deg_to_rad(a))*scale, 0 ); 

    int i,j = 0;

    vector <zindex_faces> sortfaces(MAX_NUM_FACES);

    // walk the faces and do stuff 
    for (int i=0; i<face_count; i++) 
    {
        fac_tmp.clear();
        fac_tmp = model::faces[i];
        
        // assume triangle only (3 indices)
        Vector3 p1 = model::obj_pts[ fac_tmp[0]-1 ];
        Vector3 p2 = model::obj_pts[ fac_tmp[1]-1 ];
        Vector3 p3 = model::obj_pts[ fac_tmp[2]-1 ];                 

        //Vector3 pnt_dist;
        //triangle_centroid(pnt_dist, p1, p2, p3);

        //use a struct (zipped array-ish) to sort by distance
        zindex_faces tmp;
        tmp.face     = fac_tmp;
        tmp.distance = model::triangle_mean_z(p1,p2,p3);
        
        //cout << " distance "<< tmp.distance << "\n";

        sortfaces[i] = tmp;

    }


    sort(sortfaces.begin(), sortfaces.end(), sort_by_zdist);

    // for (zindex_faces &n : sortfaces)
    //     cout << n.distance << " \n ";

    // overwrite sorted faces with sorted faces 
    for (int i=0; i<face_count; i++) 
    {
        model::faces[i] = sortfaces[i].face;
        //cout << "sortfaces "<< i <<" " << sortfaces[i].face[0]<< " " << sortfaces[i].face[1] << " "<< sortfaces[i].face[2] << "\n";
    }


    cout << "face count is " << face_count << " size of sorted is " << j <<"\n";

    //for debugging
    model::save_obj("sorted.obj");
}


/**********************/
void model::save_obj( char* filename)
{
    ofstream myfile;
    myfile.open (filename);

    myfile << "#Exported with Keith's little graphics tool\n";
    myfile << "#number of verticies "<< model::vertex_count  <<"\n";
    myfile << "#number of faces     "<< face_count <<"\n";
    myfile <<"\n";

    //DEBUG why is vertex_count an attribute ?? - cant we just get the size of the array - DEBUG!
    for (int xx=0;xx<model::vertex_count;xx++){
        myfile << "v " << obj_pts[xx][0] <<" "<< obj_pts[xx][1] <<" "<< obj_pts[xx][2] <<"\n";
    }

    myfile <<"\n";
    
    if (face_count==0){
        cout << " error - no faces to export ";
    }

    // cout << " face count is  " << face_count << endl;

    int ff = 0; 
    int xx = 0;

    // export array of N sided faces
    for (xx=0; xx<face_count; xx++)
    {
        myfile << "f ";
        for (ff=0; ff < faces[xx].size();ff++)
        {
            myfile << faces[xx][ff] << " "; 
        }
        myfile << "\n";
    }
        
    myfile.close();
    cout << endl << "obj file " << filename << " exported." << endl;

}

/**********************/
void model::load_obj(char* filename){
    ifstream fin;
    fin.open(filename); // open a file
    if (!fin.good()){ 
        cout << "OBJ file \""<< filename <<"\" appears to be missing or broken." << endl;
        exit (EXIT_FAILURE); // exit if file not found
    }

    vtx_tmp.clear();
    fac_tmp.clear();


    while (!fin.eof())
    {
        char buf[MAX_CHARS_PER_LINE];
        fin.getline(buf, MAX_CHARS_PER_LINE);
        
        int i = 0;
        int n = 0; 
        fac_tmp.clear();

        const char* token[MAX_TOKENS_PER_LINE] = {};
        token[0] = strtok(buf, DELIMITER);

        //if line has data on it ...  
        if (token[0]) 
        {
            // walk the space delineated tokens 
            for (n=1; n < MAX_TOKENS_PER_LINE; n++)
            {
                token[n] = strtok(0, DELIMITER);
                if (!token[n]) break;  
            }

        }

        /******************************/

        // process the tokens
        for (i=1; i<n; i++)
        {

            // vertex tokens "v"
            if (!strcmp(token[0],v_chr))
            {
                // first 2 of 3 vertices
                if (vtx_cnt<=2){
                    vtx_tmp.push_back( atof(token[i]) );
                }
                // third vertex 
                if (vtx_cnt==3){
                    vtx_tmp.push_back( atof(token[i]) );
                    vtx_cnt=0;
                   
                    //load 3D point 
                    obj_pts[model::vertex_count].set(vtx_tmp[0],vtx_tmp[1],vtx_tmp[2]);
                    
                    vtx_tmp.clear();
                    model::vertex_count++;
                }
                vtx_cnt++;
            }
  
            /**********/
            // face tokens "f"
            if (!strcmp(token[0],f_chr))
            {
               
                // cout << " i is "<< i << " |  num_tok  " << n << endl; 

                // if end of line is hit - store loaded data and reset 
                if (i < n-1 ) 
                {
                    //keep adding fids  
                    fac_tmp.push_back( atof(token[i]) );fac_cnt++;
                }
                if (i == n-1 ) 
                {
                    //add one more fid before we stop to get lest 
                    fac_tmp.push_back( atof(token[i]) );fac_cnt++;

                    faces[face_count] = fac_tmp; 
                    fac_cnt = 1;     // fid count on line
                    fac_tmp.clear(); // tmp face buffer 
                    face_count++;    // total face count in object 
                }
            }
           
        }//iterate tokens
       
  
    }//iterate each line
}//load object


/**********************/
/*
   load a 4X4 matrix from disk to project geometry in render
*/
void model::load_matrix(char* filename)
{
    
    ifstream fin;
    fin.open(filename); // open a file
    if (!fin.good()){ 
        cout << "matrix file \""<< filename <<"\" appears to be missing or broken." << endl;
        exit (EXIT_FAILURE); // exit if file not found
    }

    int line_ct = 0;
    while (!fin.eof())
    {
        char buf[MAX_CHARS_PER_LINE];
        fin.getline(buf, MAX_CHARS_PER_LINE);
        
        int i = 0;
        int n = 0; 

        const char* token[MAX_TOKENS_PER_LINE] = {};
        token[0] = strtok(buf, DELIMITER);

        //if line has data on it ...  
        if (token[0]) 
        {
            // walk the space delineated tokens 
            for (n=1; n < MAX_TOKENS_PER_LINE; n++)
            {
                token[n] = strtok(0, DELIMITER);
                if (!token[n]) break;  
            }

        }
        
        if (line_ct==0){
            m44[0]=atof(token[0]); m44[1]=atof(token[1]); m44[2]=atof(token[2]); m44[3]=atof(token[3]);
        }
        if (line_ct==1){
            m44[4]=atof(token[0]); m44[5]=atof(token[1]); m44[6]=atof(token[2]); m44[7]=atof(token[3]);
        }
        if (line_ct==2){
            m44[8]=atof(token[0]); m44[9]=atof(token[1]); m44[10]=atof(token[2]); m44[11]=atof(token[3]);
        }
        if (line_ct==3){
            m44[12]=atof(token[0]); m44[13]=atof(token[1]); m44[14]=atof(token[2]); m44[15]=atof(token[3]);
        }

        line_ct ++; 
    }

    //m44.transpose();
}


/**********************/
void model::make_cube(double scale){

    // vertices
    obj_pts[0].set(-scale, -scale,  scale);
    obj_pts[1].set(-scale, -scale, -scale);
    obj_pts[2].set( scale, -scale, -scale);
    obj_pts[3].set( scale, -scale,  scale);
    obj_pts[4].set(-scale,  scale,  scale);
    obj_pts[5].set(-scale,  scale, -scale);
    obj_pts[6].set( scale,  scale, -scale);
    obj_pts[7].set( scale,  scale,  scale);
       
    //fac_tmp = {1,2,3,4};
    //faces[0] = fac_tmp;

    // faces - NOT zero indexed
    // faces[0] = {1,2,3,4};
    // faces[1] = {1,2,6,5};
    // faces[2] = {2,3,7,6};
    // faces[3] = {3,7,8,4};
    // faces[4] = {4,8,5,1};
    // faces[5] = {5,6,7,8};

    /********************/    
    faces[0] = {1,2,3,4};
    faces[1] = {1,2,6,5};
    faces[2] = {2,3,7,6};
    faces[3] = {3,7,8,4};
    faces[4] = {4,8,5,1};
    faces[5] = {5,6,7,8};

    face_count = 6;  
    model::vertex_count = 8;
}

/**********************/
 void model::make_circle(int divs, double scale)
 {

    double a = 0;
    int vcnt = 0;
    int step = 360/divs;
    
    cout << "step is " << step << endl ;

    fac_tmp.clear();

    for (a=0;a<360;a=a+step)
    {
        //cout << "a is " << a << endl ;
        
        //x axis 
        //obj_pts[vcnt].set(0,  sin(deg_to_rad(a))*scale, cos(deg_to_rad(a))*scale );

        //y axis 
        //obj_pts[vcnt].set( sin(deg_to_rad(a))*scale, 0, cos(deg_to_rad(a))*scale ); 

        //z axis 
        obj_pts[vcnt].set( sin(deg_to_rad(a))*scale, cos(deg_to_rad(a))*scale, 0 ); 
        vcnt++;
    } 

    // cout << faceindices[0] << faceindices[5];

    int i = 0; 
 
    for (int i=0; i<vcnt; i++) 
    {
        //fac_tmp.clear();
        fac_tmp.push_back(i+1);  
    }


    faces[0] = fac_tmp;
    face_count = 1;

    model::vertex_count = vcnt;
 }     

/**********************/
 void model::make_square(double scale)
 {
    fac_tmp.clear();

    // vertices - (3d vectors)
    
    // obj_pts[vcnt].set( sin(deg_to_rad(a))*scale, cos(deg_to_rad(a))*scale, 0 ); 

    obj_pts[0].set(-scale, 1,  scale);
    obj_pts[1].set( scale, 1,  scale);
    obj_pts[2].set( scale, 1, -scale);
    obj_pts[3].set(-scale, 1, -scale);

    // face indices are NOT zero indexed 
    faces[0] = {1,2,3,4};

    face_count = 1;
    model::vertex_count =4;
 }    


/**********************/
 void model::make_triangle(double scale)
 {
    fac_tmp.clear();

    // vertices - (3d vectors)
    
    // obj_pts[vcnt].set( sin(deg_to_rad(a))*scale, cos(deg_to_rad(a))*scale, 0 ); 


    // //X axis 
    // obj_pts[0].set( 0, -scale ,  0     );
    // obj_pts[1].set( 0, 0      ,  scale );
    // obj_pts[2].set( 0, scale  ,  0     );

    // Y axis 
    // obj_pts[0].set( -scale ,0 ,  0     );
    // obj_pts[1].set( 0      ,0 ,  scale );
    // obj_pts[2].set( scale  ,0 ,  0     );

    // // Z axis 
    obj_pts[0].set( -scale ,  0    , 0 );
    obj_pts[1].set( 0      ,  scale, 0 );
    obj_pts[2].set( scale  ,  0    , 0 );

    faces[0] = {1,2,3};

    face_count = 1;
    model::vertex_count = 3;
 }    



/**********************/
 void model::make_line(double scale)
 {
    // vertices - (3d vectors)
    obj_pts[0].set(-scale, 0,  -scale);
    obj_pts[1].set( scale, 0,  scale);

    faces[0] = {1,2}; //notice this is only a 2 point poly (line)

    face_count = 1;

    model::vertex_count =4;
 }     



