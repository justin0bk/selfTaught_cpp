#include "../src/Entity.h"
#include "../src/EntityManager.h"
#include <iostream>

int main()
{
    EntityManager MGR;
    
    std::shared_ptr<Entity> e = MGR.addEntity("ass");

    std::cout << "Entity tag: " << e->tag() << std::endl;
    std::cout << "Active: " << e->isActive();

    return 0;
}