#include "storm.h"


 strm_cntxt *global_ctx;

strm_cntxt* strm_init_cntxt()
{
  return (malloc(sizeof(strm_cntxt)));  
}

void strm_free_cntxt(strm_cntxt *ctx)
{
  free(ctx);
}
void strm_begin(strm_cntxt *ctx)
{

  ctx->window.x11.pixels=0;
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
      ctx->setting.delta_time=0.0;
      
       strm_log("Window Initalized Successfully",STRM_SUCCESS);
    }
}

void strm_end(strm_cntxt *ctx)
{
  if(ctx->window.x11.dis !=NULL)
    {
      if(ctx->window.x11.img != NULL)
	{
	  
	  XDestroyImage(ctx->window.x11.img);
	  ctx->window.x11.pixels =NULL;
	  ctx->window.x11.img = NULL;


	}
      XDestroyWindow(ctx->window.x11.dis,ctx->window.x11.win);
      XCloseDisplay(ctx->window.x11.dis);
     
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
  ctx->window.x11.dis=NULL;
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

  
  
  if (ctx->window.x11.dis == NULL)
    {
      strm_log("Failed To Initialize.!(Check strm_begin())",STRM_ERROR);
    }
    
  
  if (ctx->init == 1)
    {

      ctx->window.x11.pixels=(byte*)(malloc(4*w*h));

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
		      DefaultDepth(ctx->window.x11.dis, ctx->window.x11.scn), ZPixmap, 0,
		      ctx->window.x11.pixels,w,h,32,0);
    
      XMapWindow(ctx->window.x11.dis, ctx->window.x11.win);

      ctx->window.x11.w=w;
      ctx->window.x11.h=h;

      ctx->init=0;
      ctx->setting.last_time=clock();
    }
  else
    {
       ctx->setting.last_time=ctx->setting.current_time;
    }

  
  

      ctx->setting.current_time=clock();

      if(ctx->setting.fps == 0 ) ctx->setting.fps = 30;
      
      ctx->setting.delta_time=(double)(ctx->setting.current_time - ctx->setting.last_time)/(ctx->setting.fps * 1000); /* its in millisecond */  

      if(ctx->setting.delta_time > 1.0/ctx->setting.fps) 
	{
	ctx->setting.delta_time=0.00;
	}

      
    
  

  
  while(XPending(ctx->window.x11.dis)> 0)    
    {


      XNextEvent(ctx->window.x11.dis, &ctx->window.x11.evt);
    	
      if (ctx->window.x11.evt.xclient.type == ClientMessage)
	{
	  if (ctx->window.x11.evt.xclient.data.l[0] == ctx->window.x11.wm.wm_delete_win)
	    {
		ctx->state =0;
	    }
	}

      else if(ctx->window.x11.evt.type == Expose)
	{
	  // XPutImage(ctx->window.x11.dis, ctx->window.x11.win,DefaultGC(ctx->window.x11.dis, ctx->window.x11.win),  ctx->window.x11.img,0,0,0,0,ctx->window.x11.w,ctx->window.x11.h);
	}
    }
 
}




void strm_draw_rect(int x, int y, unsigned int w, unsigned int h,strm_color clr,
                    strm_cntxt *ctx) {
  
  
     
	

      
  
	

      
      XSetForeground(ctx->window.x11.dis,
		     DefaultGC(ctx->window.x11.dis
			       ,ctx->window.x11.scn),clr);
    
    XDrawRectangle(ctx->window.x11.dis, ctx->window.x11.win,
    DefaultGC(ctx->window.x11.dis,
    ctx->window.x11.scn),x,y,w,h);

    XFlush(ctx->window.x11.dis);
  



  
}

int strm_get_width(strm_cntxt *ctx)
{
  XWindowAttributes cord;
  XGetWindowAttributes(ctx->window.x11.dis,ctx->window.x11.win,&cord);
  return cord.width;
}

int strm_get_height(strm_cntxt *ctx)
{
  XWindowAttributes cord;
  XGetWindowAttributes(ctx->window.x11.dis,ctx->window.x11.win,&cord);
  return cord.height;
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
  if (ctx != NULL)
    {
      global_ctx = NULL;
      global_ctx = ctx;
    }

  if (ctx == NULL)
    { 
      strm_log("Unvalid Default Context",STRM_ERROR);
    } 
}

void strm_log(strm_str log,strm_state state)
{

  if (
      global_ctx->setting.log==STRM_LOG_NONE ||
      global_ctx->setting.log==STRM_LOG_ON
      )
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
