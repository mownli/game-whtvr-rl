#ifndef COMMANDS_H
#define COMMANDS_H

#include "command_abstract.h"


class CommandMoveNW : public Command
{
public:
	CommandMoveNW(int id) noexcept : Command(id) {}
	int process(Object&) noexcept override;
};

class CommandMoveN : public Command
{
public:
	CommandMoveN(int id) noexcept : Command(id) {}
	int process(Object&) noexcept override;
};

class CommandMoveNE : public Command
{
public:
	CommandMoveNE(int id) noexcept : Command(id) {}
	int process(Object&) noexcept override;
};

class CommandMoveSW : public Command
{
public:
	CommandMoveSW(int id) noexcept : Command(id) {}
	int process(Object&) noexcept override;
};

class CommandMoveS : public Command
{
public:
	CommandMoveS(int id) noexcept : Command(id) {}
	int process(Object&) noexcept override;
};

class CommandMoveSE : public Command
{
public:
	CommandMoveSE(int id) noexcept : Command(id) {}
	int process(Object&) noexcept override;
};

class CommandMoveW : public Command
{
public:
	CommandMoveW(int id) noexcept : Command(id) {}
	int process(Object&) noexcept override;
};

class CommandMoveE : public Command
{
public:
	CommandMoveE(int id) noexcept : Command(id) {}
	int process(Object&) noexcept override;
};

#endif // COMMANDS_H
