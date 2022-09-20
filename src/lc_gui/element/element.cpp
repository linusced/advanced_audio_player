#include "lc_gui/element.hpp"

lc_gui::Element::Element(const std::string &className)
{
    this->classList = className;
}

void lc_gui::Element::resizeMesh(const Style &style, const Layout &layout, const glm::ivec2 &windowSize) {}
void lc_gui::Element::update() {}

void lc_gui::Element::addChild(Element *child)
{
    child->window = this->window;
    child->parentElement = this;
    children.push_back(child);
}

const lc_gui::Style &lc_gui::Element::getPreviousStyle() const
{
    return prevStyle;
}
const lc_gui::Layout &lc_gui::Element::getPreviousLayout() const
{
    return prevLayout;
}
const lc_gui::Layout &lc_gui::Element::getPreviousContainerLayout() const
{
    return prevContainerLayout;
}
int lc_gui::Element::getMaxY() const
{
    int maxY = bottomY;

    for (auto &ch : children)
    {
        int chMaxY = ch->getMaxY();
        if (chMaxY > maxY)
            maxY = chMaxY;
    }

    return maxY;
}