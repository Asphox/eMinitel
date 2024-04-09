//
// Created by asphox on 1/15/24.
//

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../minitel_core/Interfaces.h"
#include <SFML/Window.hpp>
#include <deque>

class Keyboard
{
    std::deque<mtlc_KeyEvent> key_events;
public:
    void process_event(const sf::Event& event);

    static mtlc_KeyEvent s_mtlc_keyboard_control(MTLC_OP_KEYBOARD op, intptr_t param, void* ctx);

    mtlc_KeyEvent pull_key();
};


#endif //KEYBOARD_H
