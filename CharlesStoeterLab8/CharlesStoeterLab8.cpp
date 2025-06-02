#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>




const int SCREEN_W = 900;
const int SCREEN_H = 800;


float bee_x = 400;  
float bee_y = 300;
float bee_dx = 4;   
float bee_dy = 0;

bool paused = false;

int flip_flag = 0;

float angle = 0.0f;
bool rotating = false;



int main() {



    // Initialize Allegro
    if (!al_init()) {
        std::cerr << "Failed to initialize Allegro!\n";
        return -1;
    }



    // Create the display
    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        std::cerr << "Failed to create display!\n";
        return -1;
    }



    // Install keyboard support
    if (!al_install_keyboard()) {
        std::cerr << "Failed to install keyboard!\n";
        return -1;
    }



    // Create timer (60 FPS)
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    if (!timer) {
        std::cerr << "Failed to create timer!\n";
        return -1;
    }



    // Create event queue
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    if (!event_queue) {
        std::cerr << "Failed to create event queue!\n";
        return -1;
    }



	// Initialize the image addon
    al_init_image_addon();

    // Load background and bee
    ALLEGRO_BITMAP* background = al_load_bitmap("field.jpg");
    if (!background) {
        std::cerr << "Failed to load background (field.jpg)!\n";
        return -1;
    }




    ALLEGRO_BITMAP* bee = al_load_bitmap("bee.png");
    if (!bee) {
        std::cerr << "Failed to load bee (bee.png)!\n";
        return -1;
    }

    // converts white to transparent
    al_convert_mask_to_alpha(bee, al_map_rgb(255, 255, 255));








    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));




    al_start_timer(timer);



    bool running = true;
    bool redraw = true;
    while (running) {



        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);




        // Handle events
        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER) {


            if (!paused && !rotating) {
                bee_x += bee_dx;
                bee_y += bee_dy;

                // Check for left/right edge
                if (bee_x <= 0 || bee_x >= SCREEN_W - al_get_bitmap_width(bee)) {
                    rotating = true;
                }
                // Check for top/bottom edge
                else if (bee_y <= 0 || bee_y >= SCREEN_H - al_get_bitmap_height(bee)) {
                    rotating = true;
                }
            }

            // Rotate if needed
            if (rotating) {
                angle += 0.1f;
                if (angle >= 3.14f) {

					
                    angle = 0.0f;
                    
                    rotating = false;// Done rotating

                    bee_dx = -bee_dx;// Reverse direction

                    bee_dy = -bee_dy;

                    flip_flag = 0; // Reset flip 
                }
            }


            redraw = true;
        }




        else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {




            if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                running = false;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {

                paused = !paused;


            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_RIGHT) {


                bee_dx = 4;
                bee_dy = 0;
                flip_flag = 0; // No flip


            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {


                bee_dx = -4;
                bee_dy = 0;
                flip_flag = ALLEGRO_FLIP_HORIZONTAL;


            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {


                bee_dx = 0;
                bee_dy = -4;
                flip_flag = 0; // No flip


            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) {


                bee_dx = 0;
                bee_dy = 4;
                flip_flag = ALLEGRO_FLIP_VERTICAL;


            }

        }





        // Draw screen
        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;

            // Draw background
            al_draw_bitmap(background, 0, 0, 0);

            // Draw bee in center 
            al_draw_rotated_bitmap(bee, al_get_bitmap_width(bee) / 2, al_get_bitmap_height(bee) / 2,  bee_x + al_get_bitmap_width(bee) / 2, bee_y + al_get_bitmap_height(bee) / 2, angle, flip_flag);



            al_flip_display();




        }
    }







 

    // Clean up
    al_destroy_bitmap(bee);
    al_destroy_bitmap(background);
    al_destroy_display(display);

    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    return 0;
}