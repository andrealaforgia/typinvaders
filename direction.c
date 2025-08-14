#include "direction.h"
#include "inline.h"

ALWAYS_INLINE int direction_dx(direction_t d) {
    return d == DIR_LEFT ? -1 : (d == DIR_RIGHT ? 1 : 0);
}

ALWAYS_INLINE int direction_dy(direction_t d) {
    return d == DIR_UP ? -1 : (d == DIR_DOWN ? 1 : 0);
}
