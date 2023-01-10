//
// Created by lucas on 20/10/22.
//

#include "../include/PropertiesEndpointConfiguration.h"

using namespace cppproperties;
using namespace std;

namespace cloudio {
    PropertiesEndpointConfiguration::PropertiesEndpointConfiguration(const string &propertiesFilePath) {
        this->properties = PropertiesParser::Read(propertiesFilePath);
    }

    PropertiesEndpointConfiguration::~PropertiesEndpointConfiguration() {

    }

    string PropertiesEndpointConfiguration::getProperty(const string &key) {
        return this->getProperty(key, "");
    }

    string PropertiesEndpointConfiguration::getProperty(const string &key, const string &defaultValue) {
        string outputProperty;
        try {
            outputProperty = properties.GetProperty(key);
        }
        catch (PropertyNotFoundException) {
            outputProperty = defaultValue;
        }
        return outputProperty;
    }

    bool PropertiesEndpointConfiguration::containsKey(const string &key) {
        try {
            properties.GetProperty(key);
            return true;
        }
        catch (PropertyNotFoundException) {
            return false;
        }
    }
} // cloudio