// VroemVroem - Menu Page

#include "pages/menu.hpp"
#include "game.hpp"
#include "widgets/label.hpp"
#include "widgets/button.hpp"
#include "rect.hpp"
#include "fonts.hpp"

namespace Pages {

MenuPage::MenuPage()
    : Page::Page()
{
    createWidgets();
}

void MenuPage::createWidgets() {
    Game *game = Game::getInstance(); // UGLY
    int gameWidth = game->getWidth();
    int gameHeight = game->getHeight();

    int y = (gameHeight - (128 + (64 + 16) * 2)) / 2;

    widgets.push_back(std::make_unique<Widgets::Label>(
        "Vroem Vroem",
        std::move(std::make_unique<Rect>(0, y, gameWidth, 128)),
        Fonts::getInstance()->getTitleFont(),
        64,
        std::move(std::make_unique<Color>(255, 255, 255)),
        Widgets::Label::Align::CENTER,
        nullptr,
        nullptr
    ));

    y += 128;

    widgets.push_back(std::make_unique<Widgets::Button>(
        "Play",
        std::move(std::make_unique<Rect>(gameWidth / 4, y, gameWidth / 2, 64)),
        Fonts::getInstance()->getTextFont(),
        48,
        std::move(std::make_unique<Color>(0, 0, 0)),
        std::move(std::make_unique<Color>(255, 255, 255)),
        nullptr
    ));

    y += 64 + 16;

    widgets.push_back(std::make_unique<Widgets::Button>(
        "Exit",
        std::move(std::make_unique<Rect>(gameWidth / 4, y, gameWidth / 2, 64)),
        Fonts::getInstance()->getTextFont(),
        48,
        std::move(std::make_unique<Color>(0, 0, 0)),
        std::move(std::make_unique<Color>(255, 255, 255)),
        []() {
            Game::getInstance()->stop();
        }
    ));
}

}
