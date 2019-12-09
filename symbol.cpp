#include "symbol.h"

void Symbol::Serialise(json& object)
{
    // check for flags - strip hardware
    object["address"] = address;
    if(labelName.length() > 0)
    {
        object["labelName"] = labelName;
    }
//    object["readName"] = readName;
//    object["writeName"]
}

void Symbol::Deserialise(json& object)
{

}
