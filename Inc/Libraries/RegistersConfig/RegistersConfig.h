/*
 * RegistersConfig.h
 *
 *  Created on: 24 Mar 2023
 *      Author: Szymon Grzgorzewski
 */

#ifndef LIBRARIES_REGISTERSCONFIG_REGISTERSCONFIG_H_
#define LIBRARIES_REGISTERSCONFIG_REGISTERSCONFIG_H_

void ClockFrequency_Setup();
void Interrupt_Setup();
void GPIOA_Setup();
void Systick_Setup();

void Spi1_Setup();
void Spi1_Send(uint8_t *byte, uint32_t length);
extern uint64_t ms;//reference time counted from the beginning

#endif /* LIBRARIES_REGISTERSCONFIG_REGISTERSCONFIG_H_ */
