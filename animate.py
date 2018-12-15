 #!/usr/local/bin/python

import os
import sys


class animate_renderthing:
    """ simple python to call render_thing in a loop and animate the output """

    def __init__(self):
        self.com_name = './renderthing'
        self.outpath = 'animated'
        self.res_x = 512
        self.res_y = 512

    ################
    def render(self, object3d, i):
        #./renderthing 512 512 ./3d_obj/monkey.obj 0 0 0 animated/first_1.bmp 100 1
        xrot = 0
        yrot = i
        zrot = i

        renderscale = 100 
      
        imagename = 'blah'

        com_to_run = (self.com_name+' '
                      +str(self.res_x)+' '+str(self.res_y)
                      +' '+ str(object3d)+' '  
                      +str(xrot)+' '+str(yrot)+' '+str(zrot)+' '
                      +self.outpath+'/%s_'%imagename
                      +str(i)+'.bmp'+' '
                      +str(renderscale)+' '+str(i)
                     ) 

      
        #print( com_to_run ) 
        os.system( com_to_run ) 
 
    ################
    def animate(self, object3d):
        for i in range(1, self.res_x, 2):
            print("rendering frame %s"%(i) )
            self.render(object3d, i)  

    

    #construct an animated GIF
    #os.system('convert   -delay 20   -loop 0   '+self.outpath+'first*.bmp   '+self.outpath+'animatespheres.gif')
 



##########################################


anim_obj = animate_renderthing()
#anim_obj.render(1)


anim_obj.animate('./3d_obj/cone.obj')  # scale to 100
#anim_obj.animate('./3d_obj/monkey.obj')  # scale to 100

#anim_obj.animate('./3d_obj/triangle.obj')  # scale to 700
#anim_obj.animate('./3d_obj/sphere.obj')  # scale to 500



