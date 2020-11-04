// VroemVroem - Intro Page Header

#include "pages/page.hpp"
#include <SDL2/SDL.h>

namespace Pages {

class IntroPage : public Page {
    private:
        static const int timerEventCode = 1;

        SDL_TimerID timer;

    public:
        IntroPage();

        bool handleEvent(const SDL_Event *event);

        void createWidgets();
};

}
