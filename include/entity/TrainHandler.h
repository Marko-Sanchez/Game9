#ifndef TRAIN_HANDLER_H
#define TRAIN_HANDLER_H

#include <glm/glm.hpp>

#include "entity/Train.h"
#include "utility/ResourceManager.h"

#include <array>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>
#include <string_view>

namespace Game9
{

/* All the possible train types currently available.*/
enum class TrainTypes
{
    TRAM,
    LIGHTRAIL,
    SUBWAY,
    COMMUTER,
    FREIGHT
};

class TrainHandler
{
private:
    /* File path to textures.*/
    std::array<std::string_view, 2 /*TOTAL_TEXTURES*/> m_texturePaths;

    /* Maps train names to its type.*/
    std::unordered_map<std::string, TrainTypes> m_trainIdentifier;

    /* Trains.*/
    std::vector<Train> m_trains;
public:
    TrainHandler();
    ~TrainHandler();

    void Draw();
    void LoadPaths();
    void AddPath();
    /*
    * AddPaths()? reads from file iostream >> '{1280, 1280}, {...}'
    * -- file --
    * train-type {...}
    */
};
}// namespace Game9
#endif
