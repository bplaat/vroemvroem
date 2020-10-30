// VroemVroem - Intro Page

#include "pages/intro.hpp"

namespace Pages {

IntroPage::IntroPage()
    : Page::Page() {}

bool IntroPage::handleEvent(const SDL_Event *event) {
    if (Page::handleEvent(event)) {
        return true;
    }

    //

    return false;
}

void IntroPage::createWidgets() {
    //
}

}
