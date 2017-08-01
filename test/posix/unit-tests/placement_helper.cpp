#include "catch.hpp"
#include <frab/frab.h>

using namespace framework_abstraction;

class normal_type
{
    int value;

protected:
    void initialize()
    {
        value = 0;
    }

public:
    normal_type()
    {
        initialize();
    }

    void increment() { value++; }
    operator const int& () { return value; }
};


class recycle_aware_type : public normal_type
{
    int recycle_counter = 0;
public:
    void recycle()
    {
        initialize();
        recycle_counter++;
    }

    int get_recycle_count() { return recycle_counter; }
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
        static experimental::placement_helper<normal_type> value;
        normal_type& v = value;

        v.increment();

        REQUIRE(v == 1);

        value.recycle();

        REQUIRE(v == 0);

    }
    SECTION("Recycle aware type")
    {
        static experimental::placement_helper<recycle_aware_type> value;

        recycle_aware_type& v = value;

        v.increment();

        REQUIRE(v == 1);
        REQUIRE(v.get_recycle_count() == 0);

        value.recycle();

        REQUIRE(v == 0);
        // Fails, explicit recycle not working
        //REQUIRE(v.get_recycle_count() == 1);
    }
}