#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

const float FPS = 18.2;
static ALLEGRO_TIMER *timer = NULL;
static ALLEGRO_DISPLAY *display = NULL;
static ALLEGRO_EVENT_QUEUE *event_queue = NULL;
static ALLEGRO_COLOR palette[512];

void init_allegro();
void destroy_allegro();
void create_cool_palette();
void redraw_road();


void init_allegro(){
  if(!al_init()) {
    fprintf(stderr, "Failed to initialize allegro!\n");
  }

  if(!al_install_keyboard()) {
    fprintf(stderr, "Failed to initialize the keyboard!\n");
  }

  if(!al_init_primitives_addon()) {
    fprintf(stderr, "failed to create primitives addon!\n");
  }

  timer = al_create_timer(1.0 / FPS);
  if(!timer) {
    fprintf(stderr, "failed to create timer!\n");
  }

  display = al_create_display(640, 480);
  if(!display){
    fprintf(stderr, "Failed to create display!\n");
  }

  event_queue = al_create_event_queue();
  if(!event_queue) {
    fprintf(stderr, "failed to create event_queue!\n");
  }

  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  al_register_event_source(event_queue, al_get_timer_event_source(timer));

  atexit(destroy_allegro);
}

void destroy_allegro(){
  timer = al_create_timer(1.0 / FPS);
  if(timer) {
    al_destroy_timer(timer);
  }

  if(!display){
    al_destroy_display(display);
  }

  if(!event_queue) {
    al_destroy_event_queue(event_queue);
  }
}
void create_cool_palette(void){
  // this function creates a cool palette. 64 shades of gray, 64 of red,
  // 64 of green and finally 64 of blue.

  ALLEGRO_COLOR color;
  int index;

  // swip thru the color registers and create 4 banks of 64 colors

  for (index=0; index < 64; index++){
    // grays
    color = al_map_rgb(index, index, index);
    palette[index] = color;

    // reds
    color = al_map_rgb(index, 0, 0);
    palette[index+64] = color;

    // greens
    color = al_map_rgb(0, index, 0);
    palette[index+128] = color;

    // blues
    color = al_map_rgb(0, 0, index);
    palette[index+192] = color;

  } // end index

} // end Create_Cool_Palette

void redraw_road(curr_color_index){
  int x1=310,            // x1 & x2 are the edges of the current piece of the road
      x2=330,
      y=0;               // y is the current y position of the piece of road
  // draw a road to nowhere
  for (y=192; y<480; y++){
    // draw next horizontal piece of road
    al_draw_line(x1, y, x2, y, palette[curr_color_index], 1.0);

    // make the road wider
    if (--x1 < 0)
      x1=0;

    if (++x2 > 639)
      x2=639;

    // next color please
    if (++curr_color_index>255)
      curr_color_index=1;

  } // end for

  al_flip_display();
}

int main(void){
  init_allegro();
  int curr_color_index=1;      // the current color being drawn
  bool redraw = true;

  create_cool_palette();
  al_clear_to_color(al_map_rgb(0,0,0));
  al_flip_display();
  al_start_timer(timer);


  while(true){
    ALLEGRO_EVENT ev;
    al_wait_for_event(event_queue, &ev);

    if(ev.type == ALLEGRO_EVENT_TIMER) {
      redraw = true;
    }
    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE || ev.type == ALLEGRO_EVENT_KEY_UP) {
      break;
    }
    if(redraw && al_is_event_queue_empty(event_queue)) {
      redraw = false;
      if (++curr_color_index>255)
        curr_color_index=1;
      redraw_road(curr_color_index);
    }
  }

  return 0;
}
