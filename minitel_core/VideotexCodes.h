//
// Created by asphox on 12/26/23.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <cstdint>
#include <cstddef>
enum VDTX_CODE : std::uint32_t;
constexpr VDTX_CODE make_vdtx_code(std::uint8_t b1, std::uint8_t b2 = 0, std::uint8_t b3 = 0)
{
    VDTX_CODE code = static_cast<VDTX_CODE>(0);
    if(b3)
        code = static_cast<VDTX_CODE>((b3<<16) | (b2<<8) | b1);
    else if(b2)
        code =  static_cast<VDTX_CODE>((b2<<8) | b1);
    else
        code =  static_cast<VDTX_CODE>(b1);
    return code;
}
constexpr std::size_t get_vdtx_code_size(VDTX_CODE code)
{
    return 1 + ((code & 0xFF00) ? 1 : 0) + ((code & 0xFF0000) ? 1 : 0);
}
enum VDTX_CODE : std::uint32_t
{
    __VC_CONTROL_MIN    ,
    VC_NUL      = make_vdtx_code(0x00),
    VC_SOH      = make_vdtx_code(0x01),
    VC_EOT      = make_vdtx_code(0x04),
    VC_ENQ      = make_vdtx_code(0x05),
    VC_BEL      = make_vdtx_code(0x07),
    VC_BS       = make_vdtx_code(0x08),
    VC_HT       = make_vdtx_code(0x09),
    VC_LF       = make_vdtx_code(0x0A),
    VC_VT       = make_vdtx_code(0x0B),
    VC_FF       = make_vdtx_code(0x0C),
    VC_CR       = make_vdtx_code(0x0D),
    VC_SO       = make_vdtx_code(0x0E),
    VC_SI       = make_vdtx_code(0x0F),
    VC_DLE      = make_vdtx_code(0x10),
    VC_CON      = make_vdtx_code(0x11),
    VC_REP      = make_vdtx_code(0x12),
    VC_SEP      = make_vdtx_code(0x13),
    VC_COFF     = make_vdtx_code(0x14),
    VC_NACK     = make_vdtx_code(0x15),
    VC_SYN      = make_vdtx_code(0x16),
    VC_CAN      = make_vdtx_code(0x18),
    VC_SS2      = make_vdtx_code(0x19),
    VC_SUB      = make_vdtx_code(0x1A),
    VC_ESC      = make_vdtx_code(0x1B),
    VC_SS3      = make_vdtx_code(0x1D),
    VC_RS       = make_vdtx_code(0x1E),
    VC_US       = make_vdtx_code(0x1F),
    __VC_CONTROL_MAX    = VC_US,
    __VC_CHAR_MIN   = make_vdtx_code(0x20),
    VC_SPACE    = __VC_CHAR_MIN,
    VC_EXCL     = make_vdtx_code(0x21),
    VC_QUOT     = make_vdtx_code(0x22),
    VC_NUM      = make_vdtx_code(0x23),
    VC_DOLLAR   = make_vdtx_code(0x24),
    VC_PERCNT   = make_vdtx_code(0x25),
    VC_AMP      = make_vdtx_code(0x26),
    VC_APOS     = make_vdtx_code(0x27),
    VC_LPAR     = make_vdtx_code(0x28),
    VC_RPAR     = make_vdtx_code(0x29),
    VC_AST      = make_vdtx_code(0x2A),
    VC_PLUS     = make_vdtx_code(0x2B),
    VC_COMMA    = make_vdtx_code(0x2C),
    VC_MINUS    = make_vdtx_code(0x2D),
    VC_PERIOD   = make_vdtx_code(0x2E),
    VC_SLASH    = make_vdtx_code(0x2F),
    VC_0        = make_vdtx_code(0x30),
    VC_1        = make_vdtx_code(0x31),
    VC_2        = make_vdtx_code(0x32),
    VC_3        = make_vdtx_code(0x33),
    VC_4        = make_vdtx_code(0x34),
    VC_5        = make_vdtx_code(0x35),
    VC_6        = make_vdtx_code(0x36),
    VC_7        = make_vdtx_code(0x37),
    VC_8        = make_vdtx_code(0x38),
    VC_9        = make_vdtx_code(0x39),
    VC_COLON    = make_vdtx_code(0x3A),
    VC_SEMI     = make_vdtx_code(0x3B),
    VC_LT       = make_vdtx_code(0x3C),
    VC_EQUALS   = make_vdtx_code(0x3D),
    VC_GT       = make_vdtx_code(0x3E),
    VC_QUEST    = make_vdtx_code(0x3F),
    VC_AT       = make_vdtx_code(0x40),
    VC_A        = make_vdtx_code(0x41),
    VC_B        = make_vdtx_code(0x42),
    VC_C        = make_vdtx_code(0x43),
    VC_D        = make_vdtx_code(0x44),
    VC_E        = make_vdtx_code(0x45),
    VC_F        = make_vdtx_code(0x46),
    VC_G        = make_vdtx_code(0x47),
    VC_H        = make_vdtx_code(0x48),
    VC_I        = make_vdtx_code(0x49),
    VC_J        = make_vdtx_code(0x4A),
    VC_K        = make_vdtx_code(0x4B),
    VC_L        = make_vdtx_code(0x4C),
    VC_M        = make_vdtx_code(0x4D),
    VC_N        = make_vdtx_code(0x4E),
    VC_O        = make_vdtx_code(0x4F),
    VC_P        = make_vdtx_code(0x50),
    VC_Q        = make_vdtx_code(0x51),
    VC_R        = make_vdtx_code(0x52),
    VC_S        = make_vdtx_code(0x53),
    VC_T        = make_vdtx_code(0x54),
    VC_U        = make_vdtx_code(0x55),
    VC_V        = make_vdtx_code(0x56),
    VC_W        = make_vdtx_code(0x57),
    VC_X        = make_vdtx_code(0x58),
    VC_Y        = make_vdtx_code(0x59),
    VC_Z        = make_vdtx_code(0x5A),
    VC_LSQB     = make_vdtx_code(0x5B),
    VC_BSLASH   = make_vdtx_code(0x5C),
    VC_RSQB     = make_vdtx_code(0x5D),
    VC_UPARROW  = make_vdtx_code(0x5E),
    VC_UNDRSCR  = make_vdtx_code(0x5F),
    VC_MIDLSCR  = make_vdtx_code(0x60),
    VC_a        = make_vdtx_code(0x61),
    VC_b        = make_vdtx_code(0x62),
    VC_c        = make_vdtx_code(0x63),
    VC_d        = make_vdtx_code(0x64),
    VC_e        = make_vdtx_code(0x65),
    VC_f        = make_vdtx_code(0x66),
    VC_g        = make_vdtx_code(0x67),
    VC_h        = make_vdtx_code(0x68),
    VC_i        = make_vdtx_code(0x69),
    VC_j        = make_vdtx_code(0x6A),
    VC_k        = make_vdtx_code(0x6B),
    VC_l        = make_vdtx_code(0x6C),
    VC_m        = make_vdtx_code(0x6D),
    VC_n        = make_vdtx_code(0x6E),
    VC_o        = make_vdtx_code(0x6F),
    VC_p        = make_vdtx_code(0x70),
    VC_q        = make_vdtx_code(0x71),
    VC_r        = make_vdtx_code(0x72),
    VC_s        = make_vdtx_code(0x73),
    VC_t        = make_vdtx_code(0x74),
    VC_u        = make_vdtx_code(0x75),
    VC_v        = make_vdtx_code(0x76),
    VC_w        = make_vdtx_code(0x77),
    VC_x        = make_vdtx_code(0x78),
    VC_y        = make_vdtx_code(0x79),
    VC_z        = make_vdtx_code(0x7A),
    VC_LVERAR   = make_vdtx_code(0x7B),
    VC_VERBAR   = make_vdtx_code(0x7C),
    VC_RVERBAR  = make_vdtx_code(0x7D),
    VC_UPRSCR   = make_vdtx_code(0x7E),

    VC_SEND     = make_vdtx_code(0x13, 0x41),
    VC_PREVIOUS = make_vdtx_code(0x13, 0x42),
    VC_REPEAT   = make_vdtx_code(0x13, 0x43),
    VC_GUIDE    = make_vdtx_code(0x13, 0x44),
    VC_CANCEL   = make_vdtx_code(0x13, 0x45),
    VC_SUMMARY  = make_vdtx_code(0x13, 0x46),
    VC_CORRECT  = make_vdtx_code(0x13, 0x47),
    VC_NEXT     = make_vdtx_code(0x13, 0x48),

    VC_POUND    = make_vdtx_code(0x19, 0x23),
    VC_SECT     = make_vdtx_code(0x19, 0x27),
    VC_LARROW   = make_vdtx_code(0x19, 0x2C),
    VC_RARROW   = make_vdtx_code(0x19, 0x2E),
    VC_DARROW   = make_vdtx_code(0x19, 0x2F),
    VC_DEG      = make_vdtx_code(0x19, 0x30),
    VC_PLUSMN   = make_vdtx_code(0x19, 0x31),
    VC_DIV      = make_vdtx_code(0x19, 0x38),
    VC_GRAVE    = make_vdtx_code(0x19, 0x41),
    VC_ACUTE    = make_vdtx_code(0x19, 0x42),
    VC_CIRC     = make_vdtx_code(0x19, 0x43),
    VC_DIAERESIS= make_vdtx_code(0x19, 0x48),
    VC_c_cedilla= make_vdtx_code(0x19, 0x4B, 0x63),
    VC_LIG_OE   = make_vdtx_code(0x19, 0x7A),
    VC_ESZETT   = make_vdtx_code(0x19, 0x7B),
    __VC_CHAR_MAX = VC_UPRSCR,
};

constexpr bool is_vdtx_code_diacritic(VDTX_CODE code)
{
    switch (code)
    {
    case VC_GRAVE:
    case VC_ACUTE:
    case VC_CIRC:
    case VC_DIAERESIS:
        return true;
    default:
        return false;
    }
}

#endif //CONSTANTS_H
