#include "entity/TrainHandler.h"
#include <filesystem>
#include <print>

namespace Game9
{
// File-private helpers: parse filenames and map train types.
namespace
{
/*
* Convert string_view to TrainTypes enum.
*/
TrainTypes toTrainType(std::string_view type)
{
    if (type == "tram")      return TrainTypes::TRAM;
    if (type == "lightrail") return TrainTypes::LIGHTRAIL;
    if (type == "subway")    return TrainTypes::SUBWAY;
    if (type == "commuter")  return TrainTypes::COMMUTER;

    return TrainTypes::FREIGHT;// default
}

/*
* Parse filename into {trainName, trainType}.
*/
std::pair<std::string, std::string> parsefilename(const std::string& filename)
{
    auto dash = filename.find('-');
    if (dash == std::string::npos)
    {
        return {};
    }

    // type, name
    return {filename.substr(0, dash), filename.substr(dash + 1)};
}
} // unnamed namespace

/* Constructor.*/
TrainHandler::TrainHandler(std::shared_ptr<Core::util::Shader> shader):
m_sprite(shader),
m_jsonHandler("resources/gamedata/traindata.json")
{
    const size_t TOTAL_TEXTURES{2};
    const std::filesystem::path m_texturePaths[TOTAL_TEXTURES] =
    {
    "resources/images/trains/freight-greytrain.png",
    "resources/images/trains/freight-redtrain.png"
    };

    // Map trains to their types and load train textures.
    for (size_t i{0}; i < TOTAL_TEXTURES; ++i)
    {
        auto [trainType, trainName] = parsefilename(m_texturePaths[i].stem().string());
        if (trainType.empty() || trainName.empty())
        {
            continue;
        }

        m_trainIdentifier[trainName] = toTrainType(trainType);
        m_resourceManager.LoadTexture(m_texturePaths[i], trainName, static_cast<int>(i + 1));
    }
}

/*
 * Iterates m_trains and calls draw function of train class object.
 */
void TrainHandler::Draw()
{
    for (auto& [name, train]: m_trains)
    {
        m_sprite.DrawSprite(train.m_texture, train.m_position, train.m_size, train.m_rotation);
    }
}

/*
* Iterates m_trains and calls travel function of train class object, that handles object movement.
*/
void TrainHandler::Update(float deltaTime)
{
    for(auto& [name, train]: m_trains)
    {
        train.Travel(deltaTime);
    }
}

/*
* Loads train data: object name, train name, train type, and path.
*/
void TrainHandler::LoadPaths()
{
    // TODO: For testing purposes.
    constexpr int WINDOW_WIDTH{1024};
    constexpr int WINDOW_HEIGHT{1024};

    // Read file and load json data to member variable.
    if (auto ex = m_jsonHandler.Read(); !ex)
    {
        std::println(stderr, "{}", ex.error());
    }
    else
    {
        for (const auto& train: m_jsonHandler.m_jsonData["trains"])
        {
            std::vector<glm::vec2> path = train["path"];
            std::string objectName = train["name"].template get<std::string>();
            std::string trainName  = train["trainName"].template get<std::string>();

            // TODO: getting values for model: initial position, size, and velocity.
            m_trains.try_emplace(objectName, m_resourceManager.GetTexture(trainName), glm::vec2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2), glm::vec2(64, 64), glm::vec2(50, 50));
            m_trains[objectName].SetPath(path);
        }
    }
}

/*
* Train information is converted and saved onto a json file. Train object is created, trainName is used to obtain texture from already
* existing textures in ResourceManager.
*/
void TrainHandler::AddTrain(const std::string& name, std::string_view trainName, const std::vector<glm::vec2>& path)
{
    constexpr int WINDOW_WIDTH{1024};
    constexpr int WINDOW_HEIGHT{1024};

    if (m_trains.find(name) != m_trains.end() || m_trainIdentifier.find(trainName) == m_trainIdentifier.end())
    {
        return;
    }

    nlohmann::json train =
    {
        {"name", name},
        {"trainName", trainName},
        {"trainType", m_trainIdentifier[trainName]},
        {"path", path}
    };

    m_jsonHandler.m_jsonData["trains"].emplace_back(train);
    std::println("{}", m_jsonHandler.m_jsonData.dump(4));

    m_trains.try_emplace(name, m_resourceManager.GetTexture(trainName), glm::vec2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2), glm::vec2(64, 64), glm::vec2(50, 50));
}
}// namespace Game9
