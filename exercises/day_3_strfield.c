#include <stdio.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#define NUM_STARS  75

#define PLANE_1    1
#define PLANE_2    2
#define PLANE_3    3

#define SCREEN_WIDTH      (unsigned int)320
#define SCREEN_HEIGHT     (unsigned int)200

const float FPS = 18.2;
static ALLEGRO_TIMER *timer = NULL;
static ALLEGRO_DISPLAY *display = NULL;
static ALLEGRO_EVENT_QUEUE *event_queue = NULL;
static ALLEGRO_FONT *default_font = NULL;

// P R O T O T Y P E S ///////////////////////////////////////////////////////

void init_allegro();
void destroy_allegro();
void init_stars(void);

// S T R U C T U R E S ///////////////////////////////////////////////////////

// data structure for a single star

typedef struct star_typ
        {
        int x,y;    // position of star
        int plane;  // which plane is star in
        ALLEGRO_COLOR color;  // color of star

        } star, *star_ptr;


// G L O B A L S /////////////////////////////////////////////////////////////

star stars[NUM_STARS]; // the star field

int velocity_1=2,      // the speeds of each plane
    velocity_2=4,
    velocity_3=6;

///////////////////////////////////////////////////////////////////////////

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

  al_init_font_addon();
  default_font = al_create_builtin_font();
  if (!default_font)
    fprintf(stderr, "Failed to create builtin font");

  timer = al_create_timer(1.0 / FPS);
  if(!timer) {
    fprintf(stderr, "failed to create timer!\n");
  }

  display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
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

/////////////////////////////////////////////////////////////////////////////////

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

  if (default_font) {
    al_destroy_font(default_font);
  }
}

///////////////////////////////////////////////////////////////////////////////////

void init_stars(void)
{

// this function will initialize the star field

int index;

// for each star choose a position, plane and color

for (index=0; index<NUM_STARS; index++)
    {
    // initialize each star to a velocity, position and color

    stars[index].x     = rand()%320;
    stars[index].y     = rand()%180;

    // decide what star plane the star is in

    switch(rand()%3)
          {
          case 0: // plane 1- the farthest star plane
               {
               // set velocity and color

               stars[index].plane = 1;
               stars[index].color = al_map_rgb(67, 67, 67);

               } break;

          case 1: // plane 2-The medium distance star plane
               {

               stars[index].plane = 2;
               stars[index].color = al_map_rgb(154, 154, 154);

               } break;

          case 2: // plane 3-The nearest star plane
               {

               stars[index].plane = 3;
               stars[index].color = al_map_rgb(255, 255, 255);

               } break;

          } // end switch

    } // end for index

} // end init_stars

//////////////////////////////////////////////////////////////////////////////

void draw_stars(){
  // move the star fields

  al_clear_to_color(al_map_rgb(0,0,0));

  al_draw_text(default_font, al_map_rgb(1,0,112), 0, 0, ALLEGRO_ALIGN_LEFT, "Press '+' or '-' to change speed.");
  al_draw_text(default_font, al_map_rgb(21,158,2), 320/2, 190, ALLEGRO_ALIGN_CENTRE, "Press 'Q' to exit.");
  for (int index=0; index<NUM_STARS; index++){
    // erase the star

    al_draw_pixel(stars[index].x,stars[index].y,al_map_rgb(0,0,0));
    // move the star and test for off screen condition

    // each star is in a different plane so test which plane star is
    // in so that proper velocity may be used
     switch(stars[index].plane) {
       case PLANE_1: // the slowest plane
         stars[index].x+=velocity_1;
         break;

       case PLANE_2: // the medium speed plane
         stars[index].x+=velocity_2;
         break;

       case PLANE_3: // the fastest plane (near)
         stars[index].x+=velocity_3;
         break;
     } // end switch

       // test if star went off screen

       if (stars[index].x > 319 ) // off right edge?
         stars[index].x=(stars[index].x-320); // wrap around
       else if (stars[index].x < 0) // off left edge?
         stars[index].x = (320+stars[index].x); // wrap around

       // draw the star at new position
       al_draw_pixel(stars[index].x,stars[index].y,stars[index].color);

  } // end for

  al_flip_display();

}

//////////////////////////////////////////////////////////////////////////////

int main(void){
  init_allegro();
  bool redraw = true;
  bool done = false;

  al_clear_to_color(al_map_rgb(0,0,0));
  al_flip_display();
  al_start_timer(timer);

  init_stars();

  while(!done){
    ALLEGRO_EVENT event;
    al_wait_for_event(event_queue, &event);

    switch (event.type) {
      case ALLEGRO_EVENT_TIMER:
        redraw = true;
        break;

      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        done = true;
        break;

      case ALLEGRO_EVENT_KEY_UP:
        switch(event.keyboard.keycode){
          case ALLEGRO_KEY_MINUS:
            // decrease velocity of each plane
            velocity_1-=1;
            velocity_2-=2;
            velocity_3-=3;
            break;

          case ALLEGRO_KEY_EQUALS:
            // decrease velocity of each plane
            velocity_1+=1;
            velocity_2+=2;
            velocity_3+=3;
            break;

          case ALLEGRO_KEY_Q:
            done = true;
            break;
        }
    }

    if(redraw && al_is_event_queue_empty(event_queue)) {
      redraw = false;
      draw_stars();
    }
  }

  return 0;
}
