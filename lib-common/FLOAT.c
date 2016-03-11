#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	long long temp_a = a, temp_b = b;
	return (temp_a * temp_b) >> 16;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	nemu_assert(0);
	return 0;
}

FLOAT f2F(float a) {
	int temp_sign, temp_int, temp_frac;
	
	temp_sign = a >= 0 ? 0 : 1;
	temp_int = trunc(a);
	return 0;
}

FLOAT Fabs(FLOAT a) {
	int temp = a;
	return temp >= 0 ? temp : -temp;
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

