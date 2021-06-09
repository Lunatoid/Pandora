#pragma once

#include "Pandora/Core/Types.h"

namespace pd {

enum class EventCategory : u8 {
    Undefined,
    Window,
    Custom
};

struct Event {
    EventCategory category = EventCategory::Undefined;
};

}
