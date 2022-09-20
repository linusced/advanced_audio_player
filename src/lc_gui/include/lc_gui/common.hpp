#pragma once

#include <iostream>
#include <cmath>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <thread>
#include <exception>
#include <memory>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

#ifndef STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#endif
#ifndef STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>
#endif
#ifndef STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>
#endif

namespace lc_gui
{
    class ID
    {
    public:
        const unsigned int &getID() const { return id; }

    protected:
        unsigned int id = -1;
    };
}