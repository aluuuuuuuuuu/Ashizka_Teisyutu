#include "FallCharactor.h"
#include "DxLib.h"
#include <ctime>
#include "SoundManager.h"
#include "Application.h"

FallCharactor::FallCharactor() :
	_fallFlame(0)
{
	// 定数ファイルの読み込み
	ReadCSV("data/constant/FallCharactor.csv");

	// 乱数生成器の初期化
	srand(static_cast<unsigned int>(time(nullptr)));

	// 落下キャラの画像のロード
	{
		_fallCharactorHandle[CHARACTER_1] = LoadGraph("data/image/Falling1.png");
		_fallCharactorHandle[CHARACTER_2] = LoadGraph("data/image/Falling2.png");
		_fallCharactorHandle[CHARACTER_3] = LoadGraph("data/image/Falling3.png");
		_fallCharactorHandle[CHARACTER_4] = LoadGraph("data/image/Falling4.png");
	}
}

FallCharactor::~FallCharactor()
{
	// 落下キャラの画像の削除
	for (int i = 0; i < 4; i++) {
		DeleteGraph(_fallCharactorHandle[i]);
	}
}

void FallCharactor::Update()
{
	// フレームを進める
	_fallFlame++;

	auto& app = Application::GetInstance();

	// フレームが240を超えたらキャラクターの落下を始める
	if (_fallFlame >= app.GetConstantInt("FRAME_NUM") * 4) {
		// フレームが100で割り切れる数になったらキャラクターを落下させる
		if (_fallFlame % app.GetConstantInt("FRAME_NUM") == 0) {

			// ランダムな座標と角度を生成
			float randomAngle = static_cast<float>(rand()) / RAND_MAX * 2 * DX_PI_F;
			bool rotateFlag = (rand() % 5 == 0);

			// 落下キャラを追加
			_fallCharactors.push_back({ _fallCharactorHandle[rand() % 4],Vec2{rand() % app.GetConstantInt("SCREEN_WIDTH"),GetConstantInt("SPAWN_Y")},randomAngle,rotateFlag });
		}
	}

	// 落下キャラの更新
	for (auto& character : _fallCharactors) {
		character._pos.y += GetConstantInt("FALL_SPEED");
		if (character._rotateFlag) {
			character._angle += 0.1;
		}
	}
	int deadLine = GetConstantInt("DEAD_LINE");

	// 画面外に出たキャラクターを削除
	_fallCharactors.erase(std::remove_if(_fallCharactors.begin(), _fallCharactors.end(), [deadLine](const Character& characotr) {
		return characotr._pos.y > deadLine;
		}), _fallCharactors.end());

}

void FallCharactor::Draw() const
{
	// 落下キャラの描画
	for (auto& characotr : _fallCharactors) {
		DrawRotaGraph(characotr._pos.intX(), characotr._pos.intY(), 0.3, characotr._angle, characotr._handle, true);
	}
}
