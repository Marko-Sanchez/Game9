#include "ui/Layer.h"

namespace Core
{
// On next render this layer, current layer, is swapped with newLayer.
void Layer::QueueTransition(std::unique_ptr<Layer> newLayer)
{
    // Iterate through a list of layers, replacing 'this', callee, layer with new layer.
}
}
