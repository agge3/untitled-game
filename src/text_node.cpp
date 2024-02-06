#include "text_node.h"
#include "utility.h"

#include <SFML/Graphics/RenderTarget.hpp>

/**
 * Default constructor sets font of TextNode to parameter, sets font size, and
 * sets text displayed to std::string parameter.
 * */
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

/**
 * Separate utility fn to both setString() and center_origin() in one fn call.
 * @param const std::string& text
 * The std::string to be set as the text for the TextNode.
 */
void TextNode::set_string(const std::string& text)
{
    m_text.setString(text);
    center_origin(m_text);
}
