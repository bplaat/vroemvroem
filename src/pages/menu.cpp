// VroemVroem - Menu Page

#include "pages/menu.hpp"
#include "config.hpp"
#include "game.hpp"
#include "widgets/label.hpp"
#include "widgets/button.hpp"
#include "rect.hpp"
#include "fonts.hpp"
#include "pages/game.hpp"

// Windows includes
#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#endif

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

    int y = ((gameHeight - 32) - (128 + (64 + 16) * 2)) / 2;

    // Title
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

    // Menu buttons
    widgets.push_back(std::make_unique<Widgets::Button>(
        "Play",
        std::move(std::make_unique<Rect>(gameWidth / 4, y, gameWidth / 2, 64)),
        Fonts::getInstance()->getTextFont(),
        48,
        std::move(std::make_unique<Color>(0, 0, 0)),
        std::move(std::make_unique<Color>(255, 255, 255)),
        []() {
            Game::getInstance()->setPage(std::move(std::make_unique<GamePage>()));
        }
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

    // Version label
    char versionLabel[32];
    sprintf(versionLabel, "v%s", Config::version);

    widgets.push_back(std::make_unique<Widgets::Label>(
        versionLabel,
        std::move(std::make_unique<Rect>(gameWidth - 16 - 256, 16, 256, 32)),
        Fonts::getInstance()->getTextFont(),
        32,
        std::move(std::make_unique<Color>(255, 255, 255)),
        Widgets::Label::Align::RIGHT,
        nullptr,
        nullptr
    ));

    // Copyright label
    widgets.push_back(std::make_unique<Widgets::Label>(
        "Made by Bastiaan van der Plaat",
        std::move(std::make_unique<Rect>(0, gameHeight - 16 - 32, gameWidth, 32)),
        Fonts::getInstance()->getTextFont(),
        32,
        std::move(std::make_unique<Color>(255, 255, 255)),
        Widgets::Label::Align::CENTER,
        nullptr,
        []() {
            // Not cross platform code

            // Windows
            #ifdef _WIN32
            ShellExecute(0, 0, Config::gitRepoUrl, 0, 0, SW_SHOW);
            #endif

            // Linux
            #ifdef __unix__
            char command[256];
            sprintf(command, "xdg-open %s", Config::gitRepoUrl);
            system(command);
            #endif
        }
    ));
}

}
