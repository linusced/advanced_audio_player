#include "lc_gui/basic_elements/text_element.hpp"

lc_gui::TextElement::TextElement(TextRenderer::Font *font, std::string text, const std::string &className, bool allowLineWrap, bool continuousThread, bool shrinkOnOverflow)
    : Element(className), font(font), text(text), allowLineWrap(allowLineWrap), continuousThread(continuousThread), shrinkOnOverflow(shrinkOnOverflow)
{
    textImg = std::make_unique<Texture>();

    mesh.vertices.resize(4);
    mesh.vertices[0].textureCoords = {0, 1};
    mesh.vertices[1].textureCoords = {1, 1};
    mesh.vertices[2].textureCoords = {1, 0};
    mesh.vertices[3].textureCoords = {0, 0};

    mesh.indices = {0, 1, 2, 0, 2, 3};
    mesh.texture = textImg.get();
}
lc_gui::TextElement::~TextElement()
{
    if (thread)
        thread->join();
}

void lc_gui::TextElement::update()
{
    if (continuousThread && createdTextData)
    {
        styleChange = true;
        bottomY = textRenderer.textureHeight;
    }
    else if (thread && createdTextData)
    {
        thread->join();
        thread.reset();
        createdTextData = false;

        styleChange = true;
        bottomY = textRenderer.textureHeight;
    }
}
void lc_gui::TextElement::resizeMesh(const Style &style, const Layout &layout, const glm::ivec2 &windowSize)
{
    bool debug = classList.contains("text-info");

    glm::ivec2 size = {layout.width, layout.height};

    glm::vec4 color = style.colorProperties.at("color").isSet ? style.colorProperties.at("color").value : glm::vec4(1);
    int fontSize = style.sizeProperties.at("font-size").isSet ? style.getPixelSize(style.sizeProperties.at("font-size"), size, windowSize, 1) : 20;

    if (layout.width != prevLayout.width || fontSize != prevFontSize)
    {
        prevFontSize = fontSize;
        maxWidth = layout.width;
        prevColor = color;

        if (!continuousThread)
        {
            if (thread)
                thread->join();

            prevFontSize = fontSize;
            maxWidth = layout.width;
            thread = std::make_unique<std::thread>(renderText, this);
        }
        else if (!thread)
        {
            runContinuousThread = true;
            createdTextData = true;
            thread = std::make_unique<std::thread>(continuousThread_func, this);
        }

        mesh.applyVertexChange();
        return;
    }
    if (threadIsActive())
        return;

    outputTextureBytes = textRenderer.textureBytes;
    for (unsigned long i = 0; i < outputTextureBytes.size(); i += 4)
    {
        outputTextureBytes[i] = 255 * color.r;
        outputTextureBytes[i + 1] = 255 * color.g;
        outputTextureBytes[i + 2] = 255 * color.b;
    }
    prevColor = color;

    prevFontSize = fontSize;
    prevColor = color;
    bottomY = textRenderer.textureHeight;

    continuousThreadReady = true;

    if (!allowLineWrap && cursorPos != -1)
    {
        int charX = 0;
        for (int i = 0; i < cursorPos; i++)
            charX += textRenderer.characterLayouts[i].width;

        if (charX <= 0)
            charX = 1;
        else if (charX >= textRenderer.textureWidth)
            charX = textRenderer.textureWidth - 1;

        for (int y = 0; y < textRenderer.textureHeight; y++)
            for (int x = charX - 1; x < charX + 1; x++)
            {
                outputTextureBytes[(y * textRenderer.textureWidth + x) * 4] = color.r * 255;
                outputTextureBytes[(y * textRenderer.textureWidth + x) * 4 + 1] = color.g * 255;
                outputTextureBytes[(y * textRenderer.textureWidth + x) * 4 + 2] = color.b * 255;
                outputTextureBytes[(y * textRenderer.textureWidth + x) * 4 + 3] = color.a * 255;
            }
    }
    int outputWidth = textRenderer.textureWidth, outputHeight = textRenderer.textureHeight;

    mesh.texture->update(outputTextureBytes, outputWidth, outputHeight, GL_RGBA);

    if (!allowLineWrap && shrinkOnOverflow && outputWidth > layout.width)
    {
        outputWidth = layout.width;
        outputHeight *= layout.width / (float)textRenderer.textureWidth;
    }

    int textOffsetX = (layout.width - outputWidth) / 2, textOffsetY = (layout.height - outputHeight) / 2;

    const auto &textAlign = style.miscProperties.at("text-align");
    if (textAlign.isSet)
    {
        if (textAlign.value.find("left") != std::string::npos)
            textOffsetX = 0;
        else if (textAlign.value.find("right") != std::string::npos)
            textOffsetX = layout.width - outputWidth;

        if (textAlign.value.find("top") != std::string::npos)
            textOffsetY = 0;
        else if (textAlign.value.find("bottom") != std::string::npos)
            textOffsetY = layout.height - outputHeight;
    }

    if (outputWidth > layout.width)
        textOffsetX = 0;
    if (outputHeight > layout.height)
        textOffsetY = 0;

    mesh.vertices[0].position = {layout.x + textOffsetX, layout.y + textOffsetY};
    mesh.vertices[1].position = {layout.x + textOffsetX + outputWidth, layout.y + textOffsetY};
    mesh.vertices[2].position = {layout.x + textOffsetX + outputWidth, layout.y + textOffsetY + outputHeight};
    mesh.vertices[3].position = {layout.x + textOffsetX, layout.y + textOffsetY + outputHeight};

    for (auto &v : mesh.vertices)
        v.position /= windowSize;

    mesh.applyVertexChange();
}

const std::string &lc_gui::TextElement::getText() const
{
    return text;
}
void lc_gui::TextElement::setText(std::string text)
{
    if (text == this->text)
        return;

    this->text = text;

    if (continuousThread)
    {
        std::unique_lock<std::mutex> ul(threadBlockingMutex);
        threadBlockingCV.notify_one();
    }
    else
    {
        if (thread)
            thread->join();

        createdTextData = false;

        thread = std::make_unique<std::thread>(renderText, this);
    }
}

void lc_gui::TextElement::updateCursorPos(int cursorPos)
{
    this->cursorPos = cursorPos;
    styleChange = true;
}

bool lc_gui::TextElement::threadIsActive() const
{
    return (!continuousThread && thread) || (continuousThread && !createdTextData);
}
void lc_gui::TextElement::stopThread()
{
    if (continuousThread)
    {
        runContinuousThread = false;
        std::unique_lock<std::mutex> ul(threadBlockingMutex);
        threadBlockingCV.notify_one();
    }
}

const int &lc_gui::TextElement::getLineLayoutY(int lineIndex) const
{
    return textRenderer.lineLayouts[lineIndex].y;
}
const int &lc_gui::TextElement::getLineLayoutHeight(int lineIndex) const
{
    return textRenderer.lineLayouts[lineIndex].height;
}

void lc_gui::TextElement::renderText(TextElement *instance)
{
    instance->createdTextData = false;

    instance->textRenderer.render(instance->text, instance->prevColor, instance->prevFontSize, instance->allowLineWrap ? instance->maxWidth : -1, instance->font);

    instance->createdTextData = true;
}

void lc_gui::TextElement::continuousThread_func(TextElement *instance)
{
    std::string prevText;
    double prevChangeTime = glfwGetTime();

    while (instance->runContinuousThread)
    {
        if (instance->text != prevText && instance->continuousThreadReady)
        {
            instance->continuousThreadReady = false;

            renderText(instance);

            prevText = instance->text;
            instance->continuousThreadReady = false;
            prevChangeTime = glfwGetTime();
        }
        else if (glfwGetTime() - prevChangeTime > 0.5)
        {
            std::unique_lock<std::mutex> ul(instance->threadBlockingMutex);
            instance->threadBlockingCV.wait(ul);
        }
    }
}