#include "rod.h"
#include <iostream>

Rod::Rod()
    : Item("Vara de Bambu",
           "Uma vara simples. Alcanca so os peixes perto da superficie.", 0),
      depth_limit(3), distance_limit(6) {}

Rod::Rod(const std::string& name, const std::string& desc, float valor,
         unsigned int depth, unsigned int distance)
    : Item(name, desc, valor), depth_limit(depth), distance_limit(distance) {}

unsigned int Rod::getDepthLimit() const {
    return depth_limit;
}

unsigned int Rod::getDistanceLimit() const {
    return distance_limit;
}

void Rod::itemDescription() const {
    Item::itemDescription();
    std::cout << "    Profundidade: " << depth_limit
              << " | Alcance lateral: " << distance_limit << std::endl;
}
