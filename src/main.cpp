#include "game/Game.h"

#include "scene/OceanMap.h"

int main()
{
    Core::ApplicationSpecification appspec{"Game9"};
    Core::Game application(appspec);
    application.PushLayer<OceanMap::OceanMapComposite>();
    application.Run();

    return EXIT_SUCCESS;
}
