//-----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2016 Thomas Steinholz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//-----------------------------------------------------------------------------

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#include <cstdio>

#include "gameobjects/graphics/sprite.h"

int main() {

    srand((unsigned int) time(NULL));

    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *queue;
    ALLEGRO_TIMER *timer;

     if (!al_init()) {
        printf("al_init Failed!\n");
        exit(-1);
    }
    if (!al_install_audio()) {
        fprintf(stderr, "Failed to initialize audio!\n");
        exit(-1);
    }
    if (!al_init_acodec_addon()) {
        fprintf(stderr, "Failed to initialize audio codecs!\n");
        exit(-1);
    }
    if (!al_reserve_samples(1)) {
        fprintf(stderr, "Failed to reserve samples!\n");
        exit(-1);
    }
    if (!al_install_mouse()) {
        fprintf(stderr, "Failed to initialize the mouse!\n");
        exit(-1);
    }
    if (!al_init_primitives_addon()) {
        fprintf(stderr, "al_init_primitives_addon Failed!\n");
        exit(-1);
    }

    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_init_image_addon();

    display = al_create_display(800, 600);
    if (!display) {
        printf("al_create_display Failed!\n");
        exit(-1);
    }

    //LOAD ASSETS///////////////////////////////////////////////////////

    ALLEGRO_BITMAP *background = al_load_bitmap("res/background.png");
    if (!background) {
        printf("Couldn't load background!\n");
        exit(-1);
    }

    ALLEGRO_BITMAP *sprite_dino = al_load_bitmap("res/dino.png");
    if (!sprite_dino) {
        printf("Couldn't load dino!\n");
        exit(-1);
    }

    ALLEGRO_FONT *font = al_load_font("res/league-gothic.ttf", 30, 0);
    if (!font) {
        printf("Couldn't load font!\n");
        exit(-1);
    }

    Sprite *dino = new Sprite(sprite_dino, 4, 5);
    dino->Play(400, 300, 0, true);

    ////////////////////////////////////////////////////////////////////

    queue = al_create_event_queue();
    timer = al_create_timer(1.0 / 15);

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));
    al_register_event_source(queue, al_get_timer_event_source(timer));

    al_start_timer(timer);

    bool render = true, executing = true;
    int bgx = 0, bgv = 0;

    while (executing) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);
        //UPDATE////////////////////////////////////////////////////////////

        dino->Update(&event);
        switch (event.type) {
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                executing = false;
                break;
            case ALLEGRO_EVENT_KEY_DOWN:
                executing = event.keyboard.keycode != ALLEGRO_KEY_ESCAPE;
                switch(event.keyboard.keycode) {
                case ALLEGRO_KEY_RIGHT:
                case ALLEGRO_KEY_D:
                    bgv = 5;
                    dino->Play(400, 300, 1, true);
                    break;
                case ALLEGRO_KEY_LEFT:
                case ALLEGRO_KEY_A:
                    bgv = -5;
                    dino->Play(400, 300, 3, true);
                    break;
                }
                break;
            case ALLEGRO_EVENT_KEY_UP:
                bgv = 0;
                break;
            case ALLEGRO_EVENT_TIMER:
                break;
            default:
                break;
        }

        if (bgx <= -1) {
            bgv = 0;
            bgx = 0;
        } else if (bgx >= 801) {
            bgv = 0;
            bgx = 800;
        }
        bgx += bgv;

        //UPDATE////////////////////////////////////////////////////////////

        //if (al_is_event_queue_empty(queue) && render) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            al_set_target_bitmap(al_get_backbuffer(display));
            //RENDER////////////////////////////////////////////////////////////

            al_draw_scaled_bitmap(background, bgx, 0, 800, 600, 0, 0, 800, 600, 0);
            al_draw_text(font, al_map_rgb(255, 255, 255), 5, 0, ALLEGRO_ALIGN_LEFT, "A - To go left");
            al_draw_text(font, al_map_rgb(255, 255, 255), 5, 30, ALLEGRO_ALIGN_LEFT, "D - To go right");
            dino->Render();

            //RENDER////////////////////////////////////////////////////////////
            al_flip_display();
        //}
        render = false;
    }
    al_destroy_display(display);
    return 0;
}
