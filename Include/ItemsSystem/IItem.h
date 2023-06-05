#pragma once
#include <string>

class IItem
{
    std::string getName();
    std::string getDescription();
    std::string pathToIcon();
};
