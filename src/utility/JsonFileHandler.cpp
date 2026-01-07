#include "utility/JsonFileHandler.h"

#include <filesystem>
#include <format>
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

namespace Core::util
{
JsonFileHandler::JsonFileHandler(const std::filesystem::path& fileName):
m_jsonFileName(fileName)
{}

std::expected<void, std::string> JsonFileHandler::Read()
{
    try
    {
        std::ifstream in;
        in.exceptions(std::ios_base::badbit);
        in.open(m_jsonFileName);
        if (!in)
        {
            return std::unexpected(std::format("Failed to open file '{}'", m_jsonFileName.string()));
        }

        m_jsonData = nlohmann::json::parse(in);
        if (m_jsonData.is_null())
        {
            m_jsonData = nlohmann::json::object();
            m_jsonData["trains"] = nlohmann::json::array();
        }

    }
    catch (const std::ios_base::failure& e)
    {
        return std::unexpected(std::format("Failed to read file '{}' stream error: {}", m_jsonFileName.string(), e.what()));
    }
    catch (const nlohmann::json::parse_error& e)
    {
        m_jsonData = nullptr;
        return std::unexpected(std::format("Json parse error in '{}' message: {}", m_jsonFileName.string(), e.what()));
    }

    return {};
}

std::expected<void, std::string> JsonFileHandler::Write() const
{
    if (m_jsonData.is_null())
    {
        return std::unexpected(std::format("JSON data is empty - nothing to write to '{}'.", m_jsonFileName.string()));
    }

    auto tempFile = m_jsonFileName;
    tempFile.append(".tmp");

    try
    {

        std::ofstream out;
        out.exceptions(std::ios_base::failbit | std::ios_base::badbit);
        out.open(tempFile, std::ios_base::trunc);
        out << m_jsonData.dump(4);
        out.flush();

        std::filesystem::remove(m_jsonFileName);
        std::filesystem::rename(tempFile, m_jsonFileName);
    }
    catch (const std::filesystem::filesystem_error& e)
    {
        return std::unexpected(std::format("Failed to rename tmp file '{}' to '{}': {}", tempFile.string(), m_jsonFileName.string(), e.what()));
    }
    catch (const std::ios_base::failure& e)
    {
        return std::unexpected(std::format("Failed to write file '{}' stream error: {}", m_jsonFileName.string(), e.what()));
    }
    catch(const nlohmann::json::exception& e)
    {
        return std::unexpected(std::format("Error writing to json file '{}' error: {}", m_jsonFileName.string(), e.what()));
    }

    return {};
}
}// namespace Game9
