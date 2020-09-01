#include <ConfigProxy.hpp>

using std::unordered_map;
using std::string;

ConfigProxy::ConfigProxy()
{
    SPDLOG_INFO("Request received to load config file");
    std::string config_path = static_cast<std::string>(getenv("BASE_DIR")) + "/config.yaml";
    config = YAML::LoadFile(config_path);
    SPDLOG_INFO("Config file Loaded ...");
}

ConfigProxy::ConfigProxy(const std::string &test_config)
{
    SPDLOG_INFO("Request received to load test config file");
    config = YAML::LoadFile(test_config);
    SPDLOG_INFO("Test Config file Loaded ...");
}

/** @TODO(abdullahrkw) fix empty input hack */
std::string ConfigProxy::get(const std::string& config_parameter) 
{   
    SPDLOG_INFO("get request received to read value of [" + config_parameter + "] from config file");
    if (config_parameter == "")
        return "?";
    std::string value =  config[config_parameter].as<std::string>();
    SPDLOG_INFO("value of [" + config_parameter + "] has been read from config file");
    return value;
}

std::string ConfigProxy::getString(const std::string& config_parameter) 
{   
    SPDLOG_INFO("getString request received to read value of [" + config_parameter + "] from config file");
    std::string value =  config[config_parameter].as<std::string>();
    SPDLOG_INFO("value of [" + config_parameter + "] has been read from config file");
    return value;

}

int ConfigProxy::getInt(const std::string& config_parameter) 
{   
    SPDLOG_INFO("getInt request received to read value of [" + config_parameter + "] from config file");
    int value =  config[config_parameter].as<int>();
    SPDLOG_INFO("value of [" + config_parameter + "] has been read from config file");
    return value;
}

float ConfigProxy::getFloat(const std::string& config_parameter) 
{   
    SPDLOG_INFO("getFloat request received to read value of [" + config_parameter + "] from config file");    
    float value =  config[config_parameter].as<float>();
    SPDLOG_INFO("value of [" + config_parameter + "] has been read from config file");
    return value;
}

double ConfigProxy::getDouble(const std::string& config_parameter) 
{   
    SPDLOG_INFO("getDouble request received to read value of [" + config_parameter + "] from config file");    
    double value =  config[config_parameter].as<double>();
    SPDLOG_INFO("value of [" + config_parameter + "] has been read from config file");
    return value;
}

bool ConfigProxy::getBool(const std::string& config_parameter)
{   
    SPDLOG_INFO("getBool request received to read value of [" + config_parameter + "] from config file");
    bool value =  config[config_parameter].as<bool>();
    SPDLOG_INFO("value of [" + config_parameter + "] has been read from config file");
    return value;
}
int ConfigProxy::getSequenceSize(const std::string& config_parameter) 
{   
    spdlog::debug("getSequenceSize request received to read value of [" + config_parameter + "] from config file");    
    assert(config[config_parameter].Type() == YAML::NodeType::Sequence);
    double size =  config[config_parameter].size();
    spdlog::debug("size of [" + config_parameter + "] has been read from config file");
    return size;
}
std::vector<std::string> ConfigProxy::getStringSequence(const std::string& config_parameter) 
{
    spdlog::debug("getStringSequence request received to read value of [" + config_parameter + "] from config file");    
    
    YAML::Node sequence = config[config_parameter];
    assert(sequence.IsSequence());
    double size =  sequence.size();
    // SPDLOG_INFO("value of [" + config_parameter + "] has been read from config file");
    std::vector<std::string> stringsequence;

    for (int i=0; i<size; i++)
    {
        stringsequence.push_back(sequence[i].as<std::string>());
    }
    SPDLOG_INFO("values of [" + config_parameter + "] has been read from config file");
    return stringsequence;
}
// std::vector<std::unordered_map<std::string, std::string>> ConfigProxy::getMap(const std::string& config_parameter)
// {
//     SPDLOG_INFO("getMap request received to read value of [" + config_parameter + "] from config file");
//     std::string map_data = config[config_parameter].as<std::string>();
//     std::vector<unordered_map<string, string>> _config_map_vec = Utility::generate_map(map_data);
//     SPDLOG_INFO("value of [" + config_parameter + "] has been read from config file");
//     return _config_map_vec;
// }

void ConfigProxy::set(const std::string& config_parameter, const std::string& value)
{
    SPDLOG_INFO("set request received to read value of [" + config_parameter + "] from config file");
    config[config_parameter] = value;
    SPDLOG_INFO("value of [" + value + "] has been updated");
   
}

void ConfigProxy::setInt(const std::string& config_parameter, int value)
{
    SPDLOG_INFO("set request received to read value of [" + config_parameter + "] from config file");
    config[config_parameter] = value;
    SPDLOG_INFO("value of {} has been updated to {}", config_parameter, value);
   
}

void ConfigProxy::setFloat(const std::string& config_parameter, float value)
{
    SPDLOG_INFO("set request received to read value of [" + config_parameter + "] from config file");
    config[config_parameter] = value;
    SPDLOG_INFO("value of {} has been updated to {}", config_parameter, value);
   
}



std::vector<std::string> ConfigProxy::getStringVector(const std::string& config_parameter)
{
   SPDLOG_INFO("set request received to read value of [" + config_parameter + "] from config file");
   std::vector<std::string> vec = config[config_parameter].as<std::vector<std::string>>();
   SPDLOG_INFO("value of [" + config_parameter + "] has been read from config file");
   return vec;
}

std::vector<float> ConfigProxy::getFloatVector(const std::string& config_parameter)
{
   SPDLOG_INFO("set request received to read value of [" + config_parameter + "] from config file");
   std::vector<float> vec = config[config_parameter].as<std::vector<float>>();
   SPDLOG_INFO("value of [" + config_parameter + "] has been read from config file");
   return vec;
}

// std::unordered_map<int, std::string> ConfigProxy::getClassMap(const std::string& config_parameter) 
// {
//     SPDLOG_INFO("getMap request received to read value of [" + config_parameter + "] from config file");
//     std::string class_map_data = config[config_parameter].as<std::string>();
//     unordered_map<int, string> _class_map = Utility::generate_ClassMap(class_map_data);
//     SPDLOG_INFO("value of [" + config_parameter + "] has been read from config file");
//     return _class_map;      
// }