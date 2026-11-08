#include "storm.h"


int main()
{

  strm_cntxt *ctx=strm_init_cntxt();
  
  ctx->setting.log=STRM_LOG_ON;
  ctx->setting.fps=60;
  
  strm_set_deflt_cntxt(ctx);
  
  strm_begin(ctx);
   
  int width=480;
  int height=360;
  
    while (ctx->state == 1)
      {
	strm_color cl1=0xFFFFFF; /* pure white */ 
    
	strm_draw_window("Storm Window",width,height,0,0,1,cl1,cl1,ctx);	 
	for(int i=0;i<strm_get_width(ctx);i+=80)
	  {for(int j=0;j<strm_get_height(ctx);j+=80){
	  
	      strm_draw_rect(i,j,width,height ,  rand(), ctx);
	    }}

   
      }
    strm_end(ctx);
    strm_free_cntxt(ctx);

    return 0;
}
