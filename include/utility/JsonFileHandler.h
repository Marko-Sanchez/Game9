#ifndef JSON_FILE_HANDLER
#define JSON_FILE_HANDLER

#include <expected>
#include <filesystem>

#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

namespace glm
{
    /* Json GLM Adapters.*/
    void to_json(nlohmann::json& j, const glm::vec2& v);
    void from_json(const nlohmann::json& j, glm::vec2& v);
}

namespace Core::util
{
/*
 * Creates a json object from a json file.
 */
struct JsonFileHandler
{
    std::filesystem::path m_jsonFileName;
    nlohmann::json m_jsonData;

    JsonFileHandler(const std::filesystem::path& fileName);

    std::expected<void, std::string> Read();
    std::expected<void, std::string> Write() const;
};
}// namespace Core::util
#endif
