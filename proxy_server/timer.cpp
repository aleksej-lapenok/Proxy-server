#include "timer.h"

timer::timer(long miliseconds)
{
	this->miliseconds = miliseconds;
	begin = clock();
}

void timer::update()
{
	begin = clock();
}

bool timer::check()
{
	// TODO: get rid of a busy-wait timer
	return ((long)(clock() - begin)) > miliseconds;
}

timer::~timer()
{ }
