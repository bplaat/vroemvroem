// VroemVroem - Game Page

#include "pages/game_page.hpp"
#include "game.hpp"
#include "widgets/viewport.hpp"
#include "widgets/button.hpp"
#include "fonts.hpp"
#include "pages/menu_page.hpp"

namespace Pages {

GamePage::GamePage()
    : Page::Page()
{
    world = std::make_shared<World>(1, 512, 512);

    camera = std::make_shared<Camera>(static_cast<float>(world->getWidth()) / 2, static_cast<float>(world->getHeight()) / 2,
        world->getWidth(), world->getHeight(), 2);

    createWidgets();
}

void GamePage::createWidgets() {
    Game *game = Game::getInstance(); // UGLY
    int gameWidth = game->getWidth();
    int gameHeight = game->getHeight();

    // Create world viewport
    widgets.push_back(std::make_unique<Widgets::Viewport>(
        world,
        camera,
        std::move(std::make_unique<Rect>(0, 0, gameWidth, gameHeight)),
        std::move(std::make_unique<Color>(17, 17, 17))
    ));

    // Create back button
    widgets.push_back(std::make_unique<Widgets::Button>(
        "Back",
        std::move(std::make_unique<Rect>(gameWidth - 128 - 16, 16, 128, 48)),
        Fonts::getInstance()->getTextFont(),
        32,
        std::move(std::make_unique<Color>(0, 0, 0)),
        std::move(std::make_unique<Color>(255, 255, 255)),
        []() {
            Game::getInstance()->setPage(std::move(std::make_unique<MenuPage>()));
        }
    ));
}

void GamePage::update(float delta) {
    world->update(delta);
}

}
