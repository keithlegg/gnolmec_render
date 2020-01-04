
#include <iostream>
#include <fstream>
#include <algorithm>

#include <cstring>

using namespace std; 

#include "include/ngc_model.h"



const int MAX_CHARS_PER_LINE = 512;
const int MAX_TOKENS_PER_LINE = 20;
const char* const DELIMITER = " ";


const char* lnum       = "n";    
const char* arc_chr    = "r";  

//// tokens to parse out of .OBJ file
//const char* v_chr    = "v";   // vertex identifier in OBJ file 

//const char* uv_chr   = "vt";  // UV identifier in OBJ file 
//const char* nrml_chr = "vn";  // UV identifier in OBJ file 
//const char* line_chr = "l";   // Line identifier in OBJ file 


/**********************************************************/
/*

    L generic parameter word for G10, M66 and others
    Set length units (G20, G21). 
    Set path control mode (G61, G61.1, G64) 

*/


// g20                       - units inch 
// g64                       - path blending (constant speed?)
// s3400 m3                  - 3400 RPM , clockwise 
// g0z1                      - rapid move Z axis 1
// g0x0y0z1                  - rapid move XYZ  0 0 1   
// g0 x1.724638 y-1.012731
// g1z-.1f24
// g1 x1.724638 y-1.012731
// g2 r1.997999 x1.613302 y-1.178668


  // G0		                         Rapid Move
  // G1		                         Linear Move
  // G2,G3	       I J K or R, P     Arc Move

/**********************************************************/

void removeCharsFromString( string &str, char* charsToRemove ) {
   for ( unsigned int i = 0; i < strlen(charsToRemove); ++i ) {
      str.erase( remove(str.begin(), str.end(), charsToRemove[i]), str.end() );
   }
}

/**********************************************************/

void ngc_model::load_ngc(char* filename)
{
    ifstream fin;
    fin.open(filename); // open a file
    if (!fin.good()){ 
        cout << "NGC file \""<< filename <<"\" appears to be missing or broken." << endl;
        exit (EXIT_FAILURE); // exit if file not found
    }

    // vtx_tmp.clear();
    // fac_tmp.clear();
    //arc_rad

    int line_ct = 0;
    int tidx, i,n = 0;

    string sr, sx, sy;

    while (!fin.eof())
    {
        char buf[MAX_CHARS_PER_LINE];
        fin.getline(buf, MAX_CHARS_PER_LINE);
        

        tidx = 0;i = 0;n = 0;

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
        // cout << "## token 0 " << token[0] << " " << " \n";
        // // process the tokens ( to last depth )
        // for (i=1; i<n; i++)
        // {
        //     cout << "## token "<< i <<" "<< token[i] << " \n";
        //     // vertex tokens "v"
        //     if (!strcmp(token[0], lnum))
        //     {
        //         // first 2 of 3 vertices
        //         if (vtx_cnt<=2){
        //             //vtx_tmp.push_back( atof(token[i]) );
        //         }
        //         // third vertex 
        //         if (vtx_cnt==3){
        //         }
        //         vtx_cnt++;
        //     }
        // }//iterate tokens

        /******************************/        
        //cout << "## token 0 " << token[0] << " " << " \n";

        // process the tokens ( to last depth )
        for (i=1; i<n; i++)
        {
            //cout << "## token "<< i <<" "<< token[i] << " \n";
             
            // // numbered lines 
            // if (!strcmp(token[0], lnum))
            // {
            // }

            //------------
          
            //if arc radius 
            sr = token[0];
			if (sr.find('r') != string::npos)
            {

                removeCharsFromString(sr, "r");
			    //cout << "arc radius is " << sr << "\n";

            	if (tidx==0)
            	{
                    sx = token[i];            		
                    removeCharsFromString(sx, "x");            		
                    //cout << " X " << sx << "\n"; 
                }

            	if (tidx==1)
            	{
                    sy = token[i];            		
                    removeCharsFromString(sy, "y");            		

                    //cout << "arc radius " << sr << " " << sx << " " << sy << "\n";
            		// now we have what we need to build an arc 
            		arctmp.r = stof(sr);
            		arctmp.x = stof(sx);
            		arctmp.y = stof(sy);

            		arcs.push_back( arctmp );
                    arc_count++;
            	}


            }//arc radius 
			
            //------------

            //------------

            tidx++;
        }//iterate tokens

        
        //------------------------------ 
        line_ct ++;

    }//iterate each line
}//load object



/**********************************************************/



void ngc_model::convert_to_3d( void )
{

	cout << "convert to 3d " << arc_count << "\n";
    
    double false_z = 0.0;

    int a = 0; 
    
    std::vector<int> linetmp;

    for (a=0;a<arc_count;a++)
    {
        linetmp.clear();
        
    	//cout << "cvt 3d " << a << " "<< arcs[a].r << "\n";
        
        obj_pts[a]=( Vector3(arcs[a].x, arcs[a].y, false_z ) ); vertex_count++;
        
        //fake line geom for testing 
        if (a>0)
        {
        	linetmp.push_back( a-1);
        	linetmp.push_back( a );
        	lines[line_count] = linetmp;
        	line_count++;      	
        }
    } 



}



/**********************************************************/


//save geometry in memory to an OBJ file on disk 
void ngc_model::save_ngc( char* filename)
{
    ofstream myfile;
    myfile.open (filename);

    myfile << "#Exported with Keith's little graphics tool\n";
    myfile << "#number of verticies "<< model::vertex_count  <<"\n";
    myfile << "#number of triangles     "<< triangle_count <<"\n";
    myfile <<"\n";

    // for (int xx=0;xx<model::vertex_count;xx++){
    //     myfile << "v " << obj_pts[xx][0] <<" "<< obj_pts[xx][1] <<" "<< obj_pts[xx][2] <<"\n";
    // }

    myfile <<"\n";
    
    if (triangle_count==0){
        cout << " error - no faces to export ";
    }

    // cout << " triangle count is  " << triangle_count << endl;

    int ff = 0; 
    int xx = 0;

    //----------------------

    /*
    // export array of N sided faces
    if (nsided_count>0)
    {
        for (xx=0; xx<nsided_count; xx++)
        {
            myfile << "f ";
            for (ff=0; ff < faces[xx].size();ff++)
            {
                myfile << faces[xx][ff] << " "; 
            }
            myfile << "\n";
        }
    }*/
    
    //----------------------
 
    // // export array triangles
    // if(triangle_count>0)
    // {
    //     for (xx=0; xx<triangle_count; xx++)
    //     {
    //         myfile << "f ";
    //         for (ff=0; ff < triangles[xx].size();ff++)
    //         {
    //             myfile << triangles[xx][ff] << " "; 
    //         }
    //         myfile << "\n";
    //     }
    // }

    //----------------------
    
    // // export array quads
    // if(quad_count>0)
    // {
    //     for (xx=0; xx<quad_count; xx++)
    //     {
    //         myfile << "f ";
    //         for (ff=0; ff < quads[xx].size();ff++)
    //         {
    //             myfile << quads[xx][ff] << " "; 
    //         }
    //         myfile << "\n";
    //     }
    // }

    //----------------------

    myfile.close();
    cout << endl << "obj file " << filename << " exported." << endl;

}

















/**********************************************************/














/**********************************************************/

/*

  // http://linuxcnc.org/docs/html/gcode.html

  G0		                         Rapid Move
  G1		                         Linear Move
  G2,G3	       I J K or R, P         Arc Move
  G4	       P	                 Dwell
  G5	       I J  P Q	             Cubic Spline
  G5.1	       I J	                 Quadratic Spline
  G5.2	       P L	                 NURBS
  G38.2-G38.5		                 Straight Probe
  G33	       K                     Spindle Synchronized Motion
  G33.1	       K	                 Rigid Tapping
  G80 		                         Cancel Canned Cycle

## Canned cycles	(X Y Z or U V W apply to canned cycles, depending on active plane)
  G81	       R L (P)	             Drilling Cycle
  G82	       R L (P)	             Drilling Cycle, Dwell
  G83	       R L Q	             Drilling Cycle, Peck
  G73	       R L Q	             Drilling Cycle, Chip Breaking
  G85	       R L (P)	             Boring Cycle, Feed Out
  G89	       R L (P)	             Boring Cycle, Dwell, Feed Out
  G76	       P Z I J R K Q H L E	 Threading Cycle

## Distance Mode
  G90, G91		                     Distance Mode
  G90.1, G91.1		                 Arc Distance Mode
  G7		                         Lathe Diameter Mode
  G8		                         Lathe Radius Mode

##Feed Rate Mode
  G93, G94, G95	                     Feed Rate Mode

##Spindle Control
  M3, M4, M5	     S	             Spindle Control
  M19		                         Orient Spindle
  G96, G97	     S D                 Spindle Control Mode

##Coolant
  M7, M8, M9                        Coolant Control

##Tool Length Offset
  G43 	         H               	 Tool Length Offset
  G43.1 		                     Dynamic Tool Length Offset
  G43.2 	     H                   Apply additional Tool Length Offset
  G49 		                         Cancel Tool Length Compensation

##Stopping
  M0, M1	                         Program Pause
  M2, M30	                         Program End
  M60		                         Pallet Change Pause

##Units
  G20                               Units inch
  G21	  	                        Units mm

##Plane Selection 	(affects G2, G3, G81…G89, G40…G42)
  G17 - G19.1		                 Plane Select

##Cutter Radius Compensation
  G40		                         Compensation Off
  G41,G42	           D	         Cutter Compensation
  G41.1, G42.1	       D L	         Dynamic Cutter Compensation

##Path Control Mode
  G61 G61.1		                     Exact Path Mode
  G64	               P Q	         Path Blending

##Return Mode in Canned Cycles
  G98, G99		                     Canned Cycle Return Level

##Other Modal Codes
  F		                             Set Feed Rate
  S		                             Set Spindle Speed
  T		                             Select Tool)
  M48, M49		                     Speed and Feed Override Control
  M50	P0 (off) or P1 (on)	         Feed Override Control
  M51	P0 (off) or P1 (on)	         Spindle Speed Override Control
  M52	P0 (off) or P1 (on)	         Adaptive Feed Control
  M53	P0 (off) or P1 (on)	         Feed Stop Control
  G54-G59.3		                     Select Coordinate System

##Flow-control Codes
  o_sub	                          	 Subroutines, sub/endsub call
  o_while		                     Looping, while/endwhile do/while
  o_if		                         Conditional, if/else/endif
  o_repeat		                     Repeat a loop of code
  []		                         Indirection
  o_call		                     Call named file
  M70		                         Save modal state
  M71		                         Invalidate stored state
  M72		                         Restore modal state
  M73		                         Save and Auto-restore modal state

##Input/Output Codes
  M62 - M65	P	                     Digital Output Control
  M66	P E L Q	                     Wait on Input
  M67	T	                         Analog Output,Synchronized
  M68	T	                         Analog Output, Immediate

##Non-modal Codes
  M6	           T	             Tool Change
  M61	           Q	             Set Current Tool
  G10 L1	       P Q R	         Set Tool Table
  G10 L10	       P	             Set Tool Table
  G10 L11	       P	             Set Tool Table
  G10 L2	       P R	             Set Coordinate System
  G10 L20	       P	             Set Coordinate System
  G28, G28.1		                 Go/Set Predefined Position
  G30, G30.1		                 Go/Set Predefined Position
  G53		                         Move in Machine Coordinates
  G92		                         Coordinate System Offset
  G92.1, G92.2		                 Reset G92 Offsets
  G92.3		                         Restore G92 Offsets
  M101 - M199	P Q	                 User Defined Commands

##Comments & Messages
  ; (…)		                         Comments
  (MSG,…)		                     Messages
  (DEBUG,…)		                     Debug Messages
  (PRINT,…)		                     Print Messages

//--------------------------------------------------


    M3 - start the spindle clockwise at the S speed.
    M4 - start the spindle counterclockwise at the S speed.
    M5 - stop the spindle.



*/

/*
	A  A axis of machine
	B  B axis of machine
	C  C axis of machine
	D  Tool radius compensation number
	F  Feed rate
	G  General function (See table Modal Groups)
	H  Tool length offset index
	I  X offset for arcs and G87 canned cycles
	J  Y offset for arcs and G87 canned cycles
	K  Z offset for arcs and G87 canned cycles.
	   Spindle-Motion Ratio for G33 synchronized movements.
	L  generic parameter word for G10, M66 and others
	M  Miscellaneous function (See table Modal Groups)
	N  Line number
	P  Dwell time in canned cycles and with G4.
	   Key used with G10.
	Q  Feed increment in G73, G83 canned cycles
	R  Arc radius or canned cycle plane
	S  Spindle speed
	T  Tool selection
	U  U axis of machine
	V  V axis of machine
	W  W axis of machine
	X  X axis of machine
	Y  Y axis of machine
	Z  Z axis of machine

*/

/**********************************************************/


