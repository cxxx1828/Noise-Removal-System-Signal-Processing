/*
 * iir.h
 *
 *  Created on: 03.12.2024.
 *      Author: Nina Dragićević
 */

#ifndef IIR_H_
#define IIR_H_

#include "tistdtypes.h"

Int16 second_order_IIR( Int16 input, Int16* coefficients, Int16* x_history, Int16* y_history);

Int16 fourth_order_IIR(Int16 input, Int16 coefficients[][6], Int16 x_history[][2], Int16 y_history[][2]);

Int16 Nth_order_IIR(Int16 input, Int16 coefficients[][6], Int16 x_history[][2], Int16 y_history[][2], Int16 order);


#endif /* IIR_H_ */
