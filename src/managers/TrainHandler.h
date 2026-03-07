#ifndef TRAIN_HANDLER_H
#define TRAIN_HANDLER_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <string_view>

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

#include <nlohmann/json.hpp>

#include "renderer/SpriteRenderer.h"
#include "entity/Train.h"

#include "utility/JsonFileHandler.h"
#include "managers/ResourceManager.h"

namespace Manager
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
    /* Maps train names to its type.*/
    std::unordered_map<std::string_view, TrainTypes> m_trainIdentifier;

    /* Train line and train objects.*/
    std::unordered_map<std::string, Entity::Train> m_trains;

    Renderer::SpriteRenderer m_sprite;

    /* Data loaded from json file containing train data.*/
    Utility::JsonFileHandler m_jsonHandler;

    /* Manage train textures. */
    Manager::ResourceManager m_resourceManager;

public:
    TrainHandler(std::shared_ptr<Renderer::Shader> shader);

    void Draw();
    void Update(float deltaTime);
    void UpdateProjection(const glm::mat4& projection);

    void LoadPaths();
    void AddTrain(const std::string& name, std::string_view trainName, const std::vector<glm::vec2>& path);
    void ExtendService(std::string_view trainName, const std::vector<glm::vec2>& path);
};
}// namespace Manager
#endif
