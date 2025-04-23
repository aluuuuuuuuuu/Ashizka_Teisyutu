#include "SceneManager.h"
#include "SceneBase.h"
#include "Application.h"
#include "Input.h"
#include "SceneMenu.h"

void SceneManager::ChangeScene(std::shared_ptr<SceneBase> next)
{
	// リストが空だったら末尾に追加
	if (_pScene.empty()) {
		_pScene.push_back(next);
	}
	else {
		_pScene.back() = next;	// 末尾のものを入れ替える
	}
}

void SceneManager::SceneUpdate()
{
	// 末尾のみ更新
	_pScene.back()->Update();
}

void SceneManager::SceneDraw() const
{
	// 末尾のみ描画
	for (auto& scene : _pScene) {
		scene->Draw();
	}
}

void SceneManager::PushScene(std::shared_ptr<SceneBase> surface)
{
	_pScene.push_back(surface);
}

void SceneManager::PopScene()
{
	// シーン配列が2つ以上の要素を持っていれば一番上の要素を取り除く
	if (_pScene.size() > 1) {
		_pScene.pop_back();
	}
}

void SceneManager::GameEnd()
{
	_endFlag = true;
}

bool SceneManager::GetGameEnd()
{
	return _endFlag;
}
