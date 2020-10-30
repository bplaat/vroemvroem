// VroemVroem - Intro Page Header

#include "pages/page.hpp"
#include <SDL2/SDL.h>

namespace Pages {

class IntroPage : public Page {
    public:
        IntroPage();

        bool handleEvent(const SDL_Event *event);

        void createWidgets();
};

}
