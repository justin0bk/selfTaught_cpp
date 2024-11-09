#include "EntityManager.h"

EntityManager::EntityManager() {}

void EntityManager::removeDeadEntities(EntityVec & vec)
{
    for (auto& e : vec)
    {
        if (!e->isActive())
        {
            e->destroy();
        }
    }
}

void EntityManager::update()
{
    for (auto& e : this->m_entitiesToAdd)
    {
    }
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string & tag)
{   
    auto e = std::make_shared<Entity>(this->m_totalEntities, tag);
    this->m_entities.push_back(e);
    this->m_entityMap[tag].push_back(e);
    this->m_totalEntities += 1;
    return e;
}

const EntityVec & EntityManager::getEntities()
{
    return this->m_entities;
}

const EntityVec & EntityManager::getEntities(const std::string & tag)
{   
    return this->m_entityMap[tag];
}