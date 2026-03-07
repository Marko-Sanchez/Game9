#include "core/Game.h"

#include "layers/BackgroundLayer.h"
#include "layers/TrainLayer.h"

int main()
{
    Core::ApplicationSpecification appspec{"Game9"};
    Core::Game application(appspec);
    application.PushLayer<Layer::BackgroundLayer>(application.GetWindow());
    application.PushLayer<Layer::TrainLayer>(application.GetWindow());
    application.Run();

    return EXIT_SUCCESS;
}
