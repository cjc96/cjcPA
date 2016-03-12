#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	long long temp_a = a, temp_b = b;
	return (temp_a * temp_b) >> 16;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	int temp_sign = 1, i;
	
	if (a < 0)
	{
		temp_sign *= -1;
		a = -a;
	}
	if (b < 0)
	{
		temp_sign *= -1;
		b = -b;
	}
	int temp_int = a / b;
	a = a % b;
	for (i = 0; i < 16; i++)
	{
		a <<= 1;
		temp_int <<= 1;
		if (a >= b)
		{
			a -= b;
			temp_int++;
		}
	}
	int ans = temp_int * temp_sign;
	return ans;
}

FLOAT f2F(float a) {
	unsigned int temp_a = *(unsigned int *)&a, temp_sign = temp_a >> 31, temp_exp = temp_a >> 23 & 0xff, temp_man = temp_a & 0x007fffff;
	if (temp_exp)
		temp_man += 1 << 23;
	temp_exp -= 150;
	if (temp_exp < -16) temp_man >>= -16 - temp_exp;
	if (temp_exp > -16) temp_man <<= temp_exp + 16;
	
	return temp_sign ? -temp_man : temp_man;
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

