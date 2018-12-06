 #!/usr/local/bin/python

import os
import sys


class render:

   def __init__(self):
      self.outpath = '/keith/tazebro/'
      self.res_x = 512
      self.res_y = 512
      
   
   #DEBUG NEED TO PAD FRAMES?
   def render(self):
      #./renderthing 640 480 "/keith/bigjilm.obj" 0 0 0 "/keith/mok.bmp"
      os.system('/keith/brorender '+str(self.res_x)+' '+str(self.res_y)+' /keith/renderthing/other/obj/monkey2.obj 90 '+str(i*10)+' '+str(i*5)+' '+self.outpath+'/first_'+str(i)+'.bmp')

 
   def animate(self):
      #./renderthing 640 480 "/keith/bigjilm.obj" 0 0 0 "/keith/mok.bmp"
      for i in range(1,2,1):
         os.system('/keith/brorender '+str(self.res_x)+' '+str(self.res_y)+' /keith/renderthing/other/obj/monkey2.obj 90 '+str(i*10)+' '+str(i*5)+' '+self.outpath+'/first_'+str(i)+'.bmp')
      
      #construct an animated GIF
      #os.system('convert   -delay 20   -loop 0   '+self.outpath+'first*.bmp   '+self.outpath+'animatespheres.gif')
 


TAZEME = render()
TAZEME.animate()


