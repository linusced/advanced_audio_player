#pragma once

#include <vector>
#include <string>
#include <fstream>

namespace lc_utility
{
    class BinaryFile
    {
    public:
        size_t size() const;

    protected:
        BinaryFile() {}

        std::vector<uint8_t> body;
    };

    class BinaryFileInput : public BinaryFile
    {
    public:
        BinaryFileInput(const std::string &filePath);

        template <typename T>
        void read(T *data, size_t arrSize)
        {
            if (arrSize == 0)
                return;

            size_t bytesSize = sizeof(T) * arrSize;
            if (readIndex + bytesSize > body.size())
                throw std::invalid_argument("lc_utility::BinaryFileInput::read End of file reached!");

            memcpy(data, body.data() + readIndex, bytesSize);
            readIndex += bytesSize;
        }

        template <typename T>
        T read()
        {
            T data;
            read(&data, 1);
            return data;
        }

        size_t getReadIndex() const;

    private:
        size_t readIndex{0};
    };

    class BinaryFileOutput : public BinaryFile
    {
    public:
        BinaryFileOutput(const std::string &filePath) : filePath(filePath) {}
        ~BinaryFileOutput();

        template <typename T>
        void write(const T *data, size_t arrSize)
        {
            if (arrSize == 0)
                return;

            size_t i = body.size(), bytesSize = sizeof(T) * arrSize;
            body.resize(i + bytesSize);
            memcpy(body.data() + i, data, bytesSize);
        }

        template <typename T>
        void write(T data)
        {
            write(&data, 1);
        }

    private:
        std::string filePath;
    };
}