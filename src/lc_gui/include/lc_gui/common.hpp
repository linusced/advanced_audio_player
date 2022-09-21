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

#include <stb/stb.h>

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