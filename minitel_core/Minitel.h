//
// Created by asphox on 12/26/23.
//

#ifndef MINITEL_H
#define MINITEL_H

#include "Interfaces.h"
#include <deque>
#include <set>
#include <string>
#include <vector>
#include <functional>
#include <chrono>

namespace mtlc
{
    class Minitel
    {
    private:
        std::deque<char>             m_inputs_buffer;
        mtlc_module_control_screen   m_screen_control = nullptr;
        void*                        m_screen_ctx     = nullptr;
        mtlc_module_control_DIN      m_DIN_control    = nullptr;
        void*                        m_DIN_ctx        = nullptr;
        mtlc_module_control_keyboard m_keyboard_control = nullptr;
        void*                        m_keyboard_ctx     = nullptr;
        void(*m_callback_on_bell)(void*) = nullptr;
        void* m_bell_ctx = nullptr;

        std::chrono::time_point<std::chrono::steady_clock> last_time_data_pulled;

        struct CursorPos
        {
            std::uint8_t col = 0;
            std::uint8_t line = 1;
        }m_cursor_pos;

        enum class MODULE_CODE : std::uint8_t
        {
            SCREEN_OUT      = 0x50,
            KEYBOARD_OUT    = 0x51,
            MODEM_OUT       = 0x52,
            DIN_OUT         = 0x53,
            SCREEN_IN       = 0x58,
            KEYBOARD_IN     = 0x59,
            MODEM_IN        = 0x5A,
            DIN_IN          = 0x5B,
        };

        bool __activate_module_switch(MODULE_CODE out, MODULE_CODE in, bool activate)
        {
            std::uint8_t bit_offset = 0;
            switch(in)
            {
                case MODULE_CODE::DIN_IN:       bit_offset = 0; break;
                case MODULE_CODE::MODEM_IN:     bit_offset = 4; break;
                case MODULE_CODE::KEYBOARD_IN:  bit_offset = 8; break;
                case MODULE_CODE::SCREEN_IN:    bit_offset = 12; break;
                default:
                    return false;
            }
            switch(out)
            {
                case MODULE_CODE::DIN_OUT:       bit_offset += 0; break;
                case MODULE_CODE::MODEM_OUT:     bit_offset += 1; break;
                case MODULE_CODE::KEYBOARD_OUT:  bit_offset += 2; break;
                case MODULE_CODE::SCREEN_OUT:    bit_offset += 3; break;
                default:
                    return false;
            }
            m_context.m_modules_switch_status = (m_context.m_modules_switch_status & ~(1<<bit_offset)) | (activate << bit_offset);
            return true;
        }
        bool __get_module_switch_state(MODULE_CODE out, MODULE_CODE in)
        {
            std::uint8_t bit_offset = 0;
            switch(out)
            {
                case MODULE_CODE::DIN_OUT:       bit_offset = 0; break;
                case MODULE_CODE::MODEM_OUT:     bit_offset = 4; break;
                case MODULE_CODE::KEYBOARD_OUT:  bit_offset = 8; break;
                case MODULE_CODE::SCREEN_OUT:    bit_offset = 12; break;
                default:
                    return false;
            }
            switch(in)
            {
                case MODULE_CODE::DIN_IN:       bit_offset += 0; break;
                case MODULE_CODE::MODEM_IN:     bit_offset += 1; break;
                case MODULE_CODE::KEYBOARD_IN:  bit_offset += 2; break;
                case MODULE_CODE::SCREEN_IN:    bit_offset += 3; break;
                default:
                    return false;
            }

            return m_context.m_modules_switch_status & (1<<bit_offset);
        }

        struct Context
        {
            // bit  output module        input module
            // 0 -  DIN             ->  DIN
            // 1 -  DIN             ->  MODEM           *NOT IMPLEMENTED*
            // 2 -  DIN             ->  KEYBOARD        *UNUSED*
            // 3 -  DIN             ->  SCREEN
            // 4 -  MODEM           ->  DIN             *NOT IMPLEMENTED*
            // 5 -  MODEM           ->  MODEM           *NOT IMPLEMENTED*
            // 6 -  MODEM           ->  KEYBOARD        *UNUSED*
            // 7 -  MODEM           ->  SCREEN
            // 8 -  KEYBOARD        ->  DIN
            // 9 -  KEYBOARD        ->  MODEM
            // 10 - KEYBOARD        ->  KEYBOARD
            // 11 - KEYBOARD        ->  SCREEN
            // 12 - SCREEN          ->  DIN             *UNUSED*
            // 13 - SCREEN          ->  MODEM           *UNUSED*
            // 14 - SCREEN          ->  KEYBOARD        *UNUSED*
            // 15 - SCREEN          ->  SCREEN
            std::uint16_t m_modules_switch_status = 0b1000111110001001;

            bool is_escaped_to_G2 = false;
            bool is_escaped_to_G1 = false;
            bool is_rolling_mode  = false;  //false: page mode / true: rolling mode
            struct
            {
                GLYPH_ATT_FCOLOR fcolor = GFC_WHITE;
                GLYPH_ATT_SIZE   size   = GS_NORMAL;
                bool             blink  = false;
                bool             negative = false;
                bool             disjoint = false;      // only for G1
            }current_char_attributes;
            struct
            {
                GLYPH_ATT_BCOLOR bcolor = GBC_BLACK;
                bool             underline = false;     // only for G0/G2
                bool             masked    = false;
                bool             applied   = true;     // true if attributes already applied
                void reset(){ *this = {}; }
            }waiting_zone_attributes;
            GLYPH_CODE last_glyph = GC_SPACE;
            CursorPos last_main_screen_cursor_pos;

            void reset(){ *this = Context(); }
            // resets escape, attributes, but not page/rolling mode
            void reset_article()
            {
                *this = Context();
            }
        }m_context;

        struct
        {
            bool stop_execution = false;
            bool execution_stopped = false;
            bool next_step = false;
        }m_debug_context;

        void __pull_data();
        void __send_data(const std::uint8_t* data, std::size_t size);
        void __pull_keyboard();
        void __process_inputs();

        void __process_switch(MODULE_CODE out, const std::uint8_t* data, std::size_t size);
        void __process_switch_modem(const std::uint8_t* data, std::size_t size);
        void __process_switch_screen(const std::uint8_t* data, std::size_t size);
        void __process_switch_din(const std::uint8_t* data, std::size_t size);


    public:
        Minitel();
        void reset();
        void update();
        void set_screen_control(mtlc_module_control_screen screen, void* ctx);
        void set_din_control(mtlc_module_control_DIN din, void* ctx);
        void set_keyboard_control(mtlc_module_control_keyboard keyboard, void* ctx);
        void set_callback_on_bell(void(*callback_on_bell)(void*), void* ctx) { m_callback_on_bell = callback_on_bell; m_bell_ctx = ctx; }

        enum class BAUDRATES : std::uint8_t
        {
            AUTO = 0x00,     // as fast as the server sends
            B300 = 0x01,
            B1200 = 0x02,
            B4800 = 0x03,
            B9600 = 0x04,     // from Minitel 2
        } m_baudrate = BAUDRATES::AUTO;

        inline void set_baudrate(BAUDRATES baudrate) { m_baudrate = baudrate; }
        inline BAUDRATES get_baudrate() const { return m_baudrate; }

        // debug controls
        inline void debug_stop_execution(bool b){ m_debug_context.stop_execution = b; if(!b) m_debug_context.execution_stopped=false;}
        inline bool debug_is_execution_stopped() const { return m_debug_context.stop_execution; }
        inline void debug_next_step(){ if(m_debug_context.stop_execution) m_debug_context.next_step = true; }


        // press a key (from __pull_keyboard or external call)
        void key_event(mtlc_KeyEvent ke);

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
        std::uint8_t __get_PRO2_status_byte();

        RESULT_FUNC __func_BELL(int& nb_of_byte_processed);
        RESULT_FUNC __func_BS(int& nb_of_byte_processed);
        RESULT_FUNC __func_HT(int& nb_of_byte_processed);
        RESULT_FUNC __func_LF(int& nb_of_byte_processed);
        RESULT_FUNC __func_VT(int& nb_of_byte_processed);
        RESULT_FUNC __func_FF(int& nb_of_byte_processed);
        RESULT_FUNC __func_CR(int& nb_of_byte_processed);
        RESULT_FUNC __func_SO(int& nb_of_byte_processed);
        RESULT_FUNC __func_SI(int& nb_of_byte_processed);
        RESULT_FUNC __func_CON(int& nb_of_byte_processed);
        RESULT_FUNC __func_REP(int& nb_of_byte_processed);
        RESULT_FUNC __func_COFF(int& nb_of_byte_processed);
        RESULT_FUNC __func_CAN(int& nb_of_byte_processed);
        RESULT_FUNC __func_SS2(int& nb_of_byte_processed);
        RESULT_FUNC __func_RS(int& nb_of_byte_processed);
        RESULT_FUNC __func_US(int& nb_of_byte_processed);
        RESULT_FUNC __func_write_char(int& nb_of_byte_processed);

        //==================================================================
        // Escaped Functionalities
        //==================================================================
        RESULT_FUNC __func_ESC(int& nb_of_byte_processed);
        // called by __func_ESC during ISO 6429 escaping
        RESULT_FUNC __func_CSI(int& nb_of_byte_processed);
        // clears from cursor included to end of screen
        RESULT_FUNC __func_CSI_J(int& nb_of_byte_processed);
        // same as __func_CSI_J
        RESULT_FUNC __func_CSI_0J(int& nb_of_byte_processed);
        // clears from screen's start to cursor included
        RESULT_FUNC __func_CSI_1J(int& nb_of_byte_processed);
        // clears all screen (cursor is not moved)
        RESULT_FUNC __func_CSI_2J(int& nb_of_byte_processed);
        // clears from cursor included to end of line
        RESULT_FUNC __func_CSI_K(int& nb_of_byte_processed);
        // same as __func_CSI_K
        RESULT_FUNC __func_CSI_0K(int& nb_of_byte_concered);
        // clears from current line's start to cursor (included)
        RESULT_FUNC __func_CSI_1K(int& nb_of_byte_concered);
        // clears all the current line
        RESULT_FUNC __func_CSI_2K(int& nb_of_byte_concered);
        // cursor goes up n-times, no looping
        RESULT_FUNC __func_CSI_A(int& nb_of_byte_processed);
        // cursor goes down n-times, no looping
        RESULT_FUNC __func_CSI_B(int& nb_of_byte_processed);
        // cursor goes right n-times, no looping
        RESULT_FUNC __func_CSI_C(int& nb_of_byte_processed);
        // cursor goes left n-times, no looping
        RESULT_FUNC __func_CSI_D(int& nb_of_byte_processed);
        // direct addressing for cursor
        RESULT_FUNC __func_CSI_H(int& nb_of_byte_processed);
        // inserts n lines from current cursor line
        RESULT_FUNC __func_CSI_L(int& nb_of_byte_processed);
        // deletes n lines from current cursor line
        RESULT_FUNC __func_CSI_M(int& nb_of_byte_processed);
        // deletes n chars starting on cursor included
        RESULT_FUNC __func_CSI_P(int& nb_of_byte_processed);
        // starts char-insertion mode
        RESULT_FUNC __func_CSI_h(int& nb_of_byte_processed);
        // ends char-insertion mode
        RESULT_FUNC __func_CSI_i(int& nb_of_byte_processed);
        // masks/unmasks screen
        RESULT_FUNC __func_set_mask_screen(int& nb_of_byte_processed);

        //==================================================================
        // Protocol
        //==================================================================
        // Sends current cursor pos
        RESULT_FUNC __func_PRO_send_cursor_pos(int& nb_of_byte_processed);
        RESULT_FUNC __func_PRO1(int& nb_of_byte_processed);
        RESULT_FUNC __func_PRO2(int& nb_of_byte_processed);
        RESULT_FUNC __func_PRO3(int& nb_of_byte_processed);
    };
}


#endif //MINITELVIEWER_MINITEL_H
