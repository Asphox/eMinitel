//
// Created by asphox on 1/8/24.
//

#ifndef MTLC_H
#define MTLC_H

#include "VideotexCodes.h"
#include "Glyph.h"

#ifndef __cplusplus
#include <stdint.h>
#else
#include <cstdint>
extern "C"
{
#endif //__cplusplus

// =====================================================================================================================
// Screen Module Interface
// =====================================================================================================================

// structure for OPS_SET_CURSOR_POS
typedef struct
{
    std::uint8_t col;
    std::uint8_t line;
}mtlc_op_screen_param_set_cursor_pos;
// structure for OPS_SET_GLYPH_AT parameter
typedef struct
{
    GLYPH_CODE gc;
    mtlc_op_screen_param_set_cursor_pos pos;
}mtlc_op_screen_param_set_glyph_at;

// screen control operations
enum MTLC_OP_SCREEN : uint8_t
{
    OPS_UPDATE,                     // param : unused
    OPS_CLEAR,                      // param (uint8_t)  : true => keep line 00
    OPS_SCROLL,                     // param (int8_t)   : lines offset (>0 : scroll-down / otherwise scroll-up)
    OPS_SET_HEIGHT,                 // param (uint16_t) : height in px (width forced with ration 320/250)
    OPS_SET_GLYPH_AT,               // param (*mtlc_op_screen_param_set_glyph_at)
    OPS_SET_CURSOR_POS,             // param (*mtlc_op_screen_param_set_cursor_pos)
    OPS_SET_CURSOR_VISIBLE,         // param (uint8_t)  : true => visible
    OPS_SET_MASK,                   // param (uint8_t)  : true => masked
};

// callback for screen control
typedef void (*mtlc_module_control_screen)(MTLC_OP_SCREEN op, intptr_t param, void* ctx);

// =====================================================================================================================
// Keyboard Module Interface
// =====================================================================================================================

enum MTLC_KEYCODE : uint8_t
{
    KC_NONE,
    KC_CONNECTION,
    KC_SUMMARY,
    KC_CANCELATION,
    KC_RETURN,
    KC_REPETITION,
    KC_GUIDE,
    KC_CORRECTION,
    KC_NEXT,
    KC_SEND,
    KC_ESC,
    KC_UARROW,
    KC_DARROW,
    KC_LARROW,
    KC_RARROW,
    KC_SPACE,
    KC_ENTER,
    KC_A,
    KC_B,
    KC_C,
    KC_D,
    KC_E,
    KC_F,
    KC_G,
    KC_H,
    KC_I,
    KC_J,
    KC_K,
    KC_L,
    KC_M,
    KC_N,
    KC_O,
    KC_P,
    KC_Q,
    KC_R,
    KC_S,
    KC_T,
    KC_U,
    KC_V,
    KC_W,
    KC_X,
    KC_Y,
    KC_Z,
    KC_0,
    KC_1,
    KC_2,
    KC_3,
    KC_4,
    KC_5,
    KC_6,
    KC_7,
    KC_8,
    KC_9,
    KC_AST,
    KC_NUM,
    KC_COMMA,
    KC_POINT,
    KC_APOS,
    KC_SEMI,
    KC_MINUS,
    KC_COLON,
    KC_QUEST
};

typedef struct
{
    MTLC_KEYCODE kc;
    uint8_t fnct: 1;
    uint8_t ctrl: 2;
    uint8_t lshft: 3;
    uint8_t rshft: 4;
}mtlc_KeyEvent;

// keyboard control operations
enum MTLC_OP_KEYBOARD : uint8_t
{
    OPK_PULL_KEY,                   // param (&KeyEvent) : pulled KeyEvent
};

// callback for keyboard control
typedef mtlc_KeyEvent (*mtlc_module_control_keyboard)(MTLC_OP_KEYBOARD op, intptr_t param, void* ctx);

// =====================================================================================================================
// DIN Module Interface
// =====================================================================================================================

typedef struct
{
    uint16_t size;
    const std::uint8_t* data;
}mtlc_op_din_param_send_data;

enum MTLC_OP_DIN : uint8_t
{
    OPD_PULL_DATA,      // param (const char**) pointer to data buffer / return (uint16_t) size
    OPD_SEND_DATA,      // param (&mtlc_op_din_param_send_data)
};

// callback for DIN control
typedef uint16_t (*mtlc_module_control_DIN)(MTLC_OP_DIN op, intptr_t param, void* ctx);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //MTLC_H
