#pragma once
#include <iostream>
#include <cmath>
#include <fstream>

using namespace std;
class ImprovedNoise {
public:
	ImprovedNoise();

	double noise(double x, double y, double z);
	double fade(double t);
	double lerp(double t, double a, double b);
	double grad(int hash, double x, double y, double z);
	int p[512];
};