#pragma once
#include <ctime>

struct timer
{
	timer(long miliseconds);
	void update();
	bool check();
	~timer();
	clock_t begin;
	long miliseconds;
};