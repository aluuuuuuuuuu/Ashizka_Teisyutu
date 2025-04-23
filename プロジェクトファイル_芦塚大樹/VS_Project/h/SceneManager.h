#pragma once
#include <memory>
#include <list>
#include "Singleton.h"

class Input;
class SceneBase;
// シーンの管理を行うシングルトンクラス
class SceneManager : public Singleton<SceneManager>
{
	friend class Singleton<SceneManager>;

public:
	
	/// <summary>
	/// 実行中の要素を切り替える
	/// </summary>
	/// <param name="next">切り替えるシーンのポインタ</param>
	void ChangeScene(std::shared_ptr<SceneBase> next);

	/// <summary>
	/// 実行中の要素の更新
	/// </summary>
	void SceneUpdate();

	/// <summary>
	/// すべての要素を描画
	/// </summary>
	void SceneDraw() const;

	/// <summary>
	/// 実行中のシーンの上に追加のシーンを追加する
	/// </summary>
	/// <param name="surface">追加するシーンのポインタ</param>
	void PushScene(std::shared_ptr<SceneBase> surface);

	/// <summary>
	/// 追加のシーンを除去する
	/// </summary>
	void PopScene();

	/// <summary>
	/// ゲーム終了フラグを立てる
	/// </summary>
	void GameEnd();

	/// <summary>
	/// ゲーム終了フラグを取得する
	/// </summary>
	/// <returns></returns>
	bool GetGameEnd();


private:
	// プライベートコンストラクタ
	SceneManager() {};

	// 各シーンのシェアポインタを格納するリスト配列
	std::list<std::shared_ptr<SceneBase>> _pScene;

	// ゲーム終了フラグ
	bool _endFlag = false;

};

