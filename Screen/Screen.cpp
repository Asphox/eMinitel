// Screen.cpp
// Created by asphox on 26/12/2023.
// Manages the drawing of the entire Minitel screen

#include "Screen.h"

Screen::Screen(float height)
{
    for(int i=0; i<40*25; i++)
    {
        m_cells.emplace_back(*this);
    }
    clear();
    set_height(height);
}

void Screen::set_height(float height)
{
    m_size = {height*SCREEN_RATIO, height};
    for(std::size_t line=0; line<25; line++)
    {
        for(std::size_t col=0; col<40; col++)
        {
            ScreenCell&  cell = m_cells[col + line * 40];
            cell.setSize({m_size.x/40, m_size.y/25});
            cell.setPosition(col*m_size.x/40, line*m_size.y/25);
        }
    }
    m_cursor.setSize({m_size.x/40, m_size.y/25});
}

void Screen::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    // we draw non-normal-sized glyph after all the normal ones
    // to avoid background overlap
    std::vector<const ScreenCell*> non_normal_sized_cells;

    states.transform *= getTransform();
    for(const ScreenCell& r_cell : m_cells)
    {
        if(get_glyph_att_size(r_cell.get_glyph()) == GS_NORMAL)
            target.draw(r_cell, states);
        else
            non_normal_sized_cells.push_back(&r_cell);
    }

    for(const ScreenCell* p_non_normal_sized_cell : non_normal_sized_cells)
        target.draw(*p_non_normal_sized_cell, states);

    if(m_cursor_visible && m_blink_state)
    {
        target.draw(m_cursor, states);
    }
}

void Screen::__apply_zones_attributes()
{
    for(int line = 1; line<25; line++)
    {
        GLYPH_ATT_BCOLOR bcolor = GBC_BLACK;
        bool underline = false;
        bool masked    = false;
        for(int col = 0; col < 40; col++)
        {
            ScreenCell& r_cell = m_cells[col+line*40];
            GLYPH_CODE gc = r_cell.get_glyph();
            if(is_glyph_zone_delimiter(gc))
            {
                bcolor    = get_glyph_att_bcolor(gc);
                underline = get_glyph_att_underline(gc);
                masked    = get_glyph_att_mask(gc);
            }
            else
            {
                gc = set_glyph_att_bcolor(gc, bcolor);
                gc = set_glyph_att_underline(gc, underline);
                gc = set_glyph_att_mask(gc, masked);
            }
            r_cell.set_glyph(gc);
        }
    }
}

void Screen::s_mtlc_screen_control(MTLC_OP_SCREEN op, intptr_t param, void* ctx)
{
    if(!ctx)
        return;
    Screen* _this = (Screen*)ctx;
    switch(op)
    {
        case OPS_UPDATE:
            _this->update();
            break;
        case OPS_CLEAR:
            _this->clear(param);
            break;
        case OPS_SCROLL:
            _this->scroll((std::int8_t)param);
            break;
        case OPS_SET_HEIGHT:
            _this->set_height(param);
            break;
        case OPS_SET_GLYPH_AT:
        {
            auto param_set_glyph_at = (const mtlc_op_screen_param_set_glyph_at*)param;
            _this->set_glyph_at(param_set_glyph_at->gc, param_set_glyph_at->pos.col, param_set_glyph_at->pos.line);
            break;
        }
        case OPS_SET_CURSOR_POS:
        {
            auto param_set_cursor_pos = (const mtlc_op_screen_param_set_cursor_pos*)param;
            _this->set_cursor_pos(param_set_cursor_pos->col, param_set_cursor_pos->line);
            break;
        }
        case OPS_SET_CURSOR_VISIBLE:
            _this->set_cursor_visible(param);
            break;
        case OPS_SET_MASK:
            _this->set_mask(param);
            break;
        default:
            break;
    }
}

void Screen::update()
{
    if(m_clock_blink.getElapsedTime().asSeconds() > 0.7)
    {
        m_blink_state = !m_blink_state;
        m_clock_blink.restart();
    }
    __apply_zones_attributes();
}

void Screen::clear(bool keep_line_0)
{
    for(int line = keep_line_0 ? 1 : 0; line<25; line++)
    {
        for(int col = 0; col < 40; col++)
        {
            m_cells[col+line*40].set_glyph(GC_SPACE);
        }
    }
}

void Screen::set_glyph_at(GLYPH_CODE gc, std::uint8_t col, std::uint8_t line)
{
    m_cells[col+line*40].set_glyph(gc);
}

void Screen::set_cursor_pos(std::uint8_t col, std::uint8_t line)
{
    if(col > 39 || line > 24)
        return;

    m_cursor_pos.x = col;
    m_cursor_pos.y = line;
    m_cursor.setPosition(col*m_cursor.getSize().x, line*m_cursor.getSize().y);
}

void Screen::set_cursor_visible(bool visible)
{
    m_cursor_visible = visible;
}

void Screen::set_mask(bool mask)
{
    m_masked = mask;
}

void Screen::scroll(std::int8_t nb_of_line)
{
    if(abs(nb_of_line) > 23)
        clear(true);
    else if(nb_of_line == 0)
        return;
    else
    {
        bool scroll_up = nb_of_line < 0;
        nb_of_line = abs(nb_of_line);
        if(scroll_up)
        {
            for(std::uint8_t line = nb_of_line; line < 25-nb_of_line; line++)
            {
                std::uint8_t src_line = line + nb_of_line;
                for(std::uint8_t col = 0; col<40; col++)
                {
                    ScreenCell& src_cell = m_cells[col+src_line*40];
                    ScreenCell& dst_cell = m_cells[col+line*40];
                    dst_cell = src_cell;
                    src_cell.set_glyph(GC_SPACE);
                }
            }
        }
        else
        {
            for(std::uint8_t line = 24-nb_of_line; line >= 2; line--)
            {
                std::uint8_t src_line = line - nb_of_line;
                for(std::uint8_t col = 0; col<40; col++)
                {
                    ScreenCell& src_cell = m_cells[col+src_line*40];
                    ScreenCell& dst_cell = m_cells[col+line*40];
                    dst_cell = src_cell;
                    src_cell.set_glyph(GC_SPACE);
                }
            }
        }
    }
}