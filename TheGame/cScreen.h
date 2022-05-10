#pragma once

class cScreen
{
public:
    virtual int Run(sf::RenderWindow& App, int &map_i) = 0;
};