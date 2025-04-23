#pragma once
#include "Singleton.h"
#include <map>

constexpr int  VOLUME_MIN = 85;
constexpr int  VOLUME_MIDDLE = 170;
constexpr int  VOLUME_MAX = 255;
constexpr int  VOLUME_ZERO = 0;

constexpr int BGM_OPENING = 0;
constexpr int BGM_BATTLE = 1;
constexpr int BGM_THEME = 2;

constexpr int SE_NUM_SELECT = 0;
constexpr int SE_TITLE_START = 1;
constexpr int SE_CHARA_SELECT = 2;
constexpr int SE_CHARA_CANCEL = 3;
constexpr int SE_GAME_START = 4;
constexpr int SE_RESULT = 5;
constexpr int SE_SHOUT = 6;
constexpr int SE_BEEP = 7;
constexpr int SE_OPEN_MENU = 8;
constexpr int SE_CLOSE_MENU = 9;
constexpr int SE_BULLET_SELECT = 10;
constexpr int SE_SHOT_NORMAL_BULET = 11;
constexpr int SE_SHOT_GRAPPLE_BULET = 12;
constexpr int SE_SHOT_BOMB_BULET = 13;
constexpr int SE_EXPLOSION = 14;
constexpr int SE_DESTRUCTION = 15;
constexpr int SE_GRAPPLE = 16;
constexpr int SE_KO = 17;



class SoundManager:
	public Singleton<SoundManager>
{
public:

	/// <summary>
	/// 初期処理
	/// </summary>
	void Init();

	/// <summary>
	/// BGMをスタートさせる
	/// </summary>
	/// <param name="bgm">bgm定数名</param>
	void StartBGM(int bgm);

	/// <summary>
	/// BGMをストップさせる
	/// </summary>
	/// <param name="bgm">bgm定数名</param>
	void StopBGM(int bgm);

	/// <summary>
	/// SEを鳴らす
	/// </summary>
	/// <param name="se">se定数名</param>
	void RingSE(int se);

	/// <summary>
	/// BGMの音量を設定する
	/// </summary>
	/// <param name="volume">音量定数</param>
	void SetVolumeBGM(int volume);

	/// <summary>
	/// SEの音量を設定する
	/// </summary>
	/// <param name="volume">音量定数</param>
	void SetVolumeSE(int volume);

	/// <summary>
	/// BGMの音量を返す
	/// </summary>
	/// <returns>BGMの音量</returns>
	int GetVolumeBGM();

	/// <summary>
	/// SEの音量を返す
	/// </summary>
	/// <returns>SEの音量</returns>
	int GetVolumeSE();

private:

	// BGM音量
	int _volumeBGM = VOLUME_MIDDLE;
	
	// SE音量
	int _volumeSE = VOLUME_MIDDLE;

	// BGMマップ
	std::map<int, int> _bgmMap;

	// SEマップ
	std::map<int, int> _seMap;
};

