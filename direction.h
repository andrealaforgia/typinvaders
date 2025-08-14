#ifndef DIRECTION_H_
#define DIRECTION_H_

typedef enum { DIR_UP, DIR_DOWN, DIR_LEFT, DIR_RIGHT } direction_t;

int direction_dx(direction_t d);
int direction_dy(direction_t d);

#endif  // DIRECTION_H_
