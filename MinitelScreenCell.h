//
// Created by asphox on 11/30/23.
//

#ifndef MINITELVIEWER_MINITELSCREENCELL_H
#define MINITELVIEWER_MINITELSCREENCELL_H

#include <SFML/Graphics.hpp>

namespace mvw
{
    enum CHARSET : uint8_t
    {
        G0,     // glyphs on G0
        G1,     // glyphs on G1 (mosaic)
        G2,     // glyphs on G2 (only mono-glyph !)
        SPEC,   // special glyphs (space + accentuated)
    };

    enum GCHAR_ACCENT : uint8_t
    {
        GCA_CIRC        = 0,
        GCA_GRAVE       = 1,
        GCA_ACUTE       = 2,
        GCA_DIAERESIS   = 3,
        GCA_CEDILLA     = 4,
    };

    constexpr std::uint16_t make_GCHAR(CHARSET charset, char val)
    {
        return (((uint16_t)(charset) << 10) | val);
    }

    constexpr std::uint16_t make_GCHAR_accent(uint16_t gc, GCHAR_ACCENT a)
    {
        // clear charset bits
        char c = (char)(gc & 0xFF);
        std::uint16_t r = make_GCHAR(SPEC, c);
        r |= (a<<7);
        return r;
    }

    // <==== bits description of GCHAR ====>
    // bits     8            0
    //  0000 GGAA    ACCC CCCC
    // C [0-6] :    base value of the glyph (G0/G1 value or G2 only value for mono-glyph)
    // A [7-8] :    type of accentuation on a glyph, only if G is 11, nothing otherwise
    //              - 000 : Circumflex accent
    //              - 001 : Grave accent
    //              - 010 : Acute accent
    //              - 011 : Diaeresis accent
    //              - 100 : Cedilla
    //              - 101 : Reserved
    //              - 110 : Reserved
    //              - 111 : Reserved
    // G [9-10] :   charset of the glyph
    //              - 00 : G0
    //              - 01 : G1
    //              - 10 : G2 only for mono-glyph
    //              - 11 : Special charset for accentuated glyphs and special chars (space)
    enum GCHAR : uint16_t
    {
        GC_EXCL                 = make_GCHAR(G0, '!'),
        GC_QUOT                 = make_GCHAR(G0, '"'),
        GC_NUM                  = make_GCHAR(G0, '#'),
        GC_DOLLAR               = make_GCHAR(G0, '$'),
        GC_PERCNT               = make_GCHAR(G0, '%'),
        GC_AMP                  = make_GCHAR(G0, '&'),
        GC_APOS                 = make_GCHAR(G0, '\''),
        GC_LPAR                 = make_GCHAR(G0, '('),
        GC_RPAR                 = make_GCHAR(G0, ')'),
        GC_AST                  = make_GCHAR(G0, '*'),
        GC_PLUS                 = make_GCHAR(G0, '+'),
        GC_COMMA                = make_GCHAR(G0, ','),
        GC_MINUS                = make_GCHAR(G0, '-'),
        GC_PERIOD               = make_GCHAR(G0, '.'),
        GC_SLASH                = make_GCHAR(G0, '/'),
        GC_0                    = make_GCHAR(G0, '0'),
        GC_1                    = make_GCHAR(G0, '1'),
        GC_2                    = make_GCHAR(G0, '2'),
        GC_3                    = make_GCHAR(G0, '3'),
        GC_4                    = make_GCHAR(G0, '4'),
        GC_5                    = make_GCHAR(G0, '5'),
        GC_6                    = make_GCHAR(G0, '6'),
        GC_7                    = make_GCHAR(G0, '7'),
        GC_8                    = make_GCHAR(G0, '8'),
        GC_9                    = make_GCHAR(G0, '9'),
        GC_COLON                = make_GCHAR(G0, ':'),
        GC_SEMI                 = make_GCHAR(G0, ';'),
        GC_LT                   = make_GCHAR(G0, '<'),
        GC_EQUALS               = make_GCHAR(G0, '='),
        GC_GT                   = make_GCHAR(G0, '>'),
        GC_QUEST                = make_GCHAR(G0, '?'),
        GC_AT                   = make_GCHAR(G0, '@'),
        GC_A                    = make_GCHAR(G0, 'A'),
        GC_A_CIRC               = make_GCHAR_accent(GC_A, GCA_CIRC),        // only on VGP2
        GC_B                    = make_GCHAR(G0, 'B'),
        GC_C                    = make_GCHAR(G0, 'C'),
        GC_C_CEDILLA            = make_GCHAR_accent(GC_C, GCA_CEDILLA),     // only on VGP2
        GC_D                    = make_GCHAR(G0, 'D'),
        GC_E                    = make_GCHAR(G0, 'E'),
        GC_E_GRAVE              = make_GCHAR_accent(GC_E, GCA_GRAVE),       // only on VGP2
        GC_E_ACUTE              = make_GCHAR_accent(GC_E, GCA_ACUTE),       // only on VGP2
        GC_F                    = make_GCHAR(G0, 'F'),
        GC_G                    = make_GCHAR(G0, 'G'),
        GC_H                    = make_GCHAR(G0, 'H'),
        GC_I                    = make_GCHAR(G0, 'I'),
        GC_J                    = make_GCHAR(G0, 'J'),
        GC_K                    = make_GCHAR(G0, 'K'),
        GC_L                    = make_GCHAR(G0, 'L'),
        GC_M                    = make_GCHAR(G0, 'M'),
        GC_N                    = make_GCHAR(G0, 'N'),
        GC_O                    = make_GCHAR(G0, 'O'),
        GC_P                    = make_GCHAR(G0, 'P'),
        GC_Q                    = make_GCHAR(G0, 'Q'),
        GC_R                    = make_GCHAR(G0, 'R'),
        GC_S                    = make_GCHAR(G0, 'S'),
        GC_T                    = make_GCHAR(G0, 'T'),
        GC_U                    = make_GCHAR(G0, 'U'),
        GC_V                    = make_GCHAR(G0, 'V'),
        GC_W                    = make_GCHAR(G0, 'W'),
        GC_X                    = make_GCHAR(G0, 'X'),
        GC_Y                    = make_GCHAR(G0, 'Y'),
        GC_Z                    = make_GCHAR(G0, 'Z'),
        GC_LSQB                 = make_GCHAR(G0, '['),
        GC_BSLASH               = make_GCHAR(G0, '\\'),
        GC_RSQB                 = make_GCHAR(G0, L']'),
        GC_UPARROW              = make_GCHAR(G0, 0x5E),
        GC_UNDRSCR              = make_GCHAR(G0, '_'),
        GC_MIDLSCR              = make_GCHAR(G0, 0x60),
        GC_a                    = make_GCHAR(G0, 'a'),
        GC_a_CIRC               = make_GCHAR_accent(GC_a, GCA_CIRC),
        GC_a_GRAVE              = make_GCHAR_accent(GC_a, GCA_GRAVE),
        GC_a_DIAERESIS          = make_GCHAR_accent(GC_a, GCA_DIAERESIS),   // only on VGP5
        GC_b                    = make_GCHAR(G0, 'b'),
        GC_c                    = make_GCHAR(G0, 'c'),
        GC_c_CEDILLA            = make_GCHAR_accent(GC_c, GCA_CEDILLA),
        GC_d                    = make_GCHAR(G0, 'd'),
        GC_e                    = make_GCHAR(G0, 'e'),
        GC_e_CIRC               = make_GCHAR_accent(GC_e, GCA_CIRC),
        GC_e_GRAVE              = make_GCHAR_accent(GC_e, GCA_GRAVE),
        GC_e_ACUTE              = make_GCHAR_accent(GC_e, GCA_ACUTE),
        GC_e_DIAERESIS          = make_GCHAR_accent(GC_e, GCA_DIAERESIS),
        GC_f                    = make_GCHAR(G0, 'f'),
        GC_g                    = make_GCHAR(G0, 'g'),
        GC_h                    = make_GCHAR(G0, 'h'),
        GC_i                    = make_GCHAR(G0, 'i'),
        GC_i_CIRC               = make_GCHAR_accent(GC_i, GCA_CIRC),
        GC_i_DIAERESIS          = make_GCHAR_accent(GC_i, GCA_DIAERESIS),
        GC_j                    = make_GCHAR(G0, 'j'),
        GC_k                    = make_GCHAR(G0, 'k'),
        GC_l                    = make_GCHAR(G0, 'l'),
        GC_m                    = make_GCHAR(G0, 'm'),
        GC_n                    = make_GCHAR(G0, 'n'),
        GC_o                    = make_GCHAR(G0, 'o'),
        GC_o_CIRC               = make_GCHAR_accent(GC_o, GCA_CIRC),
        GC_o_DIAERESIS          = make_GCHAR_accent(GC_o, GCA_DIAERESIS),   // only on VGP5
        GC_p                    = make_GCHAR(G0, 'p'),
        GC_q                    = make_GCHAR(G0, 'q'),
        GC_r                    = make_GCHAR(G0, 'r'),
        GC_s                    = make_GCHAR(G0, 's'),
        GC_t                    = make_GCHAR(G0, 't'),
        GC_u                    = make_GCHAR(G0, 'u'),
        GC_u_CIRC               = make_GCHAR_accent(GC_u, GCA_CIRC),
        GC_u_GRAVE              = make_GCHAR_accent(GC_u, GCA_GRAVE),
        GC_u_DIAERESIS          = make_GCHAR_accent(GC_u, GCA_DIAERESIS),   // only on VGP5
        GC_v                    = make_GCHAR(G0, 'v'),
        GC_w                    = make_GCHAR(G0, 'w'),
        GC_x                    = make_GCHAR(G0, 'x'),
        GC_y                    = make_GCHAR(G0, 'y'),
        GC_z                    = make_GCHAR(G0, 'z'),
        GC_LVERBAR              = make_GCHAR(G0, 0x7B),
        GC_VERBAR               = make_GCHAR(G0, '|'),
        GC_RVERBAR              = make_GCHAR(G0, 0x7D),
        GC_UPRSCR               = make_GCHAR(G0, 0x7E),

        GC_POUND                = make_GCHAR(G2, 0x23),
        GC_DOLLAR_G2            = make_GCHAR(G2, '$'),      // $ is in both G0 and G2 at the same position ...
        GC_NUM_G2               = make_GCHAR(G2, 0x26),     // # is in both G0 and G2
        GC_SECT                 = make_GCHAR(G2, 0x27),
        GC_LARROW               = make_GCHAR(G2, 0x2C),
        GC_UPARROW_G2           = make_GCHAR(G2, 0x2D),     // uparrow is in both G0 and G2
        GC_RARROW               = make_GCHAR(G2, 0x2E),
        GC_DARROW               = make_GCHAR(G2, 0x2F),
        GC_DEG                  = make_GCHAR(G2, 0x30),
        GC_PLUSMN               = make_GCHAR(G2, 0x31),
        GC_DIV                  = make_GCHAR(G2, 0x38),
        GC_FRAC14               = make_GCHAR(G2, 0x3C),
        GC_FRAC12               = make_GCHAR(G2, 0x3D),
        GC_FRAC34               = make_GCHAR(G2, 0x3E),
        GC_LIG_OE               = make_GCHAR(G2, 0x6A),
        GC_LIG_oe               = make_GCHAR(G2, 0x7A),
        GC_ESZETT               = make_GCHAR(G2, 0x7B),

        GC_SPACE                = make_GCHAR(SPEC, ' '),
    };

    class MinitelScreenCell : public sf::Drawable, public sf::Transformable
    {
    private:
        sf::Vector2f    m_size;
        sf::Text        m_char;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    public:
        MinitelScreenCell();
        void setSize(const sf::Vector2f& size);
    };
}


#endif //MINITELVIEWER_MINITELSCREENCELL_H
