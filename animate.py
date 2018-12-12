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

 
   def render(self, i):
      #./renderthing 512 512 ./3d_obj/monkey.obj 0 0 0 animated/first_1.bmp 100 1
      xrot = 0
      yrot = i*20
      zrot = 0

      imagename = 'blah'
      whichline = 200 
      renderscale = 500 

      com_to_run = (self.com_name+' '
                      +str(self.res_x)+' '+str(self.res_y)
                      +' ./3d_obj/monkey.obj '
                      +str(xrot)+' '+str(yrot)+' '+str(zrot)+' '
                      +self.outpath+'/%s_'%imagename
                      +str(i)+'.bmp'+' '
                      +str(renderscale)+' '+str(whichline)
                    ) 

      #print( com_to_run ) 
      os.system( com_to_run ) 
 
   def animate(self):
      #./renderthing 400 400 3d_obj/triangle.obj 0 0 0 foo.bmp 320 220
      for i in range(1,10,2):
          self.render(i)  

      #construct an animated GIF
      #os.system('convert   -delay 20   -loop 0   '+self.outpath+'first*.bmp   '+self.outpath+'animatespheres.gif')
 


anim_obj = animate_renderthing()

#anim_obj.render(1)

anim_obj.animate()


