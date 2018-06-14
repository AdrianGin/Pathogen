#pragma once

#include <stdint.h>



class Wavetable
{

public:
	Wavetable();
	virtual ~Wavetable();

	void importWave(const char* file);


protected:
	const double* wtLeft;
	const double* wtRight;
	uint32_t	  wtLength;
};

