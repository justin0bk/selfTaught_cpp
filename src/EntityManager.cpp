#include "EntityManager.h"
#include "Entity.h"

EntityManager::EntityManager() {}

std::shared_ptr<Entity> EntityManager::createEntity(const size_t id, const std::string& tag)
{
    return std::shared_ptr<Entity>(new Entity(id, tag));
}

void EntityManager::removeDeadEntities(EntityVec & vec)
{
    vec.erase(
        // std::remove_if returns the index of the first element to be removed after sorting 
        std::remove_if(vec.begin(), vec.end(),
            //lambda function to sort and move the false-returning items to the back
            [](const std::shared_ptr<Entity>& entity) {
                return !entity->isActive();
            }),
        vec.end());
}

void EntityManager::update()
{
    // Create entities from buffer
    for (auto& e : m_entitiesToAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->m_tag].push_back(e);
        m_totalEntities += 1;
    }

    if (!m_entitiesToAdd.empty())
    {
        m_entitiesToAdd.clear();
    }

    // Remove dead entities from the main entity list
    removeDeadEntities(m_entities);

    // Remove dead entities from the entity map
    for (auto& [tag, entities] : m_entityMap)
    {
        removeDeadEntities(entities);
    }
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string & tag)
{   
    auto e = createEntity(m_totalEntities, tag);
    if (tag == "player")
    {
        m_entities.push_back(e);
    } else
    {
        m_entitiesToAdd.push_back(e);
    }
    return e;
}

const EntityVec & EntityManager::getEntities()
{
    return m_entities;
}

const EntityVec & EntityManager::getEntities(const std::string & tag)
{   
    return m_entityMap[tag];
}