#pragma once
#include "BulletBase.h"
#include <memory>
#include <list>
#include <Constant.h>

#define NORMAL_BULLET 0
#define GRAPPLER_BULLET 1
#define BOMB_BULLET 2

#define MAX_TYPE_NUM 3
#define MIN_TYPE_NUM 0

class WedgewormManager;
class MapBulletCollisionManager;
class BulletBase;
class BulletManager :
	public Constant
{
public:
	BulletManager(std::shared_ptr<MapBulletCollisionManager>& col, std::shared_ptr<WedgewormManager>& worm);
	virtual ~BulletManager();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() const;

	/// <summary>
	/// プレイヤーが呼ぶ関数、指定の弾を発射する
	/// </summary>
	/// <param name="bul">弾の種類</param>
	/// <param name="dist">方向ベクトル</param>
	/// <param name="pos">座標</param>
	/// <param name="plNum">プレイヤーナンバー</param>
	void PushBullet(int bul, Vec3 dist, Vec3 pos, int plnum);
	
	/// <summary>
	/// 現状グラップラーが当たったかどうかを返す関数
	/// </summary>
	/// <param name="plNum">プレイヤーナンバー<</param>
	/// <returns>当たっていたらtrue</returns>
	bool IsCollisionBullet(int plNum);

	/// <summary>
	/// 弾が無効かされているか返す現状グラップラーのみ
	/// </summary>
	/// <param name="plNum">プレイヤーナンバー<</param>
	/// <returns>無効であればtrue</returns>
	bool GetInvalidFlag(int plNum) const;

	/// <summary>
	/// 弾を無効化する現状グラップラーのみ
	/// </summary>
	/// <param name="plNum">プレイヤーナンバー<</param>
	void KillBullet(int plNum);

	
	/// <summary>
	/// 現状グラップラーのみの座標を返す
	/// </summary>
	/// <param name="plNum">プレイヤーナンバー<</param>
	/// <returns>弾の座標</returns>
	Vec3 GetBulletPos(int plNum);

	/// <summary>
	/// 現状グラップラーが存在しているかどうか返す
	/// </summary>
	/// <param name="plNum">プレイヤーナンバー</param>
	/// <returns>存在していたらtrue</returns>
	bool GetBulletExist(int plNum);

	/// <summary>
	/// バレットの配列の参照を返す
	/// </summary>
	/// <returns>バレットの配列</returns>
	std::list<std::shared_ptr<BulletBase>>& GetBulletList();

private:

	// バレットを保存するリスト配列
	std::list<std::shared_ptr<BulletBase>> _pBullet;

	// コリジョンマネージャーの参照
	std::shared_ptr<MapBulletCollisionManager>& _collManager;

	// 禊虫マネージャーの参照
	std::shared_ptr<WedgewormManager>& _wedgeManager;
};
