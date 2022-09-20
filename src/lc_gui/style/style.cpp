#include "lc_gui/style.hpp"

lc_gui::Style::Style()
{
    sizeProperties = {
        {"width", SIZE()},
        {"min-width", SIZE()},
        {"max-width", SIZE()},
        {"height", SIZE()},
        {"min-height", SIZE()},
        {"max-height", SIZE()},
        {"top", SIZE()},
        {"bottom", SIZE()},
        {"left", SIZE()},
        {"right", SIZE()},
        {"border-width", SIZE()},
        {"border-radius", SIZE()},
        {"padding-top", SIZE()},
        {"padding-bottom", SIZE()},
        {"padding-left", SIZE()},
        {"padding-right", SIZE()},
        {"font-size", SIZE()},
        {"scale", SIZE()},
        {"translate-x", SIZE()},
        {"translate-y", SIZE()},
    };

    colorProperties = {
        {"color", COLOR()},
        {"background-color", COLOR()},
        {"border-color", COLOR()},
    };

    miscProperties = {
        {"overflow", MISC()},
        {"visibility", MISC()},
        {"text-align", MISC()},
    };
}

void lc_gui::Style::add(const Style &s)
{
    for (auto &sizePair : this->sizeProperties)
        if (!sizePair.second.isImportant && s.sizeProperties.at(sizePair.first).isSet)
            sizePair.second = s.sizeProperties.at(sizePair.first);

    for (auto &colorPair : this->colorProperties)
        if (!colorPair.second.isImportant && s.colorProperties.at(colorPair.first).isSet)
            colorPair.second = s.colorProperties.at(colorPair.first);

    for (auto &miscPair : this->miscProperties)
        if (!miscPair.second.isImportant && s.miscProperties.at(miscPair.first).isSet)
            miscPair.second = s.miscProperties.at(miscPair.first);
}