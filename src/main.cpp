#include "scene/Game.h"

#include "scene/BackgroundLayer.h"
#include "scene/TrainLayer.h"

int main()
{
    Core::ApplicationSpecification appspec{"Game9"};
    Core::Game application(appspec);
    application.PushLayer<Game9::BackgroundLayer>(application.GetWindow());
    application.PushLayer<Game9::TrainLayer>(application.GetWindow());
    application.Run();

    return EXIT_SUCCESS;
}
