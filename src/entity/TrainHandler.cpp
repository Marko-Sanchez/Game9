#include "entity/TrainHandler.h"
#include <iostream>

namespace Game9
{
// File-private helpers: parse filenames and map train types.
namespace
{
constexpr std::pair<std::string_view, TrainTypes> typeTable [] =
{
    {"tram",      TrainTypes::TRAM},
    {"lightrail", TrainTypes::LIGHTRAIL},
    {"subway",    TrainTypes::SUBWAY},
    {"commuter",  TrainTypes::COMMUTER},
    {"freight",   TrainTypes::FREIGHT}
};

/*
* Convert string_view to TrainTypes enum.
*/
constexpr TrainTypes toTrainType(std::string_view type)
{
    for (auto&& [name, value]: typeTable)
    {
        if (name == type) return value;
    }

    return TrainTypes::FREIGHT;
}

/*
* Parse filename into {trainName, trainType}.
*/
constexpr std::pair<std::string_view, std::string_view> parsefilename(std::string_view texturePath)
{
    auto lastSlash = texturePath.find_last_of('/');
    auto hyphenPos = texturePath.find('-', lastSlash != std::string_view::npos ? lastSlash : 0);
    auto dotPos    = texturePath.find('.', hyphenPos != std::string_view::npos ? hyphenPos : 0);

    if (lastSlash == std::string_view::npos ||
        hyphenPos == std::string_view::npos ||
        dotPos    == std::string_view::npos)
    {
        return {"", ""};
    }

    std::string_view trainType{texturePath.substr(lastSlash + 1, hyphenPos - (lastSlash + 1))};
    std::string_view trainName{texturePath.substr(hyphenPos + 1, dotPos - (hyphenPos + 1))};

    return {trainName, trainType};
}
} // unnamed namespace

/* Constructor.*/
TrainHandler::TrainHandler():
m_jsonHandler("resources/gamedata/traindata.json")
{
    m_texturePaths =
    {
    "resources/images/trains/freight-greytrain.png",
    "resources/images/trains/freight-redtrain.png"
    };

    // Map trains to their types and load train textures.
    for (size_t i{0}; i < m_texturePaths.size(); ++i)
    {
        auto [trainName, trainType] = parsefilename(m_texturePaths[i]);
        if (!trainName.empty() && !trainType.empty())
        {
            m_trainIdentifier[trainName] = toTrainType(trainType);
            // TODO: in the future all trains will be in a single texture file.
            m_resourceManager.LoadTexture(m_texturePaths[i], trainName, i + 1);
        }
    }
}

/* Destructor.*/
TrainHandler::~TrainHandler()
{}

/*
 * Iterates m_trains and calls draw function of train class object.
 */
void TrainHandler::Draw(std::shared_ptr<SpriteRenderer> rend)
{
    for (auto& [name, train]: m_trains)
    {
        train.Draw(rend);
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
    m_jsonHandler.Read();
    for (const auto& train: m_jsonHandler.m_jsonData["trains"])
    {
        std::cout << "-----------ITEM----------------" << std::endl;
        std::cout << std::setw(4) << train << std::endl;

        std::vector<glm::vec2> path = train["path"];
        std::string objectName = train["name"].template get<std::string>();
        std::string trainName  = train["trainName"].template get<std::string>();

        // TODO: getting values for model: initial position, size, and velocity.
        m_trains.try_emplace(objectName, m_resourceManager.GetTexture(trainName), glm::vec2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2), glm::vec2(64, 64), glm::vec2(50, 50));
        m_trains[objectName].SetPath(path);
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
    std::cout << std::setw(4) << m_jsonHandler.m_jsonData << std::endl;

    m_trains.try_emplace(name, m_resourceManager.GetTexture(trainName), glm::vec2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2), glm::vec2(64, 64), glm::vec2(50, 50));
}
}// namespace Game9
