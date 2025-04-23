#include "GameFlowManager.h"
#include "PlayerManager.h"

GameFlowManager::GameFlowManager(std::shared_ptr<PlayerManager>& plMg) :
	_playerManager(plMg),
	_gameEndFlag(false),
	_frame(0),
	_gameTime(0)
{
}

GameFlowManager::~GameFlowManager()
{
}

void GameFlowManager::Update()
{
	// プレイヤーの生存数を調べて一人になったら終了フラグを立てる
	if (_playerManager->GetAreAlivePlayerNum() == 1) {
		_gameEndFlag = true;

		// 勝利プレイヤーを確定する
		_playerManager->SetWinner();

	}

	// ゲームが終了してからのフレーム数を数える
	if (_gameEndFlag) {
		_frame++;
	}

	// マイフレームゲームタイムを加算する
	if (!_gameEndFlag) {
		_gameTime++;
	}
}

bool GameFlowManager::GetGameEnd()
{
	return _gameEndFlag;
}

int GameFlowManager::GetFlameCount()
{
	return _frame;
}

int GameFlowManager::GetGameTime()
{
	return _gameTime;
}
