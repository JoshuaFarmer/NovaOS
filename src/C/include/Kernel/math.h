#pragma once

double sin(double x) {
	double result = 0;
	double term = x;
	int sign = 1;
	for (int i = 1; i <= 10; i += 2) {
		result += sign * term;
		term *= -(x * x) / ((i + 1) * (i + 2));
		sign *= -1;
	}
	return result;
}

int abs(int value) {
	return value < 0 ? -value : value;
}