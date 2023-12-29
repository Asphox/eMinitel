//
// Created by asphox on 12/27/23.
//

#ifndef GLYPH_H
#define GLYPH_H

#include <cstdint>

// =============================================================================
//      Enums for glyph specialization data
// =============================================================================

// Videotext charsets ( + 1 special charset for internal usage)
enum GLYPH_CHARSET : uint8_t
{
    G0,     // glyphs on G0
    G1,     // glyphs on G1 (mosaic)
    G2,     // glyphs on G2 (only mono-glyph !)
    SPEC,   // special glyphs (space + accentuated)
};

// All type of accent available in Videotext
enum GLYPH_ACCENT : uint8_t
{
    GCA_NONE        = 0,
    GCA_CIRC        = 0,
    GCA_GRAVE       = 1,
    GCA_ACUTE       = 2,
    GCA_DIAERESIS   = 3,
    GCA_CEDILLA     = 4,
};

// Background color attribute (color or grey)
enum GLYPH_ATT_BCOLOR : uint8_t
{
    GBC_BLACK,
    GBC_BLUE,
    GBC_RED,
    GBC_MAGENTA,
    GBC_GREEN,
    GBC_CYAN,
    GBC_YELLOW,
    GBC_WHITE,
};

// Foreground color attribut (color or grey)
enum GLYPH_ATT_FCOLOR : uint8_t
{
    GFC_WHITE,
    GFC_BLUE,
    GFC_RED,
    GFC_MAGENTA,
    GFC_GREEN,
    GFC_CYAN,
    GFC_YELLOW,
    GFC_BLACK,
};

// Glyph size attribut (only for G0 and G2)
enum GLYPH_ATT_SIZE : uint8_t
{
    GS_NORMAL,
    GS_DOUBLE_WIDTH,
    GS_DOUBLE_HEIGHT,
    GS_DOUBLE_SIZE,
};

// =============================================================================
//      Function for glyph manipulation
// =============================================================================

enum GLYPH_CODE : std::uint32_t;
constexpr GLYPH_CODE operator|(GLYPH_CODE gc, std::uint32_t v)
{
    return static_cast<GLYPH_CODE>(static_cast<std::uint32_t>(gc) | v);
}
constexpr GLYPH_CODE operator|(GLYPH_CODE gc, std::uint16_t v)
{
    return static_cast<GLYPH_CODE>(static_cast<std::uint32_t>(gc) | v);
}


// Makes a glyph code (more information in GLYPH_CODE declaration)
// charset      : charset of the glyph (G0/G1/G2/SPEC)
// vdtx_code    : corresponding Videotext code for this glyph
// font_code    : the codepoint of the glyph in the eMinitel font. If defaulted, same as vdtx_code.
constexpr GLYPH_CODE make_glyph(GLYPH_CHARSET charset, char vdtx_code, std::uint16_t font_code = UINT16_MAX)
{
    GLYPH_CODE glyph_code = static_cast<GLYPH_CODE>(((uint16_t)(charset) << 10) | (vdtx_code));
    if(font_code == UINT16_MAX)
        font_code = vdtx_code;
    glyph_code = glyph_code | (static_cast<std::uint32_t>(font_code & 0xFF)<<12);
    return glyph_code;
}

// Makes an accentuated variation of a glyph
// gc   : base glyph
// font_code    : the codepoint of the glyph in the eMinitel font
// a            : type of accentuation
constexpr GLYPH_CODE make_glyph_accent(GLYPH_CODE gc, std::uint16_t font_code, GLYPH_ACCENT a)
{
    // clear charset bits
    char c = (char)(gc & 0x7F);
    GLYPH_CODE glyph_code_accent = make_glyph(SPEC, c, font_code);
    glyph_code_accent = glyph_code_accent | static_cast<std::uint32_t>(a<<7);
    return glyph_code_accent;
}

// Returns the videotext code of a glyph
// gc           : glyph code
constexpr char get_glyph_vdtx_code(GLYPH_CODE gc)
{
    return (gc & 0x7F);
}

// Returns the font code of a glyph
// gc           : glyph code
constexpr std::uint32_t get_glyph_font_code(GLYPH_CODE gc)
{
    return (gc>>12) & 0xFF;
}

// Returns the binary index for a mosaic glyph (from 0b000000 to 0b111111)
// gc           : glyph code
constexpr std::uint8_t get_glyph_mosaic_binary(GLYPH_CODE gc)
{
    std::uint8_t code = get_glyph_vdtx_code(gc);
    if(code == 0x5F)
        code = 0b111111;
    else
    {
        code -= 0x20;
        if(code >= 0x40)
            code -= 0x20;
    }
    return code;
}

// Returns the videotext charset of a glyph
// gc           : glyph code
constexpr GLYPH_CHARSET get_glyph_charset(GLYPH_CODE gc)
{
    GLYPH_CHARSET cs = static_cast<GLYPH_CHARSET>((gc>>10) & 3);
    return cs;
}

// Returns the combination of charset+accent+code in a incomplete GLYPHE_CODE
constexpr GLYPH_CODE get_glyph_charset_accent_code(GLYPH_CODE gc)
{
    return static_cast<GLYPH_CODE>(static_cast<std::uint32_t>(gc) & 0xFFF);
}

// Sets the background color of a glyph
// gc           : glyph code
// c            : background color
constexpr GLYPH_CODE set_glyph_att_bcolor(GLYPH_CODE gc, GLYPH_ATT_BCOLOR c)
{
    gc = gc | static_cast<std::uint32_t>(c<<20);
    return gc;
}

// Returns the background color of a glyph
// gc           : glyph code
constexpr GLYPH_ATT_BCOLOR get_glyph_att_bcolor(GLYPH_CODE gc)
{
    return static_cast<GLYPH_ATT_BCOLOR>(gc>>20 & 0b111);
}

// Sets the foreground color of a glyph
// gc           : glyph code
// c            : foreground color
constexpr GLYPH_CODE set_glyph_att_fcolor(GLYPH_CODE gc, GLYPH_ATT_FCOLOR c)
{
    gc = gc | static_cast<std::uint32_t>(c<<23);
    return gc;
}

// Returns the foreground color of a glyph
// gc           : glyph code
constexpr GLYPH_ATT_FCOLOR get_glyph_att_fcolor(GLYPH_CODE gc)
{
    return static_cast<GLYPH_ATT_FCOLOR>(gc>>23 & 0b111);
}

// Sets both background and foreground color of a glyph
// gc           : glyph code
// fc           : foreground color
// bc           : background color
constexpr GLYPH_CODE set_glyph_att_color(GLYPH_CODE gc, GLYPH_ATT_FCOLOR fc, GLYPH_ATT_BCOLOR bc)
{
    gc = set_glyph_att_fcolor(gc, fc);
    gc = set_glyph_att_bcolor(gc, bc);
    return gc;
}

// Sets the glyph's size attribute
// gc           : glyph code
// s            : size modifier
constexpr GLYPH_CODE set_glyph_att_size(GLYPH_CODE gc, GLYPH_ATT_SIZE s)
{
    return static_cast<GLYPH_CODE>((gc & ~(3<<26)) | ((s & 3)<<26));
}

// Returns the glyph's size attribute
// gc           : glyph code
constexpr GLYPH_ATT_SIZE get_glyph_att_size(GLYPH_CODE gc)
{
    return static_cast<GLYPH_ATT_SIZE>(gc>>26 & 3);
}

// Modifies the glyph's underline attribute (underline for G0 and G2, disjointed for G1)
// gc           : glyph code
// underline    : enable/disable
constexpr GLYPH_CODE set_glyph_att_underline(GLYPH_CODE gc, bool underline)
{
    return static_cast<GLYPH_CODE>((gc & ~(1<<28)) | (underline<<28));
}

// Returns the glyph's underline attribute
// gc           : glyph code
constexpr bool get_glyph_att_underline(GLYPH_CODE gc)
{
    return gc & (1<<28);
}

// Modifies the glyph's negative attribute
// gc           : glyph code
// negative    : enable/disable
constexpr GLYPH_CODE set_glyph_att_negative(GLYPH_CODE gc, bool negative)
{
    return static_cast<GLYPH_CODE>((gc & ~(1<<29)) | (negative<<29));
}

// Returns the glyph's negative attribute
// gc           : glyph code
constexpr bool get_glyph_att_negative(GLYPH_CODE gc)
{
    return gc & (1<<29);
}

// Modifies the glyph's blink attribute
// gc           : glyph code
// blink    : enable/disable
constexpr GLYPH_CODE set_glyph_att_blink(GLYPH_CODE gc, bool blink)
{
    return static_cast<GLYPH_CODE>((gc & ~(1<<30)) | (blink<<30));
}

// Returns the glyph's blink attribute
// gc           : glyph code
constexpr bool get_glyph_att_blink(GLYPH_CODE gc)
{
    return gc & (1<<30);
}

// =========================================================================================
//          Glyph description
//==========================================================================================
// All possible glyphs (+ all attributes combination) drawable on the Minitel are encoded over 32 bits
// based on the following repartition :
//
// bits       24           16           8            0
//     0BNL SSFF    FBBB UUUU   UUUU GGAA    AVVV VVVV
//
// V [0-6] :    videotext code minus 0x20 (G0/G1 value or G2 only value for mono-glyph)
// A [7-9] :    type of accentuation on a glyph, only if G is 11, nothing otherwise
//              - 000 : Circumflex accent
//              - 001 : Grave accent
//              - 010 : Acute accent
//              - 011 : Diaeresis accent
//              - 100 : Cedilla
//              - 101 : Reserved
//              - 110 : Reserved
//              - 111 : Reserved
// G [10-11] :   charset of the glyph
//              - 00 : G0
//              - 01 : G1
//              - 10 : G2 only for mono-glyph
//              - 11 : Special charset for accentuated glyphs and special chars (space)
// U [12-19] :  corresponding code-in-font value (on 8 bits)
//  ===== ATTRIBUTS ====
// B [20-22] :  background color
//              - 000 black
//              - 001 blue / grey 40%
//              - 010 red / grey 50 %
//              - 011 magenta / grey 60%
//              - 100 green / grey 70%
//              - 101 cyan / grey 80%
//              - 110 yellow / grey 90%
//              - 111 white
// F [23-25] :  foreground color
//              - 000 white
//              - 001 blue / grey 40%
//              - 010 red / grey 50 %
//              - 011 magenta / grey 60%
//              - 100 green / grey 70%
//              - 101 cyan / grey 80%
//              - 110 yellow / grey 90%
//              - 111 black
// S [26-27] : size
//              - 00 normal (1x1)
//              - 01 double width (2x1)
//              - 10 double height (1x2)
//              - 11 double size (2x2)
// L [28] :     underline (disjoined for G2)
//              - 0 OFF / (joined)
//              - 1 ON / (disjoined)
// N [29] :     negative colors
//              - 0 OFF
//              - 1 ON
// B [30] :     blink
//              - 0 OFF
//              - 1 ON
enum GLYPH_CODE : uint32_t
{
    GC_EXCL                 = make_glyph(G0, '!'),
    GC_QUOT                 = make_glyph(G0, '"'),
    GC_NUM                  = make_glyph(G0, '#'),
    GC_DOLLAR               = make_glyph(G0, '$'),
    GC_PERCNT               = make_glyph(G0, '%'),
    GC_AMP                  = make_glyph(G0, '&'),
    GC_APOS                 = make_glyph(G0, '\''),
    GC_LPAR                 = make_glyph(G0, '('),
    GC_RPAR                 = make_glyph(G0, ')'),
    GC_AST                  = make_glyph(G0, '*'),
    GC_PLUS                 = make_glyph(G0, '+'),
    GC_COMMA                = make_glyph(G0, ','),
    GC_MINUS                = make_glyph(G0, '-'),
    GC_PERIOD               = make_glyph(G0, '.'),
    GC_SLASH                = make_glyph(G0, '/'),
    GC_0                    = make_glyph(G0, '0'),
    GC_1                    = make_glyph(G0, '1'),
    GC_2                    = make_glyph(G0, '2'),
    GC_3                    = make_glyph(G0, '3'),
    GC_4                    = make_glyph(G0, '4'),
    GC_5                    = make_glyph(G0, '5'),
    GC_6                    = make_glyph(G0, '6'),
    GC_7                    = make_glyph(G0, '7'),
    GC_8                    = make_glyph(G0, '8'),
    GC_9                    = make_glyph(G0, '9'),
    GC_COLON                = make_glyph(G0, ':'),
    GC_SEMI                 = make_glyph(G0, ';'),
    GC_LT                   = make_glyph(G0, '<'),
    GC_EQUALS               = make_glyph(G0, '='),
    GC_GT                   = make_glyph(G0, '>'),
    GC_QUEST                = make_glyph(G0, '?'),
    GC_AT                   = make_glyph(G0, '@'),
    GC_A                    = make_glyph(G0, 'A'),
    GC_A_CIRC               = make_glyph_accent(static_cast<GLYPH_CODE>(GC_A), 'A', GCA_CIRC),        // only on VGP2
    GC_B                    = make_glyph(G0, 'B'),
    GC_C                    = make_glyph(G0, 'C'),
    GC_C_CEDILLA            = make_glyph_accent(static_cast<GLYPH_CODE>(GC_C), 'C', GCA_CEDILLA),     // only on VGP2
    GC_D                    = make_glyph(G0, 'D'),
    GC_E                    = make_glyph(G0, 'E'),
    GC_E_GRAVE              = make_glyph_accent(static_cast<GLYPH_CODE>(GC_E), 'E', GCA_GRAVE),       // only on VGP2
    GC_E_ACUTE              = make_glyph_accent(static_cast<GLYPH_CODE>(GC_E), 'E', GCA_ACUTE),       // only on VGP2
    GC_F                    = make_glyph(G0, 'F'),
    GC_G                    = make_glyph(G0, 'G'),
    GC_H                    = make_glyph(G0, 'H'),
    GC_I                    = make_glyph(G0, 'I'),
    GC_J                    = make_glyph(G0, 'J'),
    GC_K                    = make_glyph(G0, 'K'),
    GC_L                    = make_glyph(G0, 'L'),
    GC_M                    = make_glyph(G0, 'M'),
    GC_N                    = make_glyph(G0, 'N'),
    GC_O                    = make_glyph(G0, 'O'),
    GC_P                    = make_glyph(G0, 'P'),
    GC_Q                    = make_glyph(G0, 'Q'),
    GC_R                    = make_glyph(G0, 'R'),
    GC_S                    = make_glyph(G0, 'S'),
    GC_T                    = make_glyph(G0, 'T'),
    GC_U                    = make_glyph(G0, 'U'),
    GC_V                    = make_glyph(G0, 'V'),
    GC_W                    = make_glyph(G0, 'W'),
    GC_X                    = make_glyph(G0, 'X'),
    GC_Y                    = make_glyph(G0, 'Y'),
    GC_Z                    = make_glyph(G0, 'Z'),
    GC_LSQB                 = make_glyph(G0, '['),
    GC_BSLASH               = make_glyph(G0, '\\'),
    GC_RSQB                 = make_glyph(G0, L']'),
    GC_UPARROW              = make_glyph(G0, 0x5E, 0x5E),
    GC_UNDRSCR              = make_glyph(G0, '_'),
    GC_MIDLSCR              = make_glyph(G0, 0x60, 0x85),
    GC_a                    = make_glyph(G0, 'a'),
    GC_a_CIRC               = make_glyph_accent(static_cast<GLYPH_CODE>(GC_a), 0xE2, GCA_CIRC),
    GC_a_GRAVE              = make_glyph_accent(static_cast<GLYPH_CODE>(GC_a), 0xE0, GCA_GRAVE),
    GC_a_DIAERESIS          = make_glyph_accent(static_cast<GLYPH_CODE>(GC_a), 0xE4, GCA_DIAERESIS),   // only on VGP5
    GC_b                    = make_glyph(G0, 'b'),
    GC_c                    = make_glyph(G0, 'c'),
    GC_c_CEDILLA            = make_glyph_accent(static_cast<GLYPH_CODE>(GC_c), 0xE7, GCA_CEDILLA),
    GC_d                    = make_glyph(G0, 'd'),
    GC_e                    = make_glyph(G0, 'e'),
    GC_e_CIRC               = make_glyph_accent(static_cast<GLYPH_CODE>(GC_e), 0xEA, GCA_CIRC),
    GC_e_GRAVE              = make_glyph_accent(static_cast<GLYPH_CODE>(GC_e), 0xE8, GCA_GRAVE),
    GC_e_ACUTE              = make_glyph_accent(static_cast<GLYPH_CODE>(GC_e), 0xE9, GCA_ACUTE),
    GC_e_DIAERESIS          = make_glyph_accent(static_cast<GLYPH_CODE>(GC_e), 0xEB, GCA_DIAERESIS),
    GC_f                    = make_glyph(G0, 'f'),
    GC_g                    = make_glyph(G0, 'g'),
    GC_h                    = make_glyph(G0, 'h'),
    GC_i                    = make_glyph(G0, 'i'),
    GC_i_CIRC               = make_glyph_accent(static_cast<GLYPH_CODE>(GC_i), 0xEE, GCA_CIRC),
    GC_i_DIAERESIS          = make_glyph_accent(static_cast<GLYPH_CODE>(GC_i), 0xEF, GCA_DIAERESIS),
    GC_j                    = make_glyph(G0, 'j'),
    GC_k                    = make_glyph(G0, 'k'),
    GC_l                    = make_glyph(G0, 'l'),
    GC_m                    = make_glyph(G0, 'm'),
    GC_n                    = make_glyph(G0, 'n'),
    GC_o                    = make_glyph(G0, 'o'),
    GC_o_CIRC               = make_glyph_accent(static_cast<GLYPH_CODE>(GC_o), 0xF4, GCA_CIRC),
    GC_o_DIAERESIS          = make_glyph_accent(static_cast<GLYPH_CODE>(GC_o), 0xF6, GCA_DIAERESIS),   // only on VGP5
    GC_p                    = make_glyph(G0, 'p'),
    GC_q                    = make_glyph(G0, 'q'),
    GC_r                    = make_glyph(G0, 'r'),
    GC_s                    = make_glyph(G0, 's'),
    GC_t                    = make_glyph(G0, 't'),
    GC_u                    = make_glyph(G0, 'u'),
    GC_u_CIRC               = make_glyph_accent(static_cast<GLYPH_CODE>(GC_u), 0xFB, GCA_CIRC),
    GC_u_GRAVE              = make_glyph_accent(static_cast<GLYPH_CODE>(GC_u), 0xF9, GCA_GRAVE),
    GC_u_DIAERESIS          = make_glyph_accent(static_cast<GLYPH_CODE>(GC_u), 0xFC, GCA_DIAERESIS),   // only on VGP5
    GC_v                    = make_glyph(G0, 'v'),
    GC_w                    = make_glyph(G0, 'w'),
    GC_x                    = make_glyph(G0, 'x'),
    GC_y                    = make_glyph(G0, 'y'),
    GC_z                    = make_glyph(G0, 'z'),
    GC_LVERBAR              = make_glyph(G0, 0x7B, 0x83),
    GC_VERBAR               = make_glyph(G0, '|'),
    GC_RVERBAR              = make_glyph(G0, 0x7D, 0x84),
    GC_UPRSCR               = make_glyph(G0, 0x7E, 0x86),

    GC_POUND                = make_glyph(G2, 0x23, 0xA3),
    GC_DOLLAR_G2            = make_glyph(G2, '$'),      // $ is in both G0 and G2 at the same position ...
    GC_NUM_G2               = make_glyph(G2, 0x26, '#'),     // # is in both G0 and G2
    GC_SECT                 = make_glyph(G2, 0x27, 0xA7),
    GC_LARROW               = make_glyph(G2, 0x2C, 0x81),
    GC_UPARROW_G2           = make_glyph(G2, 0x2D, 0x5E),     // uparrow is in both G0 and G2
    GC_RARROW               = make_glyph(G2, 0x2E, 0x82),
    GC_DARROW               = make_glyph(G2, 0x2F, 0x80),
    GC_DEG                  = make_glyph(G2, 0x30, 0xB0),
    GC_PLUSMN               = make_glyph(G2, 0x31, 0xB1),
    GC_DIV                  = make_glyph(G2, 0x38, 0xF7),
    GC_FRAC14               = make_glyph(G2, 0x3C, 0xBC),
    GC_FRAC12               = make_glyph(G2, 0x3D, 0xBD),
    GC_FRAC34               = make_glyph(G2, 0x3E, 0xBE),
    GC_LIG_OE               = make_glyph(G2, 0x6A, 0x87),
    GC_LIG_oe               = make_glyph(G2, 0x7A, 0x88),
    GC_ESZETT               = make_glyph(G2, 0x7B, 0xDF),

    GC_SPACE                = make_glyph(SPEC, ' '),

    GC_MOSAIC_1             = make_glyph(G1, 0x21),
    GC_MOSAIC_2             = make_glyph(G1, 0x22),
    GC_MOSAIC_12            = make_glyph(G1, 0x23),
    GC_MOSAIC_3             = make_glyph(G1, 0x24),
    GC_MOSAIC_13            = make_glyph(G1, 0x25),
    GC_MOSAIC_23            = make_glyph(G1, 0x26),
    GC_MOSAIC_123           = make_glyph(G1, 0x27),
    GC_MOSAIC_4             = make_glyph(G1, 0x28),
    GC_MOSAIC_14            = make_glyph(G1, 0x29),
    GC_MOSAIC_24            = make_glyph(G1, 0x2A),
    GC_MOSAIC_124           = make_glyph(G1, 0x2B),
    GC_MOSAIC_34            = make_glyph(G1, 0x2C),
    GC_MOSAIC_134           = make_glyph(G1, 0x2D),
    GC_MOSAIC_234           = make_glyph(G1, 0x2E),
    GC_MOSAIC_1234          = make_glyph(G1, 0x2F),
    GC_MOSAIC_5             = make_glyph(G1, 0x30),
    GC_MOSAIC_15            = make_glyph(G1, 0x31),
    GC_MOSAIC_25            = make_glyph(G1, 0x32),
    GC_MOSAIC_125           = make_glyph(G1, 0x33),
    GC_MOSAIC_35            = make_glyph(G1, 0x34),
    GC_MOSAIC_135           = make_glyph(G1, 0x35),
    GC_MOSAIC_235           = make_glyph(G1, 0x36),
    GC_MOSAIC_1235          = make_glyph(G1, 0x37),
    GC_MOSAIC_45            = make_glyph(G1, 0x38),
    GC_MOSAIC_145           = make_glyph(G1, 0x39),
    GC_MOSAIC_245           = make_glyph(G1, 0x3A),
    GC_MOSAIC_1245          = make_glyph(G1, 0x3B),
    GC_MOSAIC_345           = make_glyph(G1, 0x3C),
    GC_MOSAIC_1345          = make_glyph(G1, 0x3D),
    GC_MOSAIC_2345          = make_glyph(G1, 0x3E),
    GC_MOSAIC_12345         = make_glyph(G1, 0x3F),
    GC_MOSAIC_6             = make_glyph(G1, 0x60),
    GC_MOSAIC_16            = make_glyph(G1, 0x61),
    GC_MOSAIC_26            = make_glyph(G1, 0x62),
    GC_MOSAIC_126           = make_glyph(G1, 0x63),
    GC_MOSAIC_36            = make_glyph(G1, 0x64),
    GC_MOSAIC_136           = make_glyph(G1, 0x65),
    GC_MOSAIC_236           = make_glyph(G1, 0x66),
    GC_MOSAIC_1236          = make_glyph(G1, 0x67),
    GC_MOSAIC_46            = make_glyph(G1, 0x68),
    GC_MOSAIC_146           = make_glyph(G1, 0x69),
    GC_MOSAIC_246           = make_glyph(G1, 0x6A),
    GC_MOSAIC_1246          = make_glyph(G1, 0x6B),
    GC_MOSAIC_346           = make_glyph(G1, 0x6C),
    GC_MOSAIC_1346          = make_glyph(G1, 0x6D),
    GC_MOSAIC_2346          = make_glyph(G1, 0x6E),
    GC_MOSAIC_12346         = make_glyph(G1, 0x6F),
    GC_MOSAIC_56            = make_glyph(G1, 0x70),
    GC_MOSAIC_156           = make_glyph(G1, 0x71),
    GC_MOSAIC_256           = make_glyph(G1, 0x72),
    GC_MOSAIC_1256          = make_glyph(G1, 0x73),
    GC_MOSAIC_356           = make_glyph(G1, 0x74),
    GC_MOSAIC_1356          = make_glyph(G1, 0x75),
    GC_MOSAIC_2356          = make_glyph(G1, 0x76),
    GC_MOSAIC_12356         = make_glyph(G1, 0x77),
    GC_MOSAIC_456           = make_glyph(G1, 0x78),
    GC_MOSAIC_1456          = make_glyph(G1, 0x79),
    GC_MOSAIC_2456          = make_glyph(G1, 0x7A),
    GC_MOSAIC_12456         = make_glyph(G1, 0x7B),
    GC_MOSAIC_3456          = make_glyph(G1, 0x7C),
    GC_MOSAIC_13456         = make_glyph(G1, 0x7D),
    GC_MOSAIC_23456         = make_glyph(G1, 0x7E),
    GC_MOSAIC_123456        = make_glyph(G1, 0x5F),
};

#define __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(gc) case get_glyph_charset_accent_code(gc) : return gc
// Converts a triplet CHARSET/CODE/ACCENT into a valid GLYPH code
// Returns GC_UNDRSCR if no GLYPH is found
constexpr GLYPH_CODE get_glyph_from_charset_code_accent(GLYPH_CHARSET cs, char code, GLYPH_ACCENT a)
{
    GLYPH_CODE tmp_glyph_code = make_glyph(cs, code, 0);
    tmp_glyph_code = tmp_glyph_code | static_cast<std::uint32_t>(a<<7);
    switch(tmp_glyph_code)
    {
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_EXCL         );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_QUOT         );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_NUM          );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_DOLLAR       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_PERCNT       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_AMP          );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_APOS         );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_LPAR         );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_RPAR         );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_AST          );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_PLUS         );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_COMMA        );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MINUS        );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_PERIOD       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_SLASH        );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_0            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_1            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_2            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_3            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_4            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_5            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_6            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_7            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_8            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_9            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_COLON        );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_SEMI         );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_LT           );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_EQUALS       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_GT           );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_QUEST        );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_AT           );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_A            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_A_CIRC       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_B            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_C            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_C_CEDILLA    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_D            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_E            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_E_GRAVE      );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_E_ACUTE      );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_F            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_G            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_H            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_I            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_J            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_K            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_L            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_M            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_N            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_O            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_P            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_Q            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_R            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_S            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_T            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_U            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_V            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_W            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_X            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_Y            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_Z            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_LSQB         );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_BSLASH       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_RSQB         );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_UPARROW      );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_UNDRSCR      );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MIDLSCR      );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_a            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_a_CIRC       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_a_GRAVE      );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_a_DIAERESIS  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_b            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_c            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_c_CEDILLA    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_d            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_e            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_e_CIRC       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_e_GRAVE      );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_e_ACUTE      );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_e_DIAERESIS  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_f            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_g            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_h            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_i            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_i_CIRC       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_i_DIAERESIS  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_j            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_k            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_l            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_m            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_n            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_o            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_o_CIRC       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_o_DIAERESIS  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_p            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_q            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_r            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_s            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_t            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_u            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_u_CIRC       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_u_GRAVE      );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_u_DIAERESIS  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_v            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_w            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_x            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_y            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_z            );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_LVERBAR      );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_VERBAR       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_RVERBAR      );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_UPRSCR       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_POUND        );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_DOLLAR_G2    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_NUM_G2       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_SECT         );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_LARROW       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_UPARROW_G2   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_RARROW       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_DARROW       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_DEG          );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_PLUSMN       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_DIV          );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_FRAC14       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_FRAC12       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_FRAC34       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_LIG_OE       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_LIG_oe       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_ESZETT       );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_SPACE        );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_1     );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_2     );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_12    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_3     );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_13    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_23    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_123   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_4     );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_14    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_24    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_124   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_34    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_134   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_234   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_1234  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_5     );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_15    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_25    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_125   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_35    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_135   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_235   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_1235  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_45    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_145   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_245   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_1245  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_345   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_1345  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_2345  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_12345 );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_6     );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_16    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_26    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_126   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_36    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_136   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_236   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_1236  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_46    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_146   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_246   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_1246  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_346   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_1346  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_2346  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_12346 );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_56    );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_156   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_256   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_1256  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_356   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_1356  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_2356  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_12356 );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_456   );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_1456  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_2456  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_12456 );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_3456  );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_13456 );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_23456 );
        __GLYPH_CASE_FROM_CHARSET_CODE_ACCENT__(GC_MOSAIC_123456);
        default:
            return GC_UNDRSCR;
    }
}

#endif //GLYPH_H
