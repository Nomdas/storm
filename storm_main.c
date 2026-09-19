#include "storm.h"


int main()
{

  strm_cntxt *ctx=strm_init_cntxt();
  
  ctx->setting.log=STRM_LOG_OFF;
  strm_set_deflt_cntxt(ctx);
  
  strm_begin(ctx);
   
   
    while (ctx->state == 1)
      {
    strm_color cl1=strm_rand_color();
    strm_color cl2 = strm_rand_color();
      
    strm_draw_window("Storm Window",480,360,0,0,1,strm_rand_color(),strm_rand_color(),ctx);	 
      
     	     
    strm_draw_rect(0,0,36,48,0xFF0000,ctx);
    strm_draw_rect(0,49,36,48,0x00FF00,ctx);

      }
   
    
   
    
    strm_end(ctx);
    strm_free_cntxt(ctx);
    return 0;
}
