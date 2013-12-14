#pragma once

#include <Kunlaboro/Component.hpp>
#include <SFML/System/Vector2.hpp>

class Dialog : public Kunlaboro::Component
{
public:
    Dialog();
    ~Dialog();
    
    void addedToEntity();

    inline void setMessage(const std::string& str) { mMessage = str; }

private:
    std::string mMessage;
    sf::Vector2f mPosition;
    float mTime;
};
