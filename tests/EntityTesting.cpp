#include "../src/Entity.h"
#include "../src/EntityManager.h"
#include "../src/Component.h"
#include <iostream>

int main()
{
    EntityManager MGR;
    
    std::shared_ptr<Entity> e = MGR.addEntity("ass");

    std::cout << "Entity tag: " << e->tag() << std::endl;

    std::cout << "Active: " << e->isActive() << std::endl;

    // e.cShape is currently just a nullptt
    if (e->cShape)
    {
        std::cout << "CShape: " << e->cShape->circle.getOrigin().x << std::endl;
    } else
    {
        std::cout << "CShape is nullptr" << std::endl;
    }
    
    // Properly initialize the shape component to the entity
    e->cShape = std::make_shared<CShape>(2.0f, 3, sf::Color(0,0,0),
                                         sf::Color(255,255,255), 1.0f);
    
    // e.cShape is no longer a nullptr
    if (e->cShape)
    {
        std::cout << "CShape X: " << e->cShape->circle.getOrigin().x << std::endl;
    } else
    {
        std::cout << "CShape is nullptr";
    }
    

    return 0;
}