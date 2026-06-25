/*
** EPITECH PROJECT, 2026
** Zappy-1
** File description:
** WindowTime
*/

#include "WindowTime.hpp"
#include "InfoPanel.hpp"
#include "WindowContext.hpp"

int WindowTime::getPresetCount()
{
    return TIME_FREQ_PRESETS.size();
}

int WindowTime::getSlowerFrequency(int freq)
{
    for (int index = getPresetCount() - 1; index >= 0; index--) {
        if (TIME_FREQ_PRESETS[index] < freq)
            return TIME_FREQ_PRESETS[index];
    }
    return TIME_FREQ_PRESETS[0];
}

int WindowTime::getFasterFrequency(int freq)
{
    for (int preset : TIME_FREQ_PRESETS) {
        if (preset > freq)
            return preset;
    }
    return TIME_FREQ_PRESETS[getPresetCount() - 1];
}

bool WindowTime::handleMousePressed(WindowContext& context, int mouseX,
    int mouseY)
{
    if (!isInsidePanel(context, mouseX, mouseY))
        return false;

    const sf::Vector2u windowSize = context.window.getSize();
    const sf::FloatRect timePanel = InfoPanelRenderer::getTimePanelRect(windowSize);
    const float panelX = timePanel.left;
    const float panelY = timePanel.top;
    const float buttonsY = panelY + 62.0f;
    const float buttonWidth = InfoPanelRenderer::TIME_BUTTON_WIDTH;
    const float buttonHeight = InfoPanelRenderer::TIME_BUTTON_HEIGHT;
    const float gap = 24.0f;
    const sf::FloatRect slowerButton(panelX + 16.0f, buttonsY, buttonWidth, buttonHeight);
    const sf::FloatRect pauseButton(panelX + 16.0f + buttonWidth + gap, buttonsY, buttonWidth, buttonHeight);
    const sf::FloatRect fasterButton(panelX + 16.0f + (buttonWidth + gap) * 2.0f, buttonsY, buttonWidth, buttonHeight);
    const float mx = static_cast<float>(mouseX);
    const float my = static_cast<float>(mouseY);

    if (pauseButton.contains(mx, my)) {
        if (context.time.paused) {
            context.time.serverFreq = std::clamp(context.time.resumeServerFreq,
                WindowContext::MIN_SERVER_FREQ,
                WindowContext::MAX_SERVER_FREQ);
            context.time.paused = false;
        } else {
            context.time.resumeServerFreq =
                context.time.serverFreq <= WindowContext::PAUSE_SERVER_FREQ ?
                WindowContext::NORMAL_SERVER_FREQ :
                std::clamp(context.time.serverFreq,
                    WindowContext::MIN_SERVER_FREQ,
                    WindowContext::MAX_SERVER_FREQ);
            context.time.serverFreq = WindowContext::PAUSE_SERVER_FREQ;
            context.time.paused = true;
        }
        context.time.pendingRequest = context.time.serverFreq;
        return true;
    }
    if (slowerButton.contains(mx, my)) {
        const int currentFreq = context.time.paused ?
            context.time.resumeServerFreq : context.time.serverFreq;

        context.time.serverFreq = getSlowerFrequency(currentFreq);
        context.time.resumeServerFreq =
            context.time.serverFreq <= WindowContext::PAUSE_SERVER_FREQ ?
            WindowContext::NORMAL_SERVER_FREQ : context.time.serverFreq;
        context.time.paused =
            context.time.serverFreq <= WindowContext::PAUSE_SERVER_FREQ;
        context.time.pendingRequest = context.time.serverFreq;
        return true;
    }
    if (fasterButton.contains(mx, my)) {
        const int currentFreq = context.time.paused ?
            context.time.resumeServerFreq : context.time.serverFreq;

        context.time.serverFreq = getFasterFrequency(currentFreq);
        context.time.resumeServerFreq = context.time.serverFreq;
        context.time.paused = false;
        context.time.pendingRequest = context.time.serverFreq;
        return true;
    }
    return true;
}

bool WindowTime::isInsidePanel(const WindowContext& context, int mouseX,
    int mouseY)
{
    const sf::Vector2u windowSize = context.window.getSize();
    const sf::FloatRect panel = InfoPanelRenderer::getTimePanelRect(windowSize);

    return panel.contains(static_cast<float>(mouseX), static_cast<float>(mouseY));
}

std::optional<int> WindowTime::consumeRequest(WindowContext& context)
{
    std::optional<int> request = context.time.pendingRequest;

    context.time.pendingRequest.reset();
    return request;
}

void WindowTime::setFrequency(WindowContext& context, int freq)
{
    context.time.serverFreq = std::clamp(freq, WindowContext::MIN_SERVER_FREQ, WindowContext::MAX_SERVER_FREQ);
    if (context.time.serverFreq <= WindowContext::PAUSE_SERVER_FREQ) {
        context.time.paused = true;
        if (context.time.resumeServerFreq <= WindowContext::PAUSE_SERVER_FREQ)
            context.time.resumeServerFreq = WindowContext::NORMAL_SERVER_FREQ;
    } else {
        context.time.paused = false;
        context.time.resumeServerFreq = context.time.serverFreq;
    }
}

int WindowTime::getFrequency(const WindowContext& context)
{
    return context.time.serverFreq;
}

bool WindowTime::isPaused(const WindowContext& context)
{
    return context.time.paused;
}
