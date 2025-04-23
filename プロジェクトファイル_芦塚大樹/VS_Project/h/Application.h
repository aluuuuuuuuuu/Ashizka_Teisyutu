#pragma once
#include "Singleton.h"
#include "Constant.h"

namespace
{
	constexpr const char* WINDOW_NAME = "OtoshiKing";
};

class Application:
	public Singleton<Application>,
	public Constant
{
public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <returns>初期化成功フラグ</returns>
	bool Init();

	/// <summary>
	/// メイン処理
	/// </summary>
	void Run();

private:

	/// <summary>
	/// 終了処理
	/// </summary>
	void Terminate();

};
