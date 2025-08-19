#include "entity/TrainHandler.h"
#include <iostream>
#include <fstream>

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
TrainHandler::TrainHandler()
{
    m_texturePaths =
    {
    "resources/images/trains/freight-greytrain.png",
    "resources/images/trains/freight-redtrain.png"
    };

    // Map trains to their types and load train textures.
    for (auto i{0}; i < m_texturePaths.size(); ++i)
    {
        auto [trainName, trainType] = parsefilename(m_texturePaths[i]);
        if (!trainName.empty() && !trainType.empty())
        {
            m_trainIdentifier[trainName] = toTrainType(trainType);
            // TODO: in the future all trains will be in a single texture file.
            ResourceManager::LoadTexture(m_texturePaths[i], trainName, i + 1);
        }
    }
}

/* Destructor.*/
TrainHandler::~TrainHandler()
{
    if (!jsonData.is_null())
        std::ofstream("resources/gamedata/traindata.json", std::ios::trunc) << std::setw(4) << jsonData;
}

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
* Load path of trains from file. File is organized as such:
*
* <name?> <train-type> <path>:{x_1,y_1} {x_2, y_2} {x_3, y_3}
* name: is an arbritary name provided by the user.
* train-type: train type represented in all lower-case from the enum class TrainTypes.
* path: pairs of floats representing the x and y cordinates that the train will travel between.
*/
void TrainHandler::LoadPaths(const std::string_view filePath)
{
    // TODO: For testing purposes.
    // Current Window height and width.
    constexpr int WINDOW_WIDTH{1024};
    constexpr int WINDOW_HEIGHT{1024};

    std::ifstream jsonFile("resources/gamedata/traindata.json");
    if (!jsonFile.is_open())
    {
        std::cerr << "Failed to open file: traindata.json" << std::endl;
        return;
    }

    jsonData = nlohmann::json::parse(jsonFile);
    if (jsonData.is_discarded() || jsonData.is_null())
    {
        jsonData["trains"] = nlohmann::json::array();
    }
    else
    {
        for (const auto& train: jsonData["trains"])
        {
            std::cout << "-----------ITEM----------------" << std::endl;
            std::cout << std::setw(4) << train << std::endl;

            std::vector<glm::vec2> path;
            for (const auto& pairs: train["path"].template get<std::vector<std::pair<float, float>>>())
                path.emplace_back(pairs.first, pairs.second);

            // TODO: getting values for model: initial position, size, and velocity.
            m_trains.try_emplace(train["name"], ResourceManager::GetTexture(train["trainName"].template get<std::string>()).value(), glm::vec2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2), glm::vec2(64, 64), glm::vec2(50, 50));
            m_trains[train["name"]].SetPath(path);
        }

    }
    jsonFile.close();
}

/*
* Train information is converted and saved onto a json file. Train object is created, trainName is used to obtain texture from already
* existing textures in ResourceManager.
*/
void TrainHandler::AddTrain(const std::string& name, std::string_view trainName, const std::vector<glm::vec2>& path)
{
    // name already exist, must be unique.
    if (m_trains.find(name) != m_trains.end())
    {
        return;
    }

    // TODO:
    // opening and closing files takes a bit, multithread ?
    // easier way to add array of pairs ?
    // Use Load Paths().
    nlohmann::json train =
    {
        {"name", name},
        {"trainName", trainName},
        {"trainType", m_trainIdentifier[trainName]}
    };

    nlohmann::json arr = nlohmann::json::array();
    for (const auto& coordinates: path)
    {
        arr += {coordinates.x, coordinates.y};
    }

    train["path"] = arr;
    jsonData["trains"] += train;
    std::cout << std::setw(4) << jsonData << std::endl;

    //m_trains.try_emplace(name, ResourceManager::GetTexture(trainName).value(),/* position, size, velocity*/);
}

}// namespace Game9
