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

float angle = 0.0f;
bool rotating = false;
int flip_flag = 0;

int main() {
    if (!al_init()) {
        std::cerr << "Failed to initialize Allegro!\n";
        return -1;
    }

    ALLEGRO_DISPLAY* display = al_create_display(SCREEN_W, SCREEN_H);
    if (!display) {
        std::cerr << "Failed to create display!\n";
        return -1;
    }

    if (!al_install_keyboard()) {
        std::cerr << "Failed to install keyboard!\n";
        return -1;
    }

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    if (!timer) {
        std::cerr << "Failed to create timer!\n";
        return -1;
    }

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    if (!event_queue) {
        std::cerr << "Failed to create event queue!\n";
        return -1;
    }

    al_init_image_addon();

    ALLEGRO_BITMAP* background = al_load_bitmap("field.jpg");
    if (!background) {
        std::cerr << "Failed to load background!\n";
        return -1;
    }

    ALLEGRO_BITMAP* bee = al_load_bitmap("bee.png");
    if (!bee) {
        std::cerr << "Failed to load bee!\n";
        return -1;
    }

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

        if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        else if (ev.type == ALLEGRO_EVENT_TIMER) {
            if (!paused && !rotating) {
                bee_x += bee_dx;
                bee_y += bee_dy;

                // Wall collision triggers rotation
                if (bee_x <= 0 || bee_x >= SCREEN_W - al_get_bitmap_width(bee)) {
                    rotating = true;
                }
                if (bee_y <= 0 || bee_y >= SCREEN_H - al_get_bitmap_height(bee)) {
                    rotating = true;
                }
            }

            // Smooth rotation logic
            if (rotating) {
                angle += 0.1f;
                if (angle >= 3.14f) {
                    angle = 0.0f;
                    rotating = false;

                    bee_dx = -bee_dx;
                    bee_dy = -bee_dy;

                    // Flip flag adjusted based on direction
                    if (bee_dx < 0) flip_flag = ALLEGRO_FLIP_HORIZONTAL;
                    else if (bee_dx > 0) flip_flag = 0;
                    else if (bee_dy > 0) flip_flag = ALLEGRO_FLIP_VERTICAL;
                    else flip_flag = 0;
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
                flip_flag = 0;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_LEFT) {
                bee_dx = -4;
                bee_dy = 0;
                flip_flag = ALLEGRO_FLIP_HORIZONTAL;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_UP) {
                bee_dx = 0;
                bee_dy = -4;
                flip_flag = 0;
            }
            else if (ev.keyboard.keycode == ALLEGRO_KEY_DOWN) {
                bee_dx = 0;
                bee_dy = 4;
                flip_flag = ALLEGRO_FLIP_VERTICAL;
            }
        }

        if (redraw && al_is_event_queue_empty(event_queue)) {
            redraw = false;

            al_draw_bitmap(background, 0, 0, 0);

            // Rotation only while rotating
            al_draw_rotated_bitmap(
                bee,
                al_get_bitmap_width(bee) / 2,
                al_get_bitmap_height(bee) / 2,
                bee_x + al_get_bitmap_width(bee) / 2,
                bee_y + al_get_bitmap_height(bee) / 2,
                angle,
                flip_flag
            );

            al_flip_display();
        }
    }

    al_destroy_bitmap(bee);
    al_destroy_bitmap(background);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);

    return 0;
}
