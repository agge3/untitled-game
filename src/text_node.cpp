#include "text_node.h"
#include "utility.h"

#include <SFML/Graphics/RenderTarget.hpp>

TextNode::TextNode(const FontHolder& fonts, const std::string& text)
{
    m_text.setFont(fonts.get(Fonts::Main));
    m_text.setCharacterSize(14);
    set_string(text);
}

void TextNode::draw_current(sf::RenderTarget& target, sf::RenderStates states)
    const
{
    target.draw(m_text, states);
}

void TextNode::set_string(const std::string& text)
{
    m_text.setString(text);
    center_origin(m_text);
}
