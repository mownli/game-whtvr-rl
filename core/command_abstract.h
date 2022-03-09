#ifndef COMMAND_ABSTRACT_H
#define COMMAND_ABSTRACT_H

#include "object.h"


class Command
{
	int id;
public:
	Command(int id) noexcept : id(id) {}

	virtual int process(Object&) = 0;
	virtual ~Command() {}
};

#endif // COMMAND_ABSTRACT_H
