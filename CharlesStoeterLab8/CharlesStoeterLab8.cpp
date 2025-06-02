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




    // Keep window open for now
    al_rest(3.0); // Show display for 3 seconds (for test)

    // Clean up
    al_destroy_display(display);

    return 0;
}