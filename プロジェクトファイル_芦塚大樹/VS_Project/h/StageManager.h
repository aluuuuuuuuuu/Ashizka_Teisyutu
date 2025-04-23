#pragma once
#include "Constant.h"
#include <vector>

constexpr int BLOCK_NUM_X = 20;
constexpr int BLOCK_NUM_Y = 2;
constexpr int BLOCK_NUM_Z = 20;

class StageManager:
    public Constant
{
public:
	StageManager();
	virtual ~StageManager();

	/// <summary>
	/// ステージを描画する
	/// </summary>
	void DrawStage() const;

    /// <summary>
    /// ステージの配列の中身を取得する
    /// </summary>
    /// <param name="a">配列のx要素</param>
    /// <param name="b">配列のy要素</param>
    /// <param name="c">配列のz要素</param>
    /// <returns>配列の要素</returns>
    int GetStageInfo(int a, int b, int c);

    /// <summary>
    /// 指定されたボックスに0を入れる
    /// </summary>
    /// <param name="x">マスの要素数x</param>
    /// <param name="y">マスの要素数y</param>
    /// <param name="z">マスの要素数z</param>
    void DeleteBox(int x, int y, int z);

private:

    // モデルハンドル
    int _modelHandleGreen;
    int _modelHandleRed;



    int _stage[BLOCK_NUM_X][BLOCK_NUM_Y][BLOCK_NUM_Z];

    int _model[BLOCK_NUM_X][BLOCK_NUM_Y][BLOCK_NUM_Z];

    int _size;
};

