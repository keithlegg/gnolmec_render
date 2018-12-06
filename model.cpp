
#include <iostream>
#include <stdio.h>

#include <fstream>
#include <cstring>
#include <vector>
#include <stdlib.h> 

//#include "math.h"
#include "include/Vectors.h"
#include "include/model.h"

using namespace std;


const int MAX_CHARS_PER_LINE = 512;
const int MAX_TOKENS_PER_LINE = 20;
const char* const DELIMITER = " ";

// tokens to parse out of .OBJ file
const char* v_chr = "v";  // vertex identifier in OBJ file 
const char* f_chr = "f";  // face identifier in OBJ file 


// int vtx_cnt = 1;     //num coordinates




int model::getnum_verts(){
   return sizeof( obj_pts ) / sizeof( obj_pts[0] );
}

int model::getnum_edges(){
   return sizeof( edges ) / sizeof( edges[0] );
}

int model::getnum_faces(){
   return sizeof( faces ) / sizeof( faces[0] );
}

int model::getnum_faces3(){
   return sizeof( faces3 ) / sizeof( faces3[0] );
}

/**********************/
void model::showinfo()
{
   cout << " is three sided " << is_three_sided << endl;
   cout << " is four sided " << is_four_sided << endl;
   cout << " loded vertex data  "<< model::vertex_count << endl;
   cout << " loded face data  "<< face_count << endl;
   //cout <<" size of 3 sided poly buffer " << getnum_faces3()  << endl;
   // cout <<" size of 4 sided poly buffer " << getnum_faces()  << endl;
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
       cout << faces[xx] << endl;
     }

  }

  int numf3 = getnum_faces3();
  for (int xx=0;xx<numf3;xx++){
     if (xx<dnum){
       cout << faces3[xx] << endl;
     }

  }


}

/**********************/
void model::save_obj( char* filename){
   ofstream myfile;
   myfile.open (filename);

   myfile << "#Exported with Keith's little graphics tool\n";
   myfile << "#number of verticies "<< model::vertex_count  <<"\n";
   myfile << "#number of faces     "<< face_count <<"\n";
   myfile <<"\n";

   for (int xx=0;xx<model::vertex_count;xx++){
       myfile << "v " << obj_pts[xx][0] <<" "<< obj_pts[xx][1] <<" "<< obj_pts[xx][2] <<"\n";
   }

   myfile <<"\n";

   for (int xx=0;xx<face_count;xx++){
       if ( is_three_sided )
       { 
          myfile << "f " << faces3[xx][0] <<" "<< faces3[xx][1] <<" "<< faces3[xx][2] <<"\n";
       }
       if ( is_four_sided )
       { 
          myfile << "f " << faces[xx][0] <<" "<< faces[xx][1] <<" "<< faces[xx][2]<<" "<< faces[xx][3] <<"\n";
       }
   }
        
   myfile.close();
}

/**********************/
void model::load_obj(char* filename){
    ifstream fin;
    fin.open(filename); // open a file
    if (!fin.good()){ 
        cout << "NO FILE! "<< filename << endl;
        exit (EXIT_FAILURE); // exit if file not found
    }

    while (!fin.eof())
    {
        char buf[MAX_CHARS_PER_LINE];
        fin.getline(buf, MAX_CHARS_PER_LINE);
        calcfac=0;

            int n = 0; 
            const char* token[MAX_TOKENS_PER_LINE] = {};
            token[0] = strtok(buf, DELIMITER);
            if (token[0]) 
            {
                for (n = 1; n < MAX_TOKENS_PER_LINE; n++)
                {
                    token[n] = strtok(0, DELIMITER);

                    //calculate if 3 or 4 sided faces 
                    if (!strcmp(token[0],f_chr))
                    {
                        calcfac++;
                    }////

                    if (!token[n]) break;  
            }
          
            //cout << calcfac << endl;
            if(calcfac==5){is_four_sided = true;}
            if(calcfac==4){is_three_sided = true ;}
    }
    /******************************/

    // process the tokens
    for (int i = 1; i < n; i++){
        //vertex tokens 
        if (!strcmp(token[0],v_chr))
        {
            if (vtx_cnt<=2){
                vtx_tmp.push_back( atof(token[i]) );
            }
            if (vtx_cnt==3){
                vtx_tmp.push_back( atof(token[i]) );
                vtx_cnt=0;
                //cout << vtx_tmp[0]<<" "<<vtx_tmp[1]<<" "<<vtx_tmp[2] << endl;
                obj_pts[model::vertex_count].set(vtx_tmp[0],vtx_tmp[1],vtx_tmp[2]);

                vtx_tmp.clear();
                model::vertex_count++;
            }
            vtx_cnt++;

       }

       //face tokens
       if (!strcmp(token[0],f_chr))
       {
           //is_three_sided


           //4 sided
           if (is_four_sided)
           {
               if (fac_cnt<=3){
                   fac_tmp.push_back( atof(token[i]) );
               }  

               if (fac_cnt==4){
                   fac_tmp.push_back( atof(token[i]) );
                   fac_cnt=0;
                   //cout << fac_tmp[0]<<" "<<fac_tmp[1]<<" "<<fac_tmp[2]<<" " <<fac_tmp[3]<< endl;

                   faces[face_count].set(fac_tmp[0], fac_tmp[1], fac_tmp[2],fac_tmp[3]);

                   fac_tmp.clear();
                   face_count++;
           }
               }

               if (is_three_sided){
                   //3 SIDED
                   if (fac_cnt<=2){
                       fac_tmp.push_back( atof(token[i]) );
                   }
                   
                   if (fac_cnt==3){
                       fac_tmp.push_back( atof(token[i]) );
                       fac_cnt=0;
                         
                       //0 indexed  (may NOT be zero indexed?)
                       faces3[face_count].set(fac_tmp[0]-1, fac_tmp[1]-1, fac_tmp[2]-1);

                       fac_tmp.clear();
                       face_count++;
                   }
               }
               /*******/
               fac_cnt++;
           }
       }//iterate tokens
   }//iterate each line

}//load object

/**********************/
void model::make_cube(double scale){
    //vertecies
    obj_pts[0].set(-scale,-scale,  scale);
    obj_pts[1].set(-scale,-scale, -scale);
    obj_pts[2].set( scale,-scale, -scale);
    obj_pts[3].set( scale,-scale,  scale);
    obj_pts[4].set(-scale, scale,  scale);
    obj_pts[5].set(-scale, scale, -scale);
    obj_pts[6].set( scale, scale, -scale);
    obj_pts[7].set( scale, scale,  scale);
    
    //edges
    edges[0].set(0,1);
    edges[1].set(1,2);
    edges[2].set(2,3);

    int vtx_cnt; edges[3].set(3,0);
    edges[4].set(4,5);
    edges[5].set(5,6);
    edges[6].set(6,7);
    edges[7].set(7,4);
    edges[8].set(0,4);
    edges[9].set(1,5);
    edges[10].set(3,7);
    edges[11].set(2,6);

    //faces 
    faces[0].set(0,1,2,3);
    faces[1].set(0,1,5,4);
    faces[2].set(1,2,6,5);
    faces[3].set(2,6,7,3);
    faces[4].set(3,7,4,0);
    faces[5].set(4,5,6,7);

    is_four_sided = true;
    face_count = 6;
    model::vertex_count = 8;
}

/**********************/
 void model::make_circle(int divs, double scale){
    //vertecies - (3d vectors)
    obj_pts[0].set(-scale, 1,  scale);
    obj_pts[1].set( scale, 1,  scale);
    obj_pts[2].set( scale, 1, -scale);
    obj_pts[3].set(-scale, 1, -scale);

    //edges     - (indexes to verts)
    edges[0].set(0,1); 
    edges[1].set(1,2);
    edges[2].set(2,3);
    edges[3].set(3,0);

    //face
    faces[0].set(0,1,2,3);

    //is_four_sided = true;
    //face_count   =6;
    //model::vertex_count =8;
 }     

/**********************/
 void model::make_square(double scale){
    //vertecies - (3d vectors)
    obj_pts[0].set(-scale, 1,  scale);
    obj_pts[1].set( scale, 1,  scale);
    obj_pts[2].set( scale, 1, -scale);
    obj_pts[3].set(-scale, 1, -scale);

    //edges     - (indexes to verts)
    edges[0].set(0,1); 
    edges[1].set(1,2);
    edges[2].set(2,3);
    edges[3].set(3,0);

    //face
    faces[0].set(0,1,2,3);

    is_four_sided = true;
    face_count   =1;
    model::vertex_count =4;
 }    




