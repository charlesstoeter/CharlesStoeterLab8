#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <iostream>




const int SCREEN_W = 900;
const int SCREEN_H = 800;



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

    ALLEGRO_BITMAP* bee = al_load_bitmap("bee.jpg");
    if (!bee) {
        std::cerr << "Failed to load bee (bee.jpg)!\n";
        return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));


    al_start_timer(timer);











    // Keep window open for now
    al_rest(3.0); // Show display for 3 seconds (for test)

    // Clean up
    al_destroy_bitmap(bee);
    al_destroy_bitmap(background);
    al_destroy_display(display);

    return 0;
}