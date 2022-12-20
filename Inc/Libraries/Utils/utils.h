/*
 * utils.h
 *
 *  Created on: 17 Dec 2022
 *      Author: Szymon Grzegorzewski
 */

#ifndef LIBRARIES_UTILS_UTILS_H_
#define LIBRARIES_UTILS_UTILS_H_

void Systick_Setup();
void Systick_Check();

extern uint64_t ms;//reference time counted from the beginning

#endif /* LIBRARIES_UTILS_UTILS_H_ */


//Systick in IRQ
