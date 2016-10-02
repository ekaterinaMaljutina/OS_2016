#ifndef __PIT_H__
#define __PIT_H__

#include <stdint.h>
#include "Regst.h"


void pit_init();
void timer(int frequency);
void timer_callback(struct registers_t const * reg);


#endif /*__PIT_H__*/