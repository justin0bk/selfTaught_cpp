#pragma once

#include <vector>
#include <map>
#include <memory>
#include <string>
#include "Entity.h"

using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;

class EntityManager
{
    EntityVec   m_entities;
    EntityVec   m_entitiesToAdd;
    EntityMap   m_entityMap;
    size_t      m_totalEntities = 0;

    std::shared_ptr<Entity> createEntity(const size_t id, const std::string& tag);
    void removeDeadEntities(EntityVec & vec);

public:
    EntityManager();

    void update();

    std::shared_ptr<Entity> addEntity(const std::string & tag);

    const EntityVec & getEntities();
    const EntityVec & getEntities(const std::string & tag);
};