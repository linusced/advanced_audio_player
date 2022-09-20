#include "lc_gui/window.hpp"

lc_gui::Window::Window(int x, int y, int width, int height, const std::string &title)
    : _x(x), _y(y), _width(width), _height(height)
{
    createWindow(width, height, title);

    if (x != -1 && y != -1)
        setPosition(x, y);

    setSize(width, height);
}
lc_gui::Window::Window(int fullscreenWidth, int fullscreenHeight, const std::string &title)
    : _x(0), _y(0), _width(fullscreenWidth), _height(fullscreenHeight)
{
    createWindow(fullscreenWidth, fullscreenHeight, title);

    setFullscreen(fullscreenWidth, fullscreenHeight);
}
lc_gui::Window::~Window()
{
    for (auto &i : standardCursors)
        glfwDestroyCursor(i.second);

    glfwDestroyWindow(_window);
}

const int &lc_gui::Window::x() const
{
    return _x;
}
const int &lc_gui::Window::y() const
{
    return _y;
}
const int &lc_gui::Window::width() const
{
    return _width;
}
const int &lc_gui::Window::height() const
{
    return _height;
}

void lc_gui::Window::createWindow(int width, int height, const std::string &title)
{
    _window = glfwCreateWindow(width, height, title.c_str(), 0, 0);

    if (!glfwGetCurrentContext())
    {
        makeContextCurrent();
        gladLoadGL();
    }

    glfwSetScrollCallback(_window, scroll_callback);
    glfwSetCharCallback(_window, character_callback);
    glfwSetMouseButtonCallback(_window, mouse_button_callback);

    standardCursors[GLFW_ARROW_CURSOR] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
    standardCursors[GLFW_HAND_CURSOR] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
    standardCursors[GLFW_IBEAM_CURSOR] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
    standardCursors[GLFW_HRESIZE_CURSOR] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
    standardCursors[GLFW_VRESIZE_CURSOR] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
    standardCursors[GLFW_CROSSHAIR_CURSOR] = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
}

void lc_gui::Window::setPosition(int x, int y)
{
    glfwSetWindowPos(_window, x, y);
}
void lc_gui::Window::setSize(int width, int height)
{
    glfwSetWindowSize(_window, width, height);
}

void lc_gui::Window::setFullscreen(int fullscreenWidth, int fullscreenHeight)
{
    glfwSetWindowMonitor(_window, glfwGetPrimaryMonitor(), 0, 0, fullscreenWidth, fullscreenHeight, -1);
}
void lc_gui::Window::exitFullscreen(int x, int y, int width, int height)
{
    glfwSetWindowMonitor(_window, nullptr, x, y, width, height, -1);
}

void lc_gui::Window::update()
{
    double glfwTime = glfwGetTime();
    deltaTime = glfwTime - currentTime;
    currentTime = glfwTime;

    scrollY = scrollX = 0.0;
    charCodePoint = 0;
    mouseButtonEventHandled = true;
    if (lmbUp)
        lmbDown = false;
    if (rmbUp)
        rmbDown = false;

    glfwPollEvents();
    glfwGetWindowPos(_window, &_x, &_y);
    glfwGetWindowSize(_window, &_width, &_height);
}
void lc_gui::Window::swapBuffers()
{
    glfwSetCursor(_window, standardCursors[frameCursor]);
    frameCursor = GLFW_ARROW_CURSOR;
    glfwSwapBuffers(_window);
}

void lc_gui::Window::makeContextCurrent()
{
    glfwMakeContextCurrent(_window);
}
void lc_gui::Window::close()
{
    glfwSetWindowShouldClose(_window, true);
}

bool lc_gui::Window::hasResized()
{
    bool b = _width != prevWidth || _height != prevHeight;
    prevWidth = _width;
    prevHeight = _height;
    return b;
}
bool lc_gui::Window::isFullscreen() const
{
    return glfwGetWindowMonitor(_window) != nullptr;
}
bool lc_gui::Window::shouldClose() const
{
    return glfwWindowShouldClose(_window);
}

void lc_gui::Window::setTitle(const std::string &title)
{
    glfwSetWindowTitle(_window, title.c_str());
}
void lc_gui::Window::setCursor(GLenum cursor)
{
    if (standardCursors.find(cursor) != standardCursors.end())
        frameCursor = cursor;
}

const double &lc_gui::Window::getCurrentTime() const
{
    return currentTime;
}
const double &lc_gui::Window::getDeltaTime() const
{
    return deltaTime;
}

GLFWwindow *lc_gui::Window::get()
{
    return _window;
}

double lc_gui::Window::scrollY{0.0}, lc_gui::Window::scrollX{0.0};
unsigned int lc_gui::Window::charCodePoint{0};

const double &lc_gui::Window::getScrollY()
{
    return scrollY;
}
const double &lc_gui::Window::getScrollX()
{
    return scrollX;
}
void lc_gui::Window::scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    scrollY = yoffset;
    scrollX = xoffset;
}

const unsigned int &lc_gui::Window::getCharCodePoint()
{
    return charCodePoint;
}
void lc_gui::Window::character_callback(GLFWwindow *window, unsigned int codePoint)
{
    charCodePoint = codePoint;
}

bool lc_gui::Window::lmbDown{false};
bool lc_gui::Window::lmbUp{false};
bool lc_gui::Window::rmbDown{false};
bool lc_gui::Window::rmbUp{false};
bool lc_gui::Window::mouseButtonEventHandled{false};

const bool &lc_gui::Window::isLmbDown()
{
    return lmbDown;
}
const bool &lc_gui::Window::isRmbDown()
{
    return rmbDown;
}
void lc_gui::Window::mouse_button_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (action)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            lmbDown = true;
            lmbUp = false;
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            rmbDown = true;
            rmbUp = false;
        }

        mouseButtonEventHandled = false;
    }
    else if (mouseButtonEventHandled)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
            lmbDown = false;
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
            rmbDown = false;
    }
    else
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT)
            lmbUp = true;
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
            rmbUp = true;
    }
}

int lc_gui::Window::getMonitorWidth()
{
    return glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
}
int lc_gui::Window::getMonitorHeight()
{
    return glfwGetVideoMode(glfwGetPrimaryMonitor())->height;
}
float lc_gui::Window::getContentScale()
{
    float x, y;
    glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &x, &y);
    return y < x ? y : x;
}