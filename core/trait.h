#ifndef TRAIT_H
#define TRAIT_H

class Unit;


class Trait
{
	enum StepReturn
	{
		OK,
		DELETE_THIS
	};

public:
	//Trait(Object& parent) noexcept : parent(parent) {};

	virtual int step(Unit&) noexcept { return 0; }

	virtual ~Trait() noexcept {}
};

class TraitSight : public Trait
{

};

#endif // TRAIT_H
