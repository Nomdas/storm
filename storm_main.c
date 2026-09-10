#include "storm.h"


int main()
{

  strm_cntxt *ctx;
  ctx->setting.log=STRM_LOG_ON; 
 strm_set_deflt_cntxt(ctx);
 
  strm_begin(ctx);
  
  
    while (ctx->state == 1)
      {
    strm_color cl1=strm_rand_color();
    strm_color cl2 = strm_rand_color();
    
      strm_draw_window("Storm Window",480,360,0,0,1,cl1,cl2,ctx);
	strm_draw_rect(0,100,50,50,0x0000FF,ctx);
         
	
      }
        
      strm_end(ctx);

  return 0;
}
