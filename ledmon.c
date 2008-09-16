#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/XKBlib.h>
#include <stdio.h>
#include <stdlib.h>

enum {
  CapsLock = 0,
  NumLock,
  ScrollLock
};

void displayState(unsigned int state);

static int xkb_event_base = 0;
static int xkb_error_base = 0;

int main(void) {
  Display *disp;
  int opcode;
  unsigned int state;
  int maj = XkbMajorVersion;
  int min = XkbMinorVersion;
  XkbEvent ev;

  /* Open Display */
  if ( !(disp = XOpenDisplay(NULL))) {
    fprintf(stderr, "Can't open display: CHECK DISPLAY VARIABLE\n");
    exit(1);
  }

  if (!XkbLibraryVersion(&maj, &min)) {
    fprintf(stderr, "Couldn't get Xkb library version\n");
    exit(1);
  }
  
  if (!XkbQueryExtension(disp, &opcode, &xkb_event_base, &xkb_error_base, &maj, &min)) {
    fprintf(stderr, "XkbQueryExtension error\n");
    exit(1);
  }

  if (!XkbSelectEvents(disp, XkbUseCoreKbd, XkbIndicatorStateNotifyMask, 
                       XkbIndicatorStateNotifyMask)) {
    fprintf(stderr, "XkbSelectEvents\n");
    exit(1);
  }

  XkbGetIndicatorState(disp, XkbUseCoreKbd, &state);
  displayState(state);
  
  /*
  win = XCreateSimpleWindow(disp, DefaultRootWindow(display), 0, 0, 200, 300, 5, white,
                            black);
  */

  while (1) {
    XNextEvent(disp, &ev.core);
    if (ev.type == xkb_event_base && ev.any.xkb_type == XkbIndicatorStateNotify) {
      displayState(ev.indicators.state);
    }
  }
  return 0;
}

void displayState(unsigned int state) {
  if (state & (1 << 0)) {
    printf("CAPS ");
  }
  if (state & (1 << 1)) {
    printf("NUM ");
  }
  if (state & (1 << 2)) {
    printf("SCROLL");
  }
  printf("\n");
}
