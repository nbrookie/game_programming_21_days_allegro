#include <stdio.h>
#include <allegro5/allegro5.h>

int main(int argc, char **argv){

  ALLEGRO_DISPLAY *display = NULL;
  ALLEGRO_EVENT_QUEUE *event_queue = NULL;
  bool doexit = false;

  if(!al_init()) {
    fprintf(stderr, "Failed to initialize allegro!\n");
    return -1;
  }

  if(!al_install_keyboard()) {
    fprintf(stderr, "Failed to initialize the keyboard!\n");
    return -1;
  }

  display = al_create_display(640, 480);
  if(!display){
    fprintf(stderr, "Failed to create display!\n");
    return -1;
  }

  event_queue = al_create_event_queue();
  if(!event_queue) {
     fprintf(stderr, "failed to create event_queue!\n");
     al_destroy_display(display);
     return -1;
  }

  al_register_event_source(event_queue, al_get_display_event_source(display));
  al_register_event_source(event_queue, al_get_keyboard_event_source());

  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_flip_display();

  while(!doexit){
    ALLEGRO_EVENT ev;
    al_wait_for_event(event_queue, &ev);

    if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE || ev.type == ALLEGRO_EVENT_KEY_UP) {
      doexit = true;
    }
  }

  al_destroy_display(display);
  al_destroy_event_queue(event_queue);

  return 0;

}
