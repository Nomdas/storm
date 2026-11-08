#include  <stdio.h>
#include <stdbool.h>
#include  <string.h>
#include  <stdlib.h>
#include "storm.h"
#include <X11/Xlib.h>

int main(void)
{

  Display *dis=XOpenDisplay(NULL);
  int scn=DefaultScreen(dis);
  Window root=XRootWindow(dis,scn);
  Window win=XCreateSimpleWindow(dis,root,0,0,480,360,0,0xFF0000,0x0000FF);
  int run=1;
  strm_cntxt *ctx;
 
  
  XEvent evt;
  Atom wm_delete_win=XInternAtom(dis , "WM_DELETE_WINDOW", False);


  XSetWMProtocols(dis,win,&wm_delete_win, 1);

  XSelectInput(dis,win, ExposureMask | KeyPressMask|StructureNotifyMask);
  XMapWindow(dis,win);

  while(run)
    {
      while(XPending(dis)>0)
	{
	  XNextEvent(dis,&evt);
	  

	  
	  if (evt.xclient.type == ClientMessage)
	    {
	      if (evt.xclient.data.l[0] ==wm_delete_win)
		{
		  run=0;
		}

	    }
	}
    }

  return 0;
}
