#include "lc_gui/data_loaders.hpp"

std::string lc_gui::textFileData(const std::string &filePath)
{
    std::string data;
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open())
        throw std::invalid_argument("lc_gui::textFileData Failed to load file \"" + filePath + '\"');

    file.seekg(0, std::ios::end);
    data.resize(file.tellg());
    file.seekg(0, std::ios::beg);
    file.read(&data[0], data.size());
    file.close();

    return data;
}

lc_gui::TextureData::TextureData(const std::string &filePath)
{
    load(filePath);
}

void lc_gui::TextureData::load(const std::string &filePath)
{
    int nColorChannels;
    unsigned char *rawBytes = stbi_load(filePath.c_str(), &width, &height, &nColorChannels, 0);
    if (width < 1 || height < 1 || nColorChannels < 1)
        throw std::invalid_argument("lc_gui::TextureData::load Failed to load image \"" + filePath + '\"');

    bytes.resize(width * height * nColorChannels);
    for (int i = 0; i < bytes.size(); i++)
        bytes[i] = rawBytes[i];

    free(rawBytes);

    format = GL_RGBA;
    if (nColorChannels == 1)
        format = GL_RED;
    else if (nColorChannels == 2)
        format = GL_RG;
    else if (nColorChannels == 3)
        format = GL_RGB;
}