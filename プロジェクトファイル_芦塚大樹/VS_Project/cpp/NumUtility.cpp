#include "NumUtility.h"
#include "DxLib.h"

NumUtility::NumUtility(float size, Vec2 pos, int num):
	_drawPos(pos),
	_size(size)
{

	// 数字画像のロード
	_numHandle[NUM_0] = LoadGraph("data/image/Num0.png");
	_numHandle[NUM_1] = LoadGraph("data/image/Num1.png");
	_numHandle[NUM_2] = LoadGraph("data/image/Num2.png");
	_numHandle[NUM_3] = LoadGraph("data/image/Num3.png");
	_numHandle[NUM_4] = LoadGraph("data/image/Num4.png");
	_numHandle[NUM_5] = LoadGraph("data/image/Num5.png");
	_numHandle[NUM_6] = LoadGraph("data/image/Num6.png");
	_numHandle[NUM_7] = LoadGraph("data/image/Num7.png");
	_numHandle[NUM_8] = LoadGraph("data/image/Num8.png");
	_numHandle[NUM_9] = LoadGraph("data/image/Num9.png");
	_numHandle[NUM_DOT] = LoadGraph("data/image/NumDot.png");

	_drawNumHandle[0] = 0;
	_drawNumHandle[1] = 0;
	_drawNumHandle[2] = 0;
	_drawNumHandle[3] = 0;
	_drawNumHandle[4] = 0;
	_drawNumHandle[5] = 0;

	int a = num;	// 小数点以下
	int b = 0;	// 秒
	int c = 0;	// 分

	b = a / 60;

	a = a % 60;

	if (b > 60) {
		c = b / 60;
		b = b & 60;
	}

	_drawNumHandle[0] = _numHandle[c / 10];
	_drawNumHandle[1] = _numHandle[c % 10];
	_drawNumHandle[2] = _numHandle[b / 10];
	_drawNumHandle[3] = _numHandle[b % 10];
	_drawNumHandle[4] = _numHandle[a / 10];
	_drawNumHandle[5] = _numHandle[a % 10];
}

NumUtility::NumUtility(float size, Vec2 pos):
	_drawPos(pos),
	_size(size)
{
	// 数字画像のロード
	_numHandle[NUM_0] = LoadGraph("data/image/Num0.png");
	_numHandle[NUM_1] = LoadGraph("data/image/Num1.png");
	_numHandle[NUM_2] = LoadGraph("data/image/Num2.png");
	_numHandle[NUM_3] = LoadGraph("data/image/Num3.png");
	_numHandle[NUM_4] = LoadGraph("data/image/Num4.png");
	_numHandle[NUM_5] = LoadGraph("data/image/Num5.png");
	_numHandle[NUM_6] = LoadGraph("data/image/Num6.png");
	_numHandle[NUM_7] = LoadGraph("data/image/Num7.png");
	_numHandle[NUM_8] = LoadGraph("data/image/Num8.png");
	_numHandle[NUM_9] = LoadGraph("data/image/Num9.png");
	_numHandle[NUM_DOT] = LoadGraph("data/image/NumDot.png");

	_drawNumHandle[0] = 0;
	_drawNumHandle[1] = 0;
	_drawNumHandle[2] = 0;
	_drawNumHandle[3] = 0;
	_drawNumHandle[4] = 0;
	_drawNumHandle[5] = 0;
}

NumUtility::~NumUtility()
{
	// 画像のデリート
	for (int i = 0; i <= NUM_DOT; i++) {
		DeleteGraph(_numHandle[i]);
	}
}

void NumUtility::Update(int time)
{
	int a = time;	// 小数点以下
	int b = 0;	// 秒
	int c = 0;	// 分

	b = a / 60;

	a = a % 60;

	if (b > 60) {
		c = b / 60;
		b = b & 60;
	}

	_drawNumHandle[0] = _numHandle[c / 10];
	_drawNumHandle[1] = _numHandle[c % 10];
	_drawNumHandle[2] = _numHandle[b / 10];
	_drawNumHandle[3] = _numHandle[b % 10];
	_drawNumHandle[4] = _numHandle[a / 10];
	_drawNumHandle[5] = _numHandle[a % 10];
}

void NumUtility::Draw() const
{
	int n = 0;
	int i = 1;
	while (i <= 8) {
		//DrawGraph(_drawPos.intX() + 100 * i , _drawPos.intY(), _drawNumHandle[n], true);
		DrawRotaGraph(static_cast<int>(_drawPos.x + (100 * _size) * i), static_cast<int>(_drawPos.y), _size, 0.0, _drawNumHandle[n], true);
		n++;

		if (n % 2 == 0 && n != 6) {
			i++;
			//DrawGraph(_drawPos.intX() + 100 * i, _drawPos.intY(), _numHandle[NUM_DOT], true);
			DrawRotaGraph(static_cast<int>(_drawPos.x + (100 * _size) * i), static_cast<int>(_drawPos.y), _size, 0.0, _numHandle[NUM_DOT], true);
		}
		i++;
	}
}
