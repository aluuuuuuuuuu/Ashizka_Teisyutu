#pragma once
#include "Vec2.h"
#include "vector"
#include <memory>
#include "Constant.h"

constexpr int CHARACTER_1 = 0;
constexpr int CHARACTER_2 = 1;
constexpr int CHARACTER_3 = 2;
constexpr int CHARACTER_4 = 3;


// 落下キャラ構造体
struct Character
{
	int _handle;
	Vec2 _pos;
	double _angle;
	bool _rotateFlag;
};

class SceneTitle;
class FallCharactor:
	public Constant
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	FallCharactor();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FallCharactor();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() const;	
private:

	// 落下キャラのハンドル
	int _fallCharactorHandle[4];

	// 落下キャラの座標
	Vec2 _fallCharactorPos[4];

	// 落下キャラ用のフレームカウンタ
	int _fallFlame;

	// 落下するキャラクター
	std::vector<Character> _fallCharactors;

};

