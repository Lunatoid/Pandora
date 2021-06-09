#include "Duration.h"

namespace pd {

Duration::Duration(f64 seconds) {
    this->seconds = seconds;
    this->milliseconds = (u32)seconds * 1000;
    this->microseconds = (u64)milliseconds * 1000;
}

Duration::Duration(u32 milliseconds) {
    this->seconds = (f64)milliseconds / 1000.0;
    this->milliseconds = milliseconds;
    this->microseconds = (u64)milliseconds * 1000;
}

}
