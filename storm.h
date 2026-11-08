
/* Author by : ahmed khider(2026)*/

#ifndef STORM_UTILITY
#define STORM_UTILITY

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>


#include <X11/Xlib.h>
#include <X11/Xutil.h>


#define STRM_NORMAL "\033[0m"
#define STRM_RED    "\033[1;31m"
#define STRM_GREEN  "\033[1;32m"
#define STRM_YELLOW "\033[1;33m" 
#define STRM_BLUE   "\033[1;34m"


typedef unsigned long strm_color;
typedef char byte; 
typedef char *strm_str ;
typedef char *strm_txt_color;

typedef enum {
  STRM_SUCCESS = 1,
  STRM_WARNING,
  STRM_ERROR,
  STRM_ENTERACT
  } strm_state;


#define  STRM_LOG_NONE 0
#define  STRM_LOG_OFF  1
#define  STRM_LOG_ON   2
#define ZERO_CHAR '\n'


typedef struct
{
  Atom wm_delete_win;
}strm_wm;
  
typedef struct
{
  Display *dis;
  Window win;
  Window root;
  int scn;
  XImage *img;
  strm_wm wm;
  XEvent evt;
  char *pixels;
  int w;
  int h;
  
} strm_x11;

typedef struct
{
  int log;
  int fps;
  clock_t last_time;
  clock_t current_time;
  double delta_time;
} strm_set;
  
typedef struct
{
  const char * title;
  strm_x11     x11;
  
} strm_window;

typedef struct strm_cntxt
{
  bool state;
  bool init;
  strm_window window;
  strm_set    setting;
  
} strm_cntxt;

extern strm_cntxt *global_ctx;


strm_cntxt*  strm_init_cntxt();

void strm_free_cntxt(strm_cntxt *ctx);

void strm_begin(strm_cntxt *ctx);

void strm_end(strm_cntxt *ctx);

strm_cntxt strm_deflt_cntxt();

void strm_set_deflt_cntxt(strm_cntxt *ctx);

int  strm_win_state(strm_cntxt *ctx);

void strm_window_init(strm_cntxt *ctx);


void strm_draw_window(char *title,int w, int h,int x, int y
    ,int bw,strm_color brc, strm_color bgc , strm_cntxt *ctx);


void strm_draw_rect(int x,int y,unsigned int w,unsigned int h,strm_color clr,strm_cntxt *ctx);

int strm_get_width(strm_cntxt *ctx);
int strm_get_height(strm_cntxt *ctx);


strm_txt_color strm_log_color(strm_state state);

strm_txt_color strm_reset_color();

void strm_log(strm_str log, strm_state state);

bool strm_term_support_clr();

#endif /* STORM_UTILITY*/
