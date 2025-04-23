#pragma once
#include "Components.h"
#include "Constant.h"

class CreditCharacter:
	public Transform,
	public Model,
	public Animation,
	public Constant
{
public:

	CreditCharacter(int num);

	~CreditCharacter();

	void Update();

	void Draw() const;

	void PosSet();
private:
	int _num;
};

