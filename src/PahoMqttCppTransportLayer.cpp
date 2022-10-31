//
// Created by lucas on 21/10/22.
//

#include "../include/PahoMqttCppTransportLayer.h"
#include "../include/CloudioEndpointPropertyConstants.h"
#include "../include/InvalidPropertyException.h"

namespace cloudio {
    PahoMqttCppTransportLayer::PahoMqttCppTransportLayer() {

    }

    PahoMqttCppTransportLayer::~PahoMqttCppTransportLayer() {
        delete this->mqttClient;
    }

    void
    PahoMqttCppTransportLayer::initTransportLayer(string uuid, ICloudioEndpointConfiguration *endpointConfiguration) {

        // MQTT parameters
        int connectTimeout = stoi(endpointConfiguration->getProperty(MQTT_CONNECTION_TIMEOUT_PROPERTY,
                                                                     MQTT_CONNECTION_TIMEOUT_DEFAULT));
        int retryInterval = stoi(endpointConfiguration->getProperty(MQTT_CONNECT_RETRY_PROPERTY,
                                                                    MQTT_CONNECT_RETRY_DEFAULT));
        int keepAliveInterval = stoi(endpointConfiguration->getProperty(MQTT_KEEPALIVE_INTERVAL_PROPERTY,
                                                                        MQTT_KEEPALIVE_INTERVAL_DEFAULT));

        // Certificates parameters
        string endpointIdentityFilePath;
        string authorityFilePath;
        string identityKeyPath;
        string hostURI;

        if (endpointConfiguration->containsKey(ENDPOINT_IDENTITY_FILE_PROPERTY)) {
            endpointIdentityFilePath = endpointConfiguration->getProperty(ENDPOINT_IDENTITY_FILE_PROPERTY);
        } else {
            throw InvalidPropertyException("No endpoint certificate path given");
        }

        if (endpointConfiguration->containsKey(CERT_AUTHORITY_FILE_PROPERTY)) {
            authorityFilePath = endpointConfiguration->getProperty(CERT_AUTHORITY_FILE_PROPERTY);
        } else {
            throw InvalidPropertyException("No authority path given");
        }

        if (endpointConfiguration->containsKey(ENDPOINT_IDENTITY_KEY_PROPERTY)) {
            identityKeyPath = endpointConfiguration->getProperty(ENDPOINT_IDENTITY_KEY_PROPERTY);
        } else {
            throw InvalidPropertyException("No endpoint private key path given");
        }

        if (endpointConfiguration->containsKey(MQTT_HOST_URI_PROPERTY)) {
            hostURI = endpointConfiguration->getProperty(MQTT_HOST_URI_PROPERTY);
        } else {
            throw InvalidPropertyException("No hoste URI given");
        }


        string verifyHostnameStr = endpointConfiguration->getProperty(SSL_VERIFY_HOSTNAME_PROPERTY,
                                                                      SSL_VERIFY_HOSTNAME_DEFAULT);
        bool verifyHostname = verifyHostnameStr.compare("true") == 0;


        // Construct a client using the Ip and Id
        mqttClient = new mqtt::async_client(hostURI, uuid);

        mqtt::ssl_options sslopts = mqtt::ssl_options_builder()
                .trust_store(authorityFilePath)
                .key_store(endpointIdentityFilePath)
                .private_key(identityKeyPath)
                .ssl_version(3)
                .verify(verifyHostname)
                .finalize();

        mqtt::message willmsg = mqtt::message("@offline/" + uuid, "", mqtt::GRANTED_QOS_1, true);

        this->connopts = mqtt::connect_options_builder()
                .keep_alive_interval(chrono::seconds(keepAliveInterval))
                .will(move(willmsg))
                .ssl(move(sslopts))
                .finalize();
    }

    void PahoMqttCppTransportLayer::connect() {
        // Use the connect method of the client to establish a connection to the broker.
        mqtt::token_ptr conntok = mqttClient->connect(connopts);
        cout << "Waiting for the connection..." << endl;
        conntok->wait();
    }

    void PahoMqttCppTransportLayer::disconnect() {
        mqttClient->disconnect()->wait();
    }

    void PahoMqttCppTransportLayer::publish(string topic, string payload, int qos, bool retained) {
        mqtt::message_ptr timeLeftMessagePointer = mqtt::make_message(
                topic,
                payload,
                qos,
                retained);

        mqttClient->publish(timeLeftMessagePointer);
    }
} // cloudio