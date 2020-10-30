// VroemVroem - Game Page

#include "pages/game.hpp"

namespace Pages {

GamePage::GamePage()
    : Page::Page() {};

}


    // // Generate world
    // world = std::make_shared<World>(1, 512, 512);

    // camera = std::make_unique<Camera>(static_cast<float>(world->getWidth()) / 2, static_cast<float>(world->getHeight()) / 2,
    //     world->getWidth(), world->getHeight(), 2);

    // // Send all events to camera
    // camera->handleEvent(event);


// // Clear screen
    // SDL_SetRenderDrawColor(renderer, 17, 17, 17, 255);
    // SDL_RenderClear(renderer);

    // // Draw world with camera
    // world->draw(canvas, camera.get());

    // int tileSize = Camera::zoomLevels[camera->getZoom()];

    // // Draw debug label
    // char debugLabel[128];
    // sprintf(debugLabel, "camera.x = %.02f, camera.y = %.02f, camera.tileSize = %d", camera->getX(), camera->getY(), tileSize);

    // std::unique_ptr<Image> debugLabelImage = Fonts::getInstance()->getTextFont()->render(canvas, debugLabel, 32, &Color::WHITE);
    // Rect debugLabelRect = { 16, 16, debugLabelImage->getWidth(), debugLabelImage->getHeight() };
    // debugLabelImage->draw(&debugLabelRect);
