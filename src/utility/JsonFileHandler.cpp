#include "utility/JsonFileHandler.h"

#include <print>
#include <cstdio>
#include <fstream>

namespace glm
{
void to_json(nlohmann::json& j, const glm::vec2& v)
{
    j = nlohmann::json::array({v.x, v.y});
}

void from_json(const nlohmann::json& j, glm::vec2& v)
{
    v.x = j.at(0).get<float>();
    v.y = j.at(1).get<float>();
}
}

namespace Game9
{
JsonFileHandler::JsonFileHandler(const std::filesystem::path& fileName):
m_jsonFileName(fileName)
{}

bool JsonFileHandler::Read()
{
    std::ifstream in{m_jsonFileName};
    if (!in.is_open())
    {
        return false;
    }

    try
    {
        m_jsonData = nlohmann::json::parse(in);
        if (m_jsonData.is_discarded() || m_jsonData.is_null())
        {
            std::println(stderr, "File is empty or null.");
            m_jsonData["trains"] = nlohmann::json::array();
        }

        return true;
    }
    catch(const nlohmann::json::parse_error& e)
    {
        m_jsonData = nullptr;

        std::println(stderr, "message: {}\nexception id: {}", e.what(), e.id);
        return false;
    }
}

bool JsonFileHandler::Write() const
{
    std::ofstream out{m_jsonFileName, std::ios::trunc};
    if (!out.is_open())
    {
        return false;
    }

    try
    {
        out << std::setw(4) << m_jsonData;
        return out.good();
    }
    catch(...)
    {
        std::println(stderr, "failed to write to json file");
        return false;
    }
}
}// namespace Game9
