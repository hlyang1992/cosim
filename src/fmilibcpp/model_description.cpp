#include "model_description.hpp"

namespace fmilibcpp {

void to_json(nlohmann::json& j, const default_experiment& obj) {
     j["startTime"] = obj.startTime;
     j["stopTime"] =  obj.stopTime;
     j["tolerance"] =  obj.tolerance;
     j["stepSize"] = obj.stepSize;
}

// inline void from_json(const nlohmann::json& j, default_experiment& obj) {
//     j.at("startTime").get_to(obj.startTime);
//     j.at("stopTime").get_to(obj.stopTime);
//     j.at("tolerance").get_to(obj.tolerance);
//     j.at("stepSize").get_to(obj.stepSize);
// }

void to_json(nlohmann::json& j, const model_description& obj)
{
    j["guid"] = obj.guid;
    j["author"] = obj.author;
    j["modelName"] = obj.modelName;
    j["modelIdentifier"] = obj.modelIdentifier;
    j["fmiVersion"] = obj.fmiVersion;
    j["description"] = obj.description;
    j["generationTool"] =  obj.generationTool;
    j["generationDateAndTime"] =  obj.generationDateAndTime;
    j["defaultExperiment"] =  obj.defaultExperiment;

    //j["modelVariables"] = obj.modelVariables;

    //j["modelVariables"] = nlohmann::json::array();

    //for (auto& mv : obj.modelVariables) {
    //    nlohmann::json mv_json = {};
    //    mv_json["name"] = mv.name;
    //    mv_json["causality"] = mv.causality.value_or(nullptr);
    //    mv_json["variability"] = mv.variability.value_or(nullptr);
    //    mv_json["valueReference"] = mv.vr;
    //    //mv_json["value"] = mv.typeAttributes;
    //    //j["modelVariables"].push_back(mv_json);
    //}

}
// inline void from_json(const nlohmann::json& j, model_description& obj) {
//     j.at("guid").get_to(obj.guid);
//     j.at("author").get_to(obj.author);
//     j.at("modelName").get_to(obj.modelName);
//     j.at("modelIdentifier").get_to(obj.modelIdentifier);
//     j.at("fmiVersion").get_to(obj.fmiVersion);
//     j.at("description").get_to(obj.description);
//     j.at("generationTool").get_to(obj.generationTool);
//     j.at("generationDateAndTime").get_to(obj.generationDateAndTime);
//     //j.at("modelVariables").get_to(obj.modelVariables);
//     //j.at("defaultExperiment").get_to(obj.defaultExperiment);
// }

} // namespace fmilibcpp
