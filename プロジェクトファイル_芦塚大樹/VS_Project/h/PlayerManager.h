#pragma once
#include <memory>
#include <vector>
#include "Constant.h"
#include "BulletManager.h"

constexpr int PLAYER_ONE = 0;
constexpr int PLAYER_TWO = 1;
constexpr int PLAYER_THREE = 2;
constexpr int PLAYER_FORE = 3;

struct VECTOR4
{
	int a;
	int b;
	int c;
	int d;
};

struct VECTOR2
{
	int a;
	int b;
};

// プレイヤーのデータ構造体 
struct PlayerData
{
	// パッド基準でどのモデルを使用するか
	int character[4];

	// プレイヤーの総数
	int playerNum = -1;

	// 勝利したプレイヤー
	int winner = -1;

	bool aiFlag = false;
};

// 弾のデータ
struct BulletData
{
	// 選択している弾
	int _selectBullet = NORMAL_BULLET;

	// 弾ごとのクールタイム
	int _bullletCoolTime[3];

};

class MyEffect;
class PlayerUi;
class StageManager;
class CollisionManager;
class BulletManager;
class Player;
class PlayerManager :
	public Constant
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="stageManager">ステージの参照</param>
	/// <param name="bullet">弾マネージャーの参照</param>
	/// <param name="data">プレイヤーデータの参照</param>
	PlayerManager(std::shared_ptr<StageManager>& stageManager, std::shared_ptr<BulletManager>& bullet, PlayerData& data);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~PlayerManager();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="num">プレイヤーナンバー</param>
	void Draw(int num) const;

	/// <summary>
	/// 描画範囲を取得
	/// </summary>
	/// <param name="num">プレイヤーのナンバー</param>
	/// <returns>プレイヤーのナンバーに対応した描画範囲</returns>
	VECTOR4 GetArea(int num) const;

	/// <summary>
	/// カメラのセンターを取得
	/// </summary>
	/// <param name="num">プレイヤーのナンバー</param>
	/// <returns>プレイヤーのナンバーに対応したカメラのセンター</returns>
	VECTOR2 GetCenter(int num) const;

	/// <summary>
	/// プレイヤーの総数を取得する
	/// </summary>
	/// <returns>プレイヤーの総数</returns>
	int GetPlayerNum() const;
	
	/// <summary>
	/// プレイヤーのモデルハンドルを渡す
	/// </summary>
	/// <param name="num">プレイヤーナンバー</param>
	/// <returns>モデルハンドル</returns>
	int GetModelHandle(int num) const;

	/// <summary>
	/// 引数に対応するプレイヤーのカメラの設定
	/// </summary>
	/// <param name="num">プレイヤーのナンバー</param>
	void CameraSet(int num) const;

	/// <summary>
	/// 生存しているプレイヤーの数を取得
	/// </summary>
	/// <returns>生存しているプレイヤーの数</returns>
	int GetAreAlivePlayerNum() const;

	/// <summary>
	/// プレイヤーデータを取得する
	/// </summary>
	PlayerData GetPlayerData() const;

	/// <summary>
	/// 勝利プレイヤーを設定する
	/// </summary>
	void SetWinner();

	/// <summary>
	/// AIを追加する
	/// </summary>
	void AddAi();

	/// <summary>
	/// AIが呼ぶ用のプレイヤーの座標を返す関数
	/// </summary>
	/// <returns>プレイヤーの座標</returns>
	Vec3 GetPlayerPos() const;

	/// <summary>
	/// aiフラグを取得する
	/// </summary>
	/// <returns>trueであればAI戦</returns>
	bool GetAiFlag() const;

	/// <summary>
	/// AIキャラを削除する
	/// </summary>
	void DeleteAi();

	std::vector<std::shared_ptr<Player>>& GetPlayerList();

private:

	/// <summary>
	/// プレイヤー数から描画範囲を求める
	/// </summary>
	/// <param name="num">プレイヤーの数-1/param>
	/// <param name="scWidth">スクリーンの幅</param>
	/// <param name="scHeight">スクリーンの高さ</param>
	/// <returns>描画範囲</returns>
	VECTOR4 CreateDrawArea(int num, int scWidth, int scHeight);

	/// <summary>
	/// プレイヤー数からスクリーンの中心を求める
	/// </summary>
	/// <param name="num">プレイヤーの数-1</param>
	/// <returns>カメラの中心</returns>
	VECTOR2 CreateScreenCenter(int num);

	// プレイヤーの配列
	std::vector<std::shared_ptr<Player>> _pPlayer;

	// ウィンドウの高さ
	int _windowWidth;

	// ウィンドウの幅
	int _windowHeight;

	// プレイヤーの数に対応した描画範囲
	VECTOR4 _drawArea[4];

	// プレイヤーの数に対応したカメラのセンター
	VECTOR2 _cameraSenter;

	// プレイヤーの数の対応したモデルハンドル
	int _modelHandle[4];

	// プレイヤーごとの弾のデータ
	BulletData _bulletData[4];

	// コリジョンマネージャーのポインタ
	std::shared_ptr<CollisionManager> _pCollision;

	PlayerData _playerData;

	std::shared_ptr<PlayerUi> _pUi;

	// 弾マネージャーの参照
	std::shared_ptr<BulletManager>& _bulletManager;

	// 勝利エフェクト
	std::shared_ptr<MyEffect> _pWinEffect;

	// 撃墜エフェクト
	std::shared_ptr<MyEffect> _pShotOutEffect;

	// フレームカウンタ
	int _frame;

	// 勝利したプレイヤー
	int _winner;
};

