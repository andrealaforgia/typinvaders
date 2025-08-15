#include "power_pellet.h"
#include "clock.h"

power_pellet_t power_pellet_create(void) {
    power_pellet_t pellet;
    pellet.status = PPS_VISIBLE;
    pellet.last_ticks = get_clock_ticks_ms();
    return pellet;
}

void power_pellet_update(power_pellet_t *pellet, int current_ticks) {
    if (current_ticks - pellet->last_ticks > 125) {
        pellet->status =
            (pellet->status == PPS_VISIBLE) ? PPS_INVISIBLE : PPS_VISIBLE;
        pellet->last_ticks = current_ticks;
    }
}

power_pellet_status_t power_pellet_get_status(const power_pellet_t *pellet) {
    return pellet->status;
}
