// VroemVroem - Game Page Header

#include "pages/page.hpp"
#include "world.hpp"
#include "camera.hpp"

namespace Pages {

class GamePage : public Page {
    private:
        std::shared_ptr<World> world;

        std::shared_ptr<Camera> camera;

    public:
        GamePage();

        void update(float delta);

        void createWidgets();
};

}
