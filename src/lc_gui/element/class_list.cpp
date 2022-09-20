#include "lc_gui/element.hpp"

std::vector<std::string>::iterator lc_gui::ClassList::begin()
{
    return strArray.begin();
}
std::vector<std::string>::iterator lc_gui::ClassList::end()
{
    return strArray.end();
}
const size_t &lc_gui::ClassList::size() const
{
    return strArray.size();
}
std::string &lc_gui::ClassList::operator[](size_t index)
{
    return strArray[index];
}
bool lc_gui::ClassList::contains(const std::string &str)
{
    return std::find(begin(), end(), str) != end();
}

void lc_gui::ClassList::remove(const std::string &str)
{
    auto it = std::find(begin(), end(), str);
    if (it != end())
        strArray.erase(it);
}
void lc_gui::ClassList::add(const std::string &str)
{
    auto it = std::find(begin(), end(), str);
    if (it == end())
        strArray.push_back(str);
}
void lc_gui::ClassList::clear()
{
    strArray.clear();
}

void lc_gui::ClassList::operator=(const std::string &className)
{
    clear();

    size_t currentPos = 0, whitespacePos;
    do
    {
        whitespacePos = className.find(' ', currentPos);
        if (whitespacePos == std::string::npos)
            whitespacePos = className.size();

        add(className.substr(currentPos, whitespacePos - currentPos));

        currentPos = whitespacePos + 1;

    } while (currentPos < className.size());
}