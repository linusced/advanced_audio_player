#include "lc_utility/binary_file.hpp"

size_t lc_utility::BinaryFile::size() const
{
    return body.size();
}

lc_utility::BinaryFileInput::BinaryFileInput(const std::string &filePath)
{
    std::ifstream fileIn{filePath, std::ios::binary};
    if (fileIn.is_open())
    {
        fileIn.seekg(0, std::ios::end);
        size_t size = fileIn.tellg();
        fileIn.seekg(0, std::ios::beg);

        body.resize(size);
        fileIn.read((char *)body.data(), size);
    }
}

size_t lc_utility::BinaryFileInput::getReadIndex() const
{
    return readIndex;
}

lc_utility::BinaryFileOutput::~BinaryFileOutput()
{
    std::ofstream fileOut(filePath, std::ios::binary);
    if (fileOut.is_open())
    {
        fileOut.write((const char *)body.data(), body.size());
    }
}