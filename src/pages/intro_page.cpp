// VroemVroem - Intro Page

#include "pages/intro_page.hpp"
#include "game.hpp"
#include "pages/menu_page.hpp"
#include "image.hpp"
#include "widgets/image_widget.hpp"
#include "widgets/label.hpp"
#include "fonts.hpp"
#include <iostream>

namespace Pages {

IntroPage::IntroPage()
    : Page::Page()
{
    backgroundColor = std::make_unique<Color>(255, 255, 255);

    createWidgets();

    timer = SDL_AddTimer(2000, IntroPage::timerCallback, nullptr);
    if (timer == 0) {
        std::cerr << "[ERROR] Can't create the SDL timer: " << SDL_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
}

bool IntroPage::handleEvent(const SDL_Event *event) {
    if (Page::handleEvent(event)) {
        return true;
    }

    if (event->type == SDL_USEREVENT) {
        SDL_RemoveTimer(timer);

        Game::getInstance()->setPage(std::move(std::make_unique<MenuPage>()));
    }

    if (event->type == SDL_MOUSEBUTTONUP) {
        SDL_RemoveTimer(timer);

        Game::getInstance()->setPage(std::move(std::make_unique<MenuPage>()));
    }

    if (event->type == SDL_KEYUP && event->key.keysym.sym == SDLK_ESCAPE) {
        SDL_RemoveTimer(timer);

        Game::getInstance()->setPage(std::move(std::make_unique<MenuPage>()));
    }

    return false;
}

void IntroPage::createWidgets() {
    Game *game = Game::getInstance(); // UGLY
    int gameWidth = game->getWidth();
    int gameHeight = game->getHeight();

    int y = (gameHeight - (256 + 32 + 72 + 16 + 64)) / 2;

    // Image
    std::unique_ptr<Image> logoImage = std::make_unique<Image>(game->getCanvas(), "assets/images/logo.png", false);
    widgets.push_back(std::make_unique<Widgets::ImageWidget>(
        std::move(logoImage),
        std::move(std::make_unique<Rect>(0, y, gameWidth, 256)),
        nullptr,
        nullptr
    ));

    y += 256 + 32;

    // title
    widgets.push_back(std::make_unique<Widgets::Label>(
        "Bassie Soft",
        std::move(std::make_unique<Rect>(0, y, gameWidth, 72)),
        Fonts::getInstance()->getTitleFont(),
        64,
        std::move(std::make_unique<Color>(0, 0, 0)),
        Widgets::Label::Align::CENTER,
        nullptr,
        nullptr
    ));

    y += 72 + 16;

    // Label
    widgets.push_back(std::make_unique<Widgets::Label>(
        "Presents a new highway simulator...",
        std::move(std::make_unique<Rect>(0, y, gameWidth, 64)),
        Fonts::getInstance()->getTextFont(),
        32,
        std::move(std::make_unique<Color>(0, 0, 0)),
        Widgets::Label::Align::CENTER,
        nullptr,
        nullptr
    ));
}

uint32_t IntroPage::timerCallback(uint32_t interval, void *param) {
    (void)param;

    SDL_UserEvent userevent;
    userevent.type = SDL_USEREVENT;
    userevent.code = 0;
    userevent.data1 = NULL;
    userevent.data2 = NULL;

    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);

    return interval;
}

}
