#ifndef ROD_H
#define ROD_H

#include "item.h"

// HERANÇA 1: Rod (vara de pesca) é um Item
// A vara define até onde o anzol alcança no lago.
class Rod : public Item {
private:
    unsigned int depth_limit;     // profundidade máxima (linhas abaixo da superfície)
    unsigned int distance_limit;  // distância máxima para os lados

public:
    Rod();
    Rod(const std::string& name, const std::string& desc, float valor,
        unsigned int depth, unsigned int distance);

    unsigned int getDepthLimit() const;
    unsigned int getDistanceLimit() const;

    std::string categoria() const override;
    void itemDescription() const override;
};

#endif
