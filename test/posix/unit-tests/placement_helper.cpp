#include "catch.hpp"
#include <frab/frab.h>

using namespace framework_abstraction;

class normal_type
{

};


class recycle_aware_type
{
public:
    void recycle()
    {
        
    }
};

TEST_CASE("Placement Helper Tests", "placement-helper")
{
    SECTION("Overly simple type")
    {
        static experimental::placement_helper<int> value;

        value.recycle();
    }
    SECTION("Normal type")
    {

    }
    SECTION("Recycle aware type")
    {

    }
}