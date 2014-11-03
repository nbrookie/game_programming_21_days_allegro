
// I N C L U D E S ///////////////////////////////////////////////////////////

#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

const float FPS = 1000;
static ALLEGRO_TIMER *timer = NULL;
static ALLEGRO_DISPLAY *display = NULL;
static ALLEGRO_EVENT_QUEUE *event_queue = NULL;

void init_allegro();
void destroy_allegro();

typedef struct {
  float x, y;
  ALLEGRO_COLOR color;
} Point;

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

  al_set_new_display_option(ALLEGRO_SWAP_METHOD, 1, ALLEGRO_REQUIRE);
  display = al_create_display(320, 200);
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

// M A I N ///////////////////////////////////////////////////////////////////

int main(void){
  init_allegro();
  bool redraw = true;
  int index = 0;

  Point points[3500];
  al_clear_to_color(al_map_rgb(0,0,0));
  al_flip_display();
  al_start_timer(timer);

  for(int i=0; i < 3500; i++){
    Point *p = &points[i];
    p->x = rand()%320;
    p->y = rand()%200;
    p->color = al_map_rgb(rand()%256, rand()%256, rand()%256);
  }


  while(true){
    ALLEGRO_EVENT ev;
    al_wait_for_event(event_queue, &ev);
    if(ev.type == ALLEGRO_EVENT_TIMER && index < 3500) {
      for(int j=0; j < index; j++){
        Point p = points[j];
        al_draw_pixel(p.x, p.y, p.color);
      }
      al_flip_display();
      index++;
    }
    else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE || ev.type == ALLEGRO_EVENT_KEY_UP) {
      break;
    }
  }

  return 0;
} // end main




