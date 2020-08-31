#ifndef I_ICONFIG_HEADER_
#define I_ICONFIG_HEADER_

#include <string>
#include <unordered_map>
#include <vector>

/**
* @brief Configuration object which reads and provides config.
*
* @details Responsible for providing information which will
* be used to configure the right classes for initialization
*/
class IConfig {
    public:
        IConfig() {};
        virtual ~IConfig() {};
        virtual std::string get(const std::string& key) = 0;
        virtual std::string getString(const std::string& key) = 0;
        virtual int getInt(const std::string& key) = 0;
        virtual float getFloat(const std::string& key) = 0;
        virtual double getDouble(const std::string& key) = 0;
        virtual bool getBool(const std::string& key) = 0;
        virtual std::vector<std::string> getStringVector(const std::string& key) = 0;
        virtual std::vector<float> getFloatVector(const std::string& key) = 0;
        virtual std::vector<std::unordered_map<std::string, std::string>> getMap(const std::string& key) = 0;
        virtual std::unordered_map<int, std::string> getClassMap(const std::string& key) = 0;
        virtual void set(const std::string& config_parameter, const std::string& value) = 0;
        virtual void setInt(const std::string& config_parameter, int value) = 0;
        virtual void setFloat(const std::string& config_parameter, float value) = 0;
};
#endif