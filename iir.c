/*
 * iir.c
 *
 *  Created on: 03.12.2024.
 *      Author: Nina Dragićević
 */

#ifndef IIR_C_
#define IIR_C_

#include "iir.h"

Int16 second_order_IIR(Int16 input, Int16* coefficients, Int16* x_history, Int16* y_history) {
    Int32 accum = 0;

    accum = _smac(accum, coefficients[0], input);        /* A0 * x(n)     */
    accum = _smac(accum, coefficients[1], x_history[0]); /* A1/2 * x(n-1) */
    accum = _smac(accum, coefficients[1], x_history[0]); /* A1/2 * x(n-1) */
    accum = _smac(accum, coefficients[2], x_history[1]); /* A2 * x(n-2)   */
    accum = _smas(accum, coefficients[4], y_history[0]); /* B1/2 * y(n-1) */
    accum = _smas(accum, coefficients[4], y_history[0]); /* B1/2 * y(n-1) */
    accum = _smas(accum, coefficients[5], y_history[1]); /* B2 * y(n-2)   */

    accum >>= 16;

    y_history[1] = y_history[0];    /* y(n-2) = y(n-1) */
    y_history[0] = (Int16) (accum); /* y(n-1) = y(n)   */
    x_history [1] = x_history [0];  /* x(n-2) = x(n-1) */
    x_history [0] = input;          /* x(n-1) = x(n)   */

    return ((Int16) accum);
}


Int16 fourth_order_IIR(Int16 input, Int16 coefficients[][6], Int16 x_history[][2],
		Int16 y_history[][2]) {
	Int32 accum;
	Int16 temp;
	Uint16 stage;

    temp = input; /* Copy input to temp */

	for ( stage = 0 ; stage < 2 ; stage++) /* for two stages */
	{
		accum = 0;
		accum = _smac(accum, coefficients[stage][0], temp);         /* A0 * x(n)     */
		accum = _smac(accum, coefficients[stage][1], x_history[stage][0]); /* A1/2 * x(n-1) */
		accum = _smac(accum, coefficients[stage][1], x_history[stage][0]); /* A1/2 * x(n-1) */
		accum = _smac(accum, coefficients[stage][2], x_history[stage][1]); /* A2 * x(n-2)   */
		accum = _smas(accum, coefficients[stage][4], y_history[stage][0]); /* B1/2 * y(n-1) */
		accum = _smas(accum, coefficients[stage][4], y_history[stage][0]); /* B1/2 * y(n-1) */
		accum = _smas(accum, coefficients[stage][5], y_history[stage][1]); /* B2 * y(n-2)   */

		accum >>= 16;

		/* Shuffle values along one place for next time */
		y_history[stage][1] = y_history[stage][0];    /* y(n-2) = y(n-1) */
		y_history[stage][0] = (Int16) (accum); /* y(n-1) = y(n)   */
		x_history[stage][1] = x_history[stage][0];  /* x(n-2) = x(n-1) */
		x_history[stage][0] = temp;          /* x(n-1) = x(n)   */

		/* temp is used as input next time through */
		temp = (Int16)accum;
	}
	/* End loop */

	return (Int16)accum;
}

Int16 Nth_order_IIR(Int16 input, Int16 coefficients[][6], Int16 x_history[][2],
		Int16 y_history[][2], Int16 order) {
	Int32 accum;
		Int16 temp;
		Uint16 stage;

	    temp = input; /* Copy input to temp */

		for ( stage = 0 ; stage < order/2 ; stage++) /* for two stages */
		{
			accum = 0;
			accum = _smac(accum, coefficients[stage][0], temp);         /* A0 * x(n)     */
			accum = _smac(accum, coefficients[stage][1], x_history[stage][0]); /* A1/2 * x(n-1) */
			accum = _smac(accum, coefficients[stage][1], x_history[stage][0]); /* A1/2 * x(n-1) */
			accum = _smac(accum, coefficients[stage][2], x_history[stage][1]); /* A2 * x(n-2)   */
			accum = _smas(accum, coefficients[stage][4], y_history[stage][0]); /* B1/2 * y(n-1) */
			accum = _smas(accum, coefficients[stage][4], y_history[stage][0]); /* B1/2 * y(n-1) */
			accum = _smas(accum, coefficients[stage][5], y_history[stage][1]); /* B2 * y(n-2)   */

			accum >>= 16;

			/* Shuffle values along one place for next time */
			y_history[stage][1] = y_history[stage][0];    /* y(n-2) = y(n-1) */
			y_history[stage][0] = (Int16) (accum); /* y(n-1) = y(n)   */
			x_history[stage][1] = x_history[stage][0];  /* x(n-2) = x(n-1) */
			x_history[stage][0] = temp;          /* x(n-1) = x(n)   */

			/* temp is used as input next time through */
			temp = (Int16)accum;
		}
		/* End loop */

		return (Int16)accum;
}



#endif /* IIR_C_ */
