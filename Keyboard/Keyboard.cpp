//
// Created by asphox on 1/15/24.
//

#include <iostream>

#include "Keyboard.h"

void Keyboard::process_event(const sf::Event& event)
{
    if(event.type != sf::Event::KeyPressed && event.type != sf::Event::KeyReleased && event.type != sf::Event::TextEntered)
        return;
    mtlc_KeyEvent m_ke;
    m_ke.kc = KC_NONE;
    if(event.type == sf::Event::KeyPressed)
    {
        switch(event.key.code)
        {
            case sf::Keyboard::LShift:
                m_ke.lshft = true;
                break;
            case sf::Keyboard::RShift:
                m_ke.rshft = true;
                break;
            case sf::Keyboard::LControl:
            case sf::Keyboard::RControl:
                m_ke.ctrl = true;
                break;
            case sf::Keyboard::LAlt:
            case sf::Keyboard::RAlt:
                m_ke.fnct = true;

            case sf::Keyboard::F1:
                m_ke.kc = KC_GUIDE; break;  // F1 <=> HELP
            case sf::Keyboard::F2:
                m_ke.kc = KC_RETURN; break;
            case sf::Keyboard::F3:
                m_ke.kc = KC_NEXT; break;
            case sf::Keyboard::Enter:
            {
               if(m_ke.lshft || m_ke.rshft)
                   m_ke.kc = KC_ENTER;
               else
                   m_ke.kc = KC_SEND;
               break;
            }

            case sf::Keyboard::A: m_ke.kc = KC_A; break;
            case sf::Keyboard::B: m_ke.kc = KC_B; break;
            case sf::Keyboard::C: m_ke.kc = KC_C; break;
            case sf::Keyboard::D: m_ke.kc = KC_D; break;
            case sf::Keyboard::E: m_ke.kc = KC_E; break;
            case sf::Keyboard::F: m_ke.kc = KC_F; break;
            case sf::Keyboard::G: m_ke.kc = KC_G; break;
            case sf::Keyboard::H: m_ke.kc = KC_H; break;
            case sf::Keyboard::I: m_ke.kc = KC_I; break;
            case sf::Keyboard::J: m_ke.kc = KC_J; break;
            case sf::Keyboard::K: m_ke.kc = KC_K; break;
            case sf::Keyboard::L: m_ke.kc = KC_L; break;
            case sf::Keyboard::M: m_ke.kc = KC_M; break;
            case sf::Keyboard::N: m_ke.kc = KC_N; break;
            case sf::Keyboard::O: m_ke.kc = KC_O; break;
            case sf::Keyboard::P: m_ke.kc = KC_P; break;
            case sf::Keyboard::Q: m_ke.kc = KC_Q; break;
            case sf::Keyboard::R: m_ke.kc = KC_R; break;
            case sf::Keyboard::S: m_ke.kc = KC_S; break;
            case sf::Keyboard::T: m_ke.kc = KC_T; break;
            case sf::Keyboard::U: m_ke.kc = KC_U; break;
            case sf::Keyboard::V: m_ke.kc = KC_V; break;
            case sf::Keyboard::W: m_ke.kc = KC_W; break;
            case sf::Keyboard::X: m_ke.kc = KC_X; break;
            case sf::Keyboard::Y: m_ke.kc = KC_Y; break;
            case sf::Keyboard::Z: m_ke.kc = KC_Z; break;

            case sf::Keyboard::Num0:
            case sf::Keyboard::Numpad0:
                m_ke.kc = KC_0; break;
            case sf::Keyboard::Num1:
            case sf::Keyboard::Numpad1:
                m_ke.kc = KC_1; break;
            case sf::Keyboard::Num2:
            case sf::Keyboard::Numpad2:
                m_ke.kc = KC_2; break;
            case sf::Keyboard::Num3:
            case sf::Keyboard::Numpad3:
                m_ke.kc = KC_3; break;
            case sf::Keyboard::Num4:
            case sf::Keyboard::Numpad4:
                m_ke.kc = KC_4; break;
            case sf::Keyboard::Num5:
            case sf::Keyboard::Numpad5:
                m_ke.kc = KC_5; break;
            case sf::Keyboard::Num6:
            case sf::Keyboard::Numpad6:
                m_ke.kc = KC_6; break;
            case sf::Keyboard::Num7:
            case sf::Keyboard::Numpad7:
                m_ke.kc = KC_7; break;
            case sf::Keyboard::Num8:
            case sf::Keyboard::Numpad8:
                m_ke.kc = KC_8; break;
            case sf::Keyboard::Num9:
            case sf::Keyboard::Numpad9:
                m_ke.kc = KC_9; break;
            case sf::Keyboard::Multiply:
                m_ke.kc = KC_AST; break;
            case sf::Keyboard::Dash:
                m_ke.kc = KC_NUM; break;

            default:
                break;
        }
    }
    else if(event.type == sf::Event::KeyReleased)
    {
        switch(event.key.code)
        {
            case sf::Keyboard::LShift:
                m_ke.lshft = false;
                break;
            case sf::Keyboard::RShift:
                m_ke.rshft = false;
                break;
            case sf::Keyboard::LControl:
            case sf::Keyboard::RControl:
                m_ke.ctrl = false;
                break;
            case sf::Keyboard::LAlt:
            case sf::Keyboard::RAlt:
                m_ke.fnct = false;
            default:
                break;
        }
    }
    else if(event.type == sf::Event::TextEntered)
    {
        switch(event.text.unicode)
        {
            case ',' : m_ke.kc = KC_COMMA; break;
            case '<' : m_ke.kc = KC_COMMA; m_ke.lshft = true; break;
            case '.' : m_ke.kc = KC_POINT; break;
            case '>' : m_ke.kc = KC_POINT; m_ke.lshft = true; break;
            case '\'': m_ke.kc = KC_APOS; break;
            case '@' : m_ke.kc = KC_APOS; m_ke.lshft = true; break;
            case ';' : m_ke.kc = KC_SEMI; break;
            case '+' : m_ke.kc = KC_SEMI; m_ke.lshft = true; break;
            case '-' : m_ke.kc = KC_MINUS; break;
            case '=' : m_ke.kc = KC_MINUS; m_ke.lshft = true; break;
            case ':' : m_ke.kc = KC_COLON; break;
            case '*' : m_ke.kc = KC_AST; break;
            case '?' : m_ke.kc = KC_QUEST; break;
            case '/' : m_ke.kc = KC_QUEST; m_ke.lshft = true; break;
            case ' ' : m_ke.kc = KC_SPACE; break;
        }
    }

    if(m_ke.kc != KC_NONE)
        key_events.push_back(m_ke);
}

mtlc_KeyEvent Keyboard::s_mtlc_keyboard_control(MTLC_OP_KEYBOARD op, intptr_t param, void* ctx)
{
    mtlc_KeyEvent ke;
    if(!ctx)
        return ke;

    auto _this = (Keyboard*)ctx;
    switch(op)
    {
        case OPK_PULL_KEY:
        {
            ke = _this->pull_key();
            break;
        }
        default:
            break;
    }

    return ke;
}

mtlc_KeyEvent Keyboard::pull_key()
{
    mtlc_KeyEvent ke;
    ke.kc = KC_NONE;
    if(key_events.size() > 0)
    {
        ke = key_events.front();
        key_events.pop_front();
    }
    return ke;
}