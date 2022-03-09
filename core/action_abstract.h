#ifndef ACTION_ABSTRACT_H
#define ACTION_ABSTRACT_H

class Unit;


class Action
{
public:
	virtual int exec(Unit&) = 0;
	virtual ~Action() {}
};



#endif // ACTION_ABSTRACT_H
