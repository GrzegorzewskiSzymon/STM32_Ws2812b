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
void TIM1_Setup();
void TIM2_Setup();
void Systick_Setup();
void Systick_Check();

extern uint64_t ms;//reference time counted from the beginning

#endif /* LIBRARIES_REGISTERSCONFIG_REGISTERSCONFIG_H_ */
