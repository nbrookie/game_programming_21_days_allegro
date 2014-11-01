#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

const float FPS = 18.2;
static ALLEGRO_TIMER *timer = NULL;
static ALLEGRO_DISPLAY *display = NULL;
static ALLEGRO_EVENT_QUEUE *event_queue = NULL;

void init_allegro();
void destroy_allegro();

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

int main(void){
  init_allegro();
  bool redraw = true;

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
    }
  }

  return 0;
}
