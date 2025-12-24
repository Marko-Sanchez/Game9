#include "scene/BackgroundLayer.h"
#include "scene/Game.h"

int main()
{
    Core::ApplicationSpecification appspec{"Game9"};
    Core::Game application(appspec);
    application.PushLayer<Game9::BackgroundLayer>(application.GetWindow());
    application.Run();

    return EXIT_SUCCESS;
}
