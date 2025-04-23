#pragma once
#include <memory>

class PlayerManager;
class GameFlowManager
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="plMg">プレイヤーマネージャーの参照</param>
	GameFlowManager(std::shared_ptr<PlayerManager>& plMg);

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~GameFlowManager();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// ゲーム終了フラグを返す
	/// </summary>
	/// <returns>終了していたらtrue</returns>
	bool GetGameEnd();

	/// <summary>
	/// ゲームが終了してからのフレーム数を返す
	/// </summary>
	/// <returns>フレーム数</returns>
	int GetFlameCount();


	/// <summary>
	/// ゲーム開始からの時間を返す
	/// </summary>
	/// <returns>ゲームタイム</returns>
	int GetGameTime();

private:
	// プレイヤーマネージャーの参照
	std::shared_ptr<PlayerManager>& _playerManager;

	// ゲーム終了フラグ
	bool _gameEndFlag;

	// フレームカウンタ
	int _frame;

	// ゲーム開始からのフレームを数える
	int _gameTime;
};
