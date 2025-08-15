#ifndef POWER_PELLET_H_
#define POWER_PELLET_H_

typedef enum { PPS_VISIBLE, PPS_INVISIBLE } power_pellet_status_t;

typedef struct {
    power_pellet_status_t status;
    int last_ticks;
} power_pellet_t;

power_pellet_t power_pellet_create(void);
void power_pellet_update(power_pellet_t *pellet, int current_ticks);
power_pellet_status_t power_pellet_get_status(const power_pellet_t *pellet);

#endif  // POWER_PELLET_H_
