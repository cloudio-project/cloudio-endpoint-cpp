//
// Created by lucas on 20/10/22.
//

#include "../include/CloudioNode.h"

namespace cloudio {

    CloudioNode::CloudioNode(string nodeName) {
        this->nodeName = nodeName;
    }

    CloudioNode::~CloudioNode() {

    }

    list<CloudioObject *> CloudioNode::getObjects() {
        return this->objects;
    }

    list<string> CloudioNode::getInterfaces() {
        return this->interfaces;
    }

    void CloudioNode::addObject(CloudioObject *object) {
        object->setParent(this);
        this->objects.push_front(object);
    }

    string CloudioNode::getName() {
        return this->nodeName;
    }

    void CloudioNode::attributeHasChangedByEndpoint(CloudioAttribute *attribute) {
        if (this->parent != nullptr) {
            this->parent->attributeHasChangedByEndpoint(attribute);
        }
    }

    void CloudioNode::setParent(ICloudioNodeContainer *parent) {
        this->parent = parent;
    }

    ICloudioObjectContainer *CloudioNode::getParentObjectContainer() {
        return nullptr;
    }

    ICloudioNodeContainer *CloudioNode::getParentNodeContainer() {
        return parent;
    }
} // cloudio