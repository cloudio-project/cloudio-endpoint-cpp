//
// Created by lucas on 20/10/22.
//

#ifndef CLOUDIO_ENDPOINT_CPP_PROPERTIESENDPOINTCONFIGURATION_H
#define CLOUDIO_ENDPOINT_CPP_PROPERTIESENDPOINTCONFIGURATION_H

#include "ICloudioEndpointConfiguration.h"
#include <iostream>
#include "PropertiesParser.h"

using namespace cppproperties;
using namespace std;

namespace cloudio {

    class PropertiesEndpointConfiguration : public ICloudioEndpointConfiguration {
    public:
        PropertiesEndpointConfiguration(const string propertiesFilePath);

        ~PropertiesEndpointConfiguration();

        string getProperty(string key);

        string getProperty(string key, string defaultValue);

        bool containsKey(string key);

    private:
        Properties properties;

    };

} // cloudio

#endif //CLOUDIO_ENDPOINT_CPP_PROPERTIESENDPOINTCONFIGURATION_H
