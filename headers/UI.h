#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Math.h"

class UIElement {
public:
    Vec2 position {0.0f, 0.0f};
    Vec2 scale    {1.0f, 1.0f};
    float rotation = 0.0f;

    UIElement* parent = nullptr;
    std::vector<std::unique_ptr<UIElement>> children;

    virtual ~UIElement() = default;

    void AddChild(std::unique_ptr<UIElement> child) {
        child->parent = this;
        children.push_back(std::move(child));
    }
};

class UIImage : public UIElement {
public:
    std::string texturePath;

    UIImage(const std::string& texture = "")
        : texturePath(texture) {}
};

class UIText : public UIElement {
public:
    std::string text;
    float fontSize = 16.0f;

    UIText(const std::string& value = "")
        : text(value) {}
};

class UIPanel : public UIElement {
public:
    Vec2 size {100.0f, 100.0f};
    std::string backgroundTexture;

    UIPanel(const Vec2& panelSize = {100, 100})
        : size(panelSize) {}
};

class UI {
public:
    std::vector<std::unique_ptr<UIElement>> roots;

    UIPanel* CreatePanel(const Vec2& size) {
        auto panel = std::make_unique<UIPanel>(size);
        UIPanel* ptr = panel.get();
        roots.push_back(std::move(panel));
        return ptr;
    }
};
