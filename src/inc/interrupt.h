#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__

#include "Regst.h"
#include "desc.h"


void idt_install();
void intr_install();
void handler(struct registers_t  const * r);

#endif /*__INTERRUPT_H__*/
