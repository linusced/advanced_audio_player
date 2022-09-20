#include "main/window_position.hpp"

void window_position::load(const std::string &appSupportFilePath, lc_gui::Layout *windowLayout)
{
    std::ifstream file(appSupportFilePath + "window_position.dat");
    if (!file.is_open())
        return;

    std::string key, value;
    while (!file.eof())
    {
        file >> key >> value;

        bool invalidNum = false;
        for (auto &c : value)
            if (c < '0' || c > '9')
            {
                invalidNum = true;
                break;
            }

        if (!invalidNum)
        {
            if (key == "x")
                windowLayout->x = std::stoi(value);
            else if (key == "y")
                windowLayout->y = std::stoi(value);
            else if (key == "w")
                windowLayout->width = std::stoi(value);
            else if (key == "h")
                windowLayout->height = std::stoi(value);
        }
    }
    file.close();
}

void window_position::save(const std::string &appSupportFilePath, lc_gui::Window *window)
{
    std::ofstream file(appSupportFilePath + "window_position.dat");
    if (!file.is_open())
        return;

    file << "x " << window->x()
         << "\ny " << window->y()
         << "\nw " << window->width()
         << "\nh " << window->height();

    file.close();
}