#ifndef JSON_FILE_HANDLER
#define JSON_FILE_HANDLER
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

#include <filesystem>
namespace glm
{
    /* Json GLM Adapters.*/
    void to_json(nlohmann::json& j, const glm::vec2& v);
    void from_json(const nlohmann::json& j, glm::vec2& v);
}

namespace Game9
{
struct JsonFileHandler
{
    std::filesystem::path m_jsonFileName;
    nlohmann::json m_jsonData;

    JsonFileHandler(const std::filesystem::path& fileName);

    // read from file.
    bool Read();
    // write to file.
    bool Write() const;
};
}// namespace Game9
#endif
