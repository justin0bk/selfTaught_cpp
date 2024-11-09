#include "Entity.h"

Entity::Entity(const size_t id, const std::string & tag)
    : m_id(id)
    , m_tag(tag)
    {}

bool Entity::isActive() const
{
    return this->m_active;
}

const std::string& Entity::tag() const
{
    return this->m_tag;
}

const std::size_t Entity::id() const
{
    return this->m_id;
}

void Entity::destroy()
{
    this->m_active = false;
}