#pragma once
#include "Components.h"

class ResultCharacter :
	public Model,
	public Animation,
	public Transform
{
public:
	ResultCharacter(int plNum);
	virtual ~ResultCharacter();

	void Update();

	void Draw() const;

private:

	//// アニメーションが終了したタイミングをとるためのフラグ
	//bool _flag;

	//// 演出アニメーションの最大フレームが入る
	//int _maxAnimeFrame;

	//// アニメーションのフレームを増やすか減らすかを判断するフラグ
	//bool _animFlag;

};

