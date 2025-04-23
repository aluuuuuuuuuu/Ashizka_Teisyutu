#pragma once
#include <Crown.h>
#include "Vec2.h"
#include <memory>
#include "Constant.h"

class SceneTitle;
class Crown:
	public Constant
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Crown(SceneTitle& scene);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Crown();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();
private:

	// 王冠画像ハンドル
	int _crownHandle;

	// 王冠パーティクル画像ハンドル
	int _crownParticleHandle1;
	int _crownParticleHandle2;
	int _crownParticleHandle3;

	// この変数の中にパーティクル画像ハンドルが順々に入っていく
	int _crownParticle;

	// フレームカウント
	int _flame1;
	int _flame2;

	bool _flame1Flag;

	bool _blinkingFlag;
	
	Vec2 _dispPos;

};

