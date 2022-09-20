#include "lc_gui/text_renderer.hpp"

lc_gui::TextRenderer::Font::Font(const std::string &filePath)
{
    if (!std::filesystem::exists(filePath))
        throw std::invalid_argument("lc_gui::Font Failed to load file \"" + filePath + '\"');

    FILE *fontFile = fopen(filePath.c_str(), "rb");
    fseek(fontFile, 0, SEEK_END);
    unsigned long size = ftell(fontFile);
    fseek(fontFile, 0, SEEK_SET);
    buffer.resize(size);
    fread(buffer.data(), size, 1, fontFile);
    fclose(fontFile);
}

const uint8_t *lc_gui::TextRenderer::Font::data() const
{
    return buffer.data();
}
size_t lc_gui::TextRenderer::Font::size() const
{
    return buffer.size();
}

void lc_gui::TextRenderer::render(const std::string &text, const glm::vec4 &color, int fontSize, int maxWidth, const Font *font)
{
    render(text, color, fontSize, maxWidth, font, {});
}

void lc_gui::TextRenderer::render(const std::string &text, const glm::vec4 &color, int fontSize, int maxWidth, const Font *font, const std::vector<CharacterColor> &characterColors)
{
    const size_t textSize = text.empty() ? 1 : text.size();

    std::string str = text.empty() ? " " : text;
    std::vector<std::string> lines;

    lineLayouts.clear();
    int lineOffsetY = 0, lineHeightReal = fontSize + 4,
        lineCount = std::count(str.begin(), str.end(), '\n') + 1;
    for (int i = 0; i < lineCount; i++)
    {
        lines.push_back(str.substr(0, str.find('\n')));
        if (i + 1 < lineCount)
            str = str.substr(str.find('\n') + 1);

        lineLayouts.push_back({0, lineOffsetY, maxWidth, lineHeightReal});
        lineOffsetY += lineHeightReal;
    }

    stbtt_fontinfo info;
    stbtt_InitFont(&info, font->data(), 0);

    int imgSizeMult = 2, lineHeight = lineHeightReal * imgSizeMult;
    float scale = stbtt_ScaleForPixelHeight(&info, fontSize * imgSizeMult);
    int x = 0, ascent, descent, lineGap, lineOffset = 0;
    stbtt_GetFontVMetrics(&info, &ascent, &descent, &lineGap);
    ascent = std::round(ascent * scale);
    descent = std::round(descent * scale);

    if (maxWidth == -1)
        maxWidth = sizeof(int) == 4 ? INT32_MAX : INT16_MAX;
    else
        maxWidth *= imgSizeMult;

    std::string newLine;
    int lineLayoutIndexOffset = 0, lineLayoutIndex;

    for (int l = 0; l < lines.size(); l++)
    {
        x = 0;
        str = lines[l];
        for (int i = 0; i < str.size(); i++)
        {
            int ax;
            stbtt_GetCodepointHMetrics(&info, str[i], &ax, (int *)0);
            x += std::round(ax * scale);
            int kern = stbtt_GetCodepointKernAdvance(&info, str[i], str[i + 1]);
            x += std::round(kern * scale);
            if (x >= maxWidth - lineHeight)
            {
                size_t split = str.substr(0, i + 1).find_last_of(' ');
                if (split == std::string::npos)
                    split = i + 1;

                if (split < str.size())
                {
                    lines[l] = str.substr(0, split + 1);
                    newLine = str.substr(split + 1);
                    if (!newLine.empty())
                    {
                        lines.insert(lines.begin() + l + 1, newLine);

                        lineLayoutIndex = l + lineLayoutIndexOffset;
                        lineLayouts[lineLayoutIndex].height += lineHeightReal;

                        for (++lineLayoutIndex; lineLayoutIndex < lineLayouts.size(); lineLayoutIndex++)
                            lineLayouts[lineLayoutIndex].y += lineHeightReal;

                        lineLayoutIndexOffset--;
                    }
                }
                break;
            }
        }
    }

    for (int l = lines.size() - 1; l >= 1; l--)
        if (lines[l] == " ")
            lines.erase(lines.begin() + l);

    textureHeight = lines.size() * lineHeight;
    textureWidth = 0;
    characterLayouts.resize(textSize);

    int charIndex = 0;
    for (int l = 0; l < lines.size(); l++)
    {
        x = 0;
        int prevX = 0;
        str = lines[l];
        for (int i = 0; i < str.size(); i++)
        {
            int ax;
            stbtt_GetCodepointHMetrics(&info, str[i], &ax, (int *)0);
            x += std::round(ax * scale);
            int kern = stbtt_GetCodepointKernAdvance(&info, str[i], str[i + 1]);
            x += std::round(kern * scale);

            characterLayouts[charIndex].x = prevX;
            characterLayouts[charIndex].y = l * lineHeight;
            characterLayouts[charIndex].width = x - prevX;
            characterLayouts[charIndex].height = lineHeight;
            prevX = x;
            charIndex++;
        }

        if (x > textureWidth)
            textureWidth = x;
    }

    std::vector<unsigned char> bitmap(textureWidth * textureHeight), rawTextData(bitmap.size() * 4);

    for (int l = 0; l < lines.size(); l++)
    {
        x = 0;
        str = lines[l];
        for (int i = 0; i < str.size(); i++)
        {
            int ax, lsb;
            stbtt_GetCodepointHMetrics(&info, str[i], &ax, &lsb);
            int c_x1, c_y1, c_x2, c_y2;
            stbtt_GetCodepointBitmapBox(&info, str[i], scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);
            int y = ascent + c_y1 + lineOffset, byteOffset = x + std::round(lsb * scale) + (y * textureWidth);
            stbtt_MakeCodepointBitmap(&info, bitmap.data() + byteOffset, c_x2 - c_x1, c_y2 - c_y1, textureWidth, scale, scale, str[i]);
            x += std::round(ax * scale);
            int kern = stbtt_GetCodepointKernAdvance(&info, str[i], str[i + 1]);
            x += std::round(kern * scale);
        }
        lineOffset += lineHeight;
    }

    for (unsigned long i = 0, j = 0; i < bitmap.size(); i++, j += 4)
    {
        rawTextData[j] = 255 * color.r;
        rawTextData[j + 1] = 255 * color.g;
        rawTextData[j + 2] = 255 * color.b;
        rawTextData[j + 3] = bitmap[i] > 127 ? 255 * color.a : 0;
    }

    textureBytes.resize((textureWidth / imgSizeMult) * (textureHeight / imgSizeMult) * 4);

    stbir_resize_uint8(rawTextData.data(), textureWidth, textureHeight, 0, textureBytes.data(), textureWidth / imgSizeMult, textureHeight / imgSizeMult, 0, 4);
    textureWidth /= imgSizeMult;
    textureHeight /= imgSizeMult;

    for (int i = 0; i < textSize; i++)
    {
        characterLayouts[i].x /= imgSizeMult;
        characterLayouts[i].y /= imgSizeMult;
        characterLayouts[i].width /= imgSizeMult;
        characterLayouts[i].height /= imgSizeMult;
    }

    for (auto &chColor : characterColors)
        for (auto &i : chColor.characterIndices)
            if (i < textSize)
            {
                auto &layout = characterLayouts[i];
                for (int x = layout.x; x < layout.x + layout.width; x++)
                    for (int y = layout.y; y < layout.y + layout.height; y++)
                    {
                        int i = ((y * textureWidth) + x) * 4;
                        textureBytes[i] = 255 * chColor.color.r;
                        textureBytes[i + 1] = 255 * chColor.color.g;
                        textureBytes[i + 2] = 255 * chColor.color.b;
                        textureBytes[i + 3] *= chColor.color.a;
                    }
            }
}