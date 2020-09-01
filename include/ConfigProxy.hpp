#ifndef _CONFIGPROXY_HEADER_
#define _CONFIGPROXY_HEADER_

#include "assert.h"
#include <exception>
#include <iostream>
#include <string>
#include <stdexcept>
#include <unordered_map>
#include <vector>

#include "spdlog/spdlog.h"
#include "yaml-cpp/yaml.h"

#include "IConfig.hpp"

/**
* @brief Configuration object which reads and provides config.
*
* @details Responsible for providing information which will
* be used to configure the right classes for initialization
*/
class ConfigProxy : public IConfig
{
    public:
        ConfigProxy();
        ConfigProxy(const std::string &test_config);
        std::string get(const std::string& config_parameter);
        std::string getString(const std::string& config_parameter);
        int getInt(const std::string& config_parameter);
        bool getBool(const std::string& config_parameter);
        double getDouble(const std::string& config_parameter);
        std::vector<std::string> getStringVector(const std::string& config_parameter);
        std::vector<float> getFloatVector(const std::string& config_parameter);
        float getFloat(const std::string& config_parameter);
        std::vector<std::string> getStringSequence(const std::string& config_parameter);
        int getSequenceSize(const std::string& config_parameter);


        /**
         * @param config_parameter: key in configuration file
         * @return list of maps.
         */
        // std::vector<std::unordered_map<std::string, std::string>> getMap(const std::string& config_parameter);
        // std::unordered_map<int, std::string> getClassMap(const std::string& config_parameter);
        void set(const std::string& config_parameter, const std::string& value);
        void setInt(const std::string& config_parameter, int value);
        void setFloat(const std::string& config_parameter, float value);
    private:
        YAML::Node config;
    };
#endif
