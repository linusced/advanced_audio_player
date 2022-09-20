#pragma once

#include "common.hpp"

namespace lc_gui
{
    class Window
    {
    public:
        Window(int x, int y, int width, int height, const std::string &title);
        Window(int fullscreenWidth, int fullscreenHeight, const std::string &title);
        ~Window();

        const int &x() const;
        const int &y() const;
        const int &width() const;
        const int &height() const;

        void setPosition(int x, int y);
        void setSize(int width, int height);

        void setFullscreen(int fullscreenWidth, int fullscreenHeight);
        void exitFullscreen(int x, int y, int width, int height);

        void update();
        void swapBuffers();

        void makeContextCurrent();
        void close();

        bool hasResized();
        bool isFullscreen() const;
        bool shouldClose() const;

        void setTitle(const std::string &title);
        void setCursor(GLenum cursor);

        const double &getCurrentTime() const;
        const double &getDeltaTime() const;

        static const double &getScrollY();
        static const double &getScrollX();
        static const unsigned int &getCharCodePoint();
        static const bool &isLmbDown();
        static const bool &isRmbDown();

        static int getMonitorWidth();
        static int getMonitorHeight();
        static float getContentScale();

        operator GLFWwindow *() const { return _window; };
        GLFWwindow *get();

    private:
        void createWindow(int width, int height, const std::string &title);

        GLFWwindow *_window{nullptr};
        int _x, _y, _width, _height,
            prevWidth{0}, prevHeight{0};

        std::unordered_map<GLenum, GLFWcursor *> standardCursors;
        GLenum frameCursor{GLFW_ARROW_CURSOR};

        double currentTime{0.0}, deltaTime{0.0};

        static double scrollY, scrollX;
        static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

        static unsigned int charCodePoint;
        static void character_callback(GLFWwindow *window, unsigned int codePoint);

        static bool lmbDown, lmbUp, rmbDown, rmbUp,
            mouseButtonEventHandled;
        static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
    };
}