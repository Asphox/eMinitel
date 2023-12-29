//
// Created by asphox on 12/26/23.
//

#ifndef MINITEL_H
#define MINITEL_H

#include "../Screen/Screen.h"
#include <deque>

class Minitel : public sf::Drawable, public sf::Transformable
{
private:
    std::deque<char> m_inputs_buffer;
    Screen m_screen;
    struct CursorPos
    {
        unsigned int col = 0;
        unsigned int line = 1;
    }m_cursor_pos;

    struct Context
    {
        bool is_escaped_to_G2 = false;
        bool is_escaped_to_G1 = false;
        struct
        {
            GLYPH_ATT_FCOLOR fcolor = GFC_WHITE;
            bool             blink  = false;
            GLYPH_ATT_SIZE   size   = GS_NORMAL;
            bool             negative = false;
        }current_char_attributes;
        GLYPH_CODE last_glyph = GC_SPACE;
        CursorPos last_main_screen_cursor_pos;

        void reset(){ *this = Context(); }
        // resets escape, attributes, but not page/rolling mode
        void reset_article()
        {
            *this = Context();
        }
    }m_context;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void __pull_data_from_inputs();
    void __process_inputs();
public:
    Minitel(float height);
    void reset();
    void update();

// =====================================================
//      functionalities
// =====================================================
enum RESULT_FUNC
{
    RF_OK,
    RF_IGNORED,
    RF_INSUFFICIENT_DATA,
    RF_ERR,
};
private:
    void        __write_glyph(GLYPH_CODE gc);
    CursorPos   __get_cursor_pos();
    void        __set_cursor_pos(CursorPos pos);
    void        __push_cursor_left(bool loop);
    void        __push_cursor_right(bool loop);
    void        __push_cursor_down(bool loop);
    void        __push_cursor_up(bool loop);
    void        __write_char(char c);


    RESULT_FUNC __func_BS(int& nb_of_byte_concerned);
    RESULT_FUNC __func_HT(int& nb_of_byte_concerned);
    RESULT_FUNC __func_LF(int& nb_of_byte_concerned);
    RESULT_FUNC __func_VT(int& nb_of_byte_concerned);
    RESULT_FUNC __func_FF(int& nb_of_byte_concerned);
    RESULT_FUNC __func_CR(int& nb_of_byte_concerned);
    RESULT_FUNC __func_SO(int& nb_of_byte_concerned);
    RESULT_FUNC __func_SI(int& nb_of_byte_concerned);
    RESULT_FUNC __func_CON(int& nb_of_byte_concerned);
    RESULT_FUNC __func_REP(int& nb_of_byte_concerned);
    RESULT_FUNC __func_COFF(int& nb_of_byte_concerned);
    RESULT_FUNC __func_CAN(int& nb_of_byte_concerned);
    RESULT_FUNC __func_SS2(int& nb_of_byte_concerned);
    RESULT_FUNC __func_RS(int& nb_of_byte_concerned);
    RESULT_FUNC __func_US(int& nb_of_byte_concerned);
    RESULT_FUNC __func_write_char(int& nb_of_byte_concerned);

    //==================================================================
    // Escaped Functionalities
    //==================================================================
    RESULT_FUNC __func_ESC(int& nb_of_byte_concerned);
    // called by __func_ESC during ISO 6429 escaping
    RESULT_FUNC __func_CSI(int& nb_of_byte_concerned);
    // clears from cursor included to end of screen
    RESULT_FUNC __func_CSI_J(int& nb_of_byte_concerned);
    // same as __func_CSI_J
    RESULT_FUNC __func_CSI_0J(int& nb_of_byte_concerned);
    // clears from screen's start to cursor included
    RESULT_FUNC __func_CSI_1J(int& nb_of_byte_concerned);
    // clears all screen (cursor is not moved)
    RESULT_FUNC __func_CSI_2J(int& nb_of_byte_concerned);
    // clears from cursor included to end of line
    RESULT_FUNC __func_CSI_K(int& nb_of_byte_concerned);
    // same as __func_CSI_K
    RESULT_FUNC __func_CSI_0K(int& nb_of_byte_concered);
    // clears from current line's start to cursor (included)
    RESULT_FUNC __func_CSI_1K(int& nb_of_byte_concered);
    // clears all the current line
    RESULT_FUNC __func_CSI_2K(int& nb_of_byte_concered);
    // cursor goes up n-times, no looping
    RESULT_FUNC __func_CSI_A(int& nb_of_byte_concerned);
    // cursor goes down n-times, no looping
    RESULT_FUNC __func_CSI_B(int& nb_of_byte_concerned);
    // cursor goes right n-times, no looping
    RESULT_FUNC __func_CSI_C(int& nb_of_byte_concerned);
    // cursor goes left n-times, no looping
    RESULT_FUNC __func_CSI_D(int& nb_of_byte_concerned);
    // direct addressing for cursor
    RESULT_FUNC __func_CSI_H(int& nb_of_byte_concerned);
    // inserts n lines from current cursor line
    RESULT_FUNC __func_CSI_L(int& nb_of_byte_concerned);
    // deletes n lines from current cursor line
    RESULT_FUNC __func_CSI_M(int& nb_of_byte_concerned);
    // deletes n chars starting on cursor included
    RESULT_FUNC __func_CSI_P(int& nb_of_byte_concerned);
    // starts char-insertion mode
    RESULT_FUNC __func_CSI_h(int& nb_of_byte_concerned);
    // ends char-insertion mode
    RESULT_FUNC __func_CSI_i(int& nb_of_byte_concerned);



};


#endif //MINITELVIEWER_MINITEL_H
