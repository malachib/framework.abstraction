#pragma once

namespace framework_abstraction {

struct event_tx_begin_experimental
{
    void *context;
};

struct event_tx_end_experimental
{
    void *context;
};


}