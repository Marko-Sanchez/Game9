#include "scene/Game.h"

int main()
{
    Core::ApplicationSpecification appspec{"Game9"};
    Core::Game application(appspec);
    application.Run();

    return EXIT_SUCCESS;
}
