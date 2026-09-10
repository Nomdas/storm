#include "storm.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <bits/getopt_core.h>
#include <string.h>

strm_cntxt *global_ctx;

void strm_begin(strm_cntxt *ctx)
{
  //  global_ctx = NULL;

  strm_log("Started Context Successfully",STRM_SUCCESS);
  strm_window_init(ctx);

  ctx->window.x11.dis = XOpenDisplay(NULL);
  
  if (ctx->window.x11.dis != NULL)
    {
  strm_log("Connect To X11 Display Server",STRM_ENTERACT);
  ctx->window.x11.scn=DefaultScreen(ctx->window.x11.dis);
  ctx->window.x11.root = XRootWindow(ctx->window.x11.dis,
                                     ctx->window.x11.scn);

  
  ctx->state=1;
  ctx->init = 1;
     strm_log("Window Initalized Successfully",STRM_SUCCESS);
    }
}

void strm_end(strm_cntxt *ctx)
{
  if(ctx->window.x11.dis !=NULL){;
     XDestroyWindow(ctx->window.x11.dis,ctx->window.x11.win);
    
     if( XCloseDisplay(ctx->window.x11.dis)==0)
       {
       strm_log("Window Closed Successfully",STRM_SUCCESS);
       }
     else 
       {
	 strm_log("Fail To Close Window",STRM_ERROR);
       }
         ctx->state = 0;
   
   strm_log("Stop Context Successfully",STRM_SUCCESS);
  }
  else
    {
  strm_log("Fail To Stop Context",STRM_ERROR);
    }
}

void strm_window_init(strm_cntxt *ctx)
{
  ctx->window.x11.dis=malloc(sizeof(Display*));
  ctx->window.x11.root= 0;
  ctx->window.x11.win = 0;
  ctx->window.x11.scn = 0;

}


int strm_win_state (strm_cntxt *ctx)
{
  return ctx->state;
}


void  strm_draw_window(char *title,int w, int h,int x, int y
    ,int bw,strm_color brc, strm_color bgc , strm_cntxt *ctx)
{


  if (ctx->window.x11.dis == NULL) strm_log("Failed To Initialize.!(Check strm_begin())",STRM_ERROR);
    
  
  if (ctx->init == 1)
      {

    ctx->window.x11.win = XCreateSimpleWindow(ctx->window.x11.dis,
                         ctx->window.x11.root,x,y,w,h,bw,brc,bgc);

    XStoreName(ctx->window.x11.dis, ctx->window.x11.win, title);

    ctx->window.x11.wm.wm_delete_win =
    XInternAtom(ctx->window.x11.dis , "WM_DELETE_WINDOW", False);


    XSetWMProtocols(ctx->window.x11.dis,
		    ctx->window.x11.win,&ctx->window.x11.wm.wm_delete_win, 1);

    XSelectInput(ctx->window.x11.dis, ctx->window.x11.win,
		 ExposureMask | KeyPressMask|StructureNotifyMask);


    ctx->window.x11.img =
        XCreateImage (ctx->window.x11.dis,
                     DefaultVisual(ctx->window.x11.dis, ctx->window.x11.scn),
                     STRM_MAX_24_BIT, ZPixmap, STRM_ZERO,
                     ctx->window.x11.pixels,w,h,32,STRM_ZERO);
    
    XMapWindow(ctx->window.x11.dis, ctx->window.x11.win);

    ctx->init=0;
      }
 
  if (ctx->window.x11.evt.xclient.type == ClientMessage)
    {
    if (ctx->window.x11.evt.xclient.data.l[0] == ctx->window.x11.wm.wm_delete_win) {
      ctx->state =0;
    }
        }


    
  
 
  XNextEvent(ctx->window.x11.dis,&ctx->window.x11.evt);

}




void strm_draw_rect(int x, int y, unsigned int w, unsigned int h,strm_color clr,
                    strm_cntxt *ctx) {

    if (ctx->window.x11.evt.type == Expose)
      {
      XSetForeground(ctx->window.x11.dis,
                     DefaultGC(ctx->window.x11.dis
			      ,ctx->window.x11.scn), clr);

       
       XDrawRectangle(ctx->window.x11.dis, ctx->window.x11.win,
                      DefaultGC(ctx->window.x11.dis,
                      ctx->window.x11.scn), x,y,w,h);

       XFlush(ctx->window.x11.dis);
  
    }  
}

strm_color strm_rand_color()
{
  srand(time(NULL));
  return (rand()% (STRM_MAX_24_BIT-0+1)+0);
}



strm_txt_color strm_log_color(strm_state state)
{

    
  if (state == STRM_SUCCESS)  return STRM_GREEN;
  if (state == STRM_WARNING)  return STRM_YELLOW;
  if (state == STRM_ERROR)    return STRM_RED;
  if (state == STRM_ENTERACT) return STRM_BLUE;

  return STRM_NORMAL;
}

strm_txt_color strm_reset_color()
{ 
  return STRM_NORMAL;  
}
void strm_set_deflt_cntxt(strm_cntxt *ctx)
{
  if (ctx != NULL) global_ctx = ctx;
    
  if (ctx == NULL)
    strm_log("Unvalid Default Context",STRM_ERROR);
}


void strm_log(strm_str log,strm_state state)
{

 
  char *txt_color=strm_log_color(state);

  if (strm_term_support_clr())
    {
        printf("%sSTORM:%s%s\n",txt_color,strm_reset_color(),log);
  }
  else
  {
    printf("STORM:%s\n",log);  
  }
 }


bool strm_term_support_clr()
{
  if (isatty(STDOUT_FILENO))
  {
    char *env = getenv("TERM");

    if (strcmp( env,"dumb")==0) return false;
    return true;
  }

  return false;
}
