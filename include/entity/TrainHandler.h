#ifndef TRAIN_HANDLER_H
#define TRAIN_HANDLER_H

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

#include <nlohmann/json.hpp>

#include "entity/Train.h"
#include "utility/ResourceManager.h"
#include "utility/SpriteRenderer.h"
#include "utility/JsonFileHandler.h"

#include <array>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string_view>

namespace Game9
{

/* All the possible train types currently available.*/
enum class TrainTypes
{
    UNDEFINED,
    TRAM,
    LIGHTRAIL,
    SUBWAY,
    COMMUTER,
    FREIGHT
};

// Specialized enum conversion, instead of converting enum to an integer value converts to string.
NLOHMANN_JSON_SERIALIZE_ENUM(TrainTypes,
                             {
                             {TrainTypes::UNDEFINED, "undefined"},
                             {TrainTypes::TRAM, "tram"},
                             {TrainTypes::LIGHTRAIL, "lightrail"},
                             {TrainTypes::SUBWAY, "subway"},
                             {TrainTypes::COMMUTER, "commuter"},
                             {TrainTypes::FREIGHT, "freight"},
                             })

class TrainHandler
{
private:
    /* File path to textures.*/
    std::array<std::string_view, 2 /*TOTAL_TEXTURES*/> m_texturePaths;

    /* Maps train names to its type.*/
    std::unordered_map<std::string_view, TrainTypes> m_trainIdentifier;

    /* Train line and train objects.*/
    std::unordered_map<std::string, Train> m_trains;

    /* Data loaded from json file containing train data.*/
    JsonFileHandler m_jsonHandler;

    /* Manage train textures. */
    Core::util::ResourceManager m_resourceManager;

public:
    TrainHandler();
    ~TrainHandler();

    void Draw(std::shared_ptr<SpriteRenderer> rend);
    void Update(float deltaTime);
    void LoadPaths();
    void AddTrain(const std::string& name, std::string_view trainName, const std::vector<glm::vec2>& path);
    void ExtendService(std::string_view trainName, const std::vector<glm::vec2>& path);
};
}// namespace Game9
#endif
