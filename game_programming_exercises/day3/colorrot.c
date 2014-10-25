#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

ALLEGRO_COLOR palette[256];
const float FPS = 18.2;

void Create_Cool_Palette(void){
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

  // make color 0 black
  color = al_map_rgb(0, 0, 0);
  palette[0] = color;

} // end Create_Cool_Palette

void redraw_road(curr_color_index){
  al_clear_to_color(al_map_rgb(0,0,0));
  int index,           // loop var
      x1=150,            // x1 & x2 are the edges of the current piece of the road
      x2=170,
      y=0;               // y is the current y position of the piece of road
  // draw a road to nowhere
  for (y=80; y<200; y++){
    // draw next horizontal piece of road
    al_draw_line(x1, y, x2, y, palette[curr_color_index], 1.0);

    // make the road wider
    if (--x1 < 0)
      x1=0;

    if (++x2 > 319)
      x2=319;

    // next color please
    if (++curr_color_index>255)
      curr_color_index=1;

  } // end for

  al_flip_display();
}
int main(void){
  int index,           // loop var
      x1=150,            // x1 & x2 are the edges of the current piece of the road
      x2=170,
      y=0,               // y is the current y position of the piece of road
      curr_color_index=1;      // the current color being drawn
  bool doexit = false;
  ALLEGRO_TIMER *timer = NULL;
  bool redraw = true;


  ALLEGRO_DISPLAY *display = NULL;
  ALLEGRO_EVENT_QUEUE *event_queue = NULL;

  if(!al_init()) {
    fprintf(stderr, "Failed to initialize allegro!\n");
    return -1;
  }
  timer = al_create_timer(1.0 / FPS);
  if(!timer) {
    fprintf(stderr, "failed to create timer!\n");
    return -1;
  }

  if(!al_install_keyboard()) {
    fprintf(stderr, "Failed to initialize the keyboard!\n");
    return -1;
  }

  display = al_create_display(320, 200);
  if(!display){
    fprintf(stderr, "Failed to create display!\n");
    al_destroy_timer(timer);
    return -1;
  }

  event_queue = al_create_event_queue();
  if(!event_queue) {
    fprintf(stderr, "failed to create event_queue!\n");
    al_destroy_display(display);
    al_destroy_timer(timer);
    return -1;
  }

  bool primitives = al_init_primitives_addon();
  if(!primitives) {
    fprintf(stderr, "failed to create primitives addon!\n");
    al_destroy_display(display);
    return -1;
  }

  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_keyboard_event_source());
  al_register_event_source(event_queue, al_get_timer_event_source(timer));

  al_clear_to_color(al_map_rgb(0,0,0));

  al_flip_display();

  al_start_timer(timer);

  Create_Cool_Palette();



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

  al_destroy_display(display);
  al_destroy_timer(timer);
  al_destroy_event_queue(event_queue);

  return 0;
}
