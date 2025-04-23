#pragma once
#include "Vec3.h"
#include <vector>
#include <string>
#include <list>

// 座標、拡大、回転を制御する
class Transform
{
public:
	Transform();
	virtual ~Transform();

	// 座標
	Vec3 Position;

	// 拡大
	Vec3 Scale;

	// 回転
	Vec3 Angle;

	Transform& GetTransformInstance();

private:

};

// モデル全般の制御をする
class Model
{
public:
	Model();
	virtual ~Model();

	// 3Dモデル初期処理
	void InitModel(int modelHandle, int textureHandle);
	void InitModel(int modelHandle);
	
	// 3D更新処理
	void UpdateModel(Transform& transform);

	// 3Dモデル描画処理
	void DrawModel() const;

	// モデルハンドルのゲッター
	int GetModelHandle() const;

	// テクスチャのゲッター
	int GetTextureHandle() const;

protected:
	// モデルハンドル
	int _modelHandle = 0;

private:

	// モデルのテクスチャ
	int m_textureHandle = 0;

};

// アニメーションの制御をする
class Animation
{
public:
	Animation();
	virtual ~Animation();

	/// <summary>
	/// アニメーション初期処理
	/// </summary>
	/// <param name="modelHandle">モデルハンドル</param>
	/// <param name="tag">デフォルトのアニメーションタグ</param>
	/// <param name="rate">デフォルトのブレンドレート</param>
	void InitAnimation(int& modelHandle, int tag, float rate);

	/// <summary>
	/// アニメーション更新処理
	/// </summary>
	/// <param name="modelHandle">モデルハンドル</param>
	/// <param name="count">再生速度</param>
	void UpdateAnimation(int& modelHandle, float count);

	/// <summary>
	/// アニメーション更新処理（アニメーションが終了すると停止する）
	/// </summary>
	/// <param name="modelHandle">モデルハンドル</param>
	/// <param name="count">再生速度</param>
	void UpdateAnimationOnce(int& modelHandle, float count);

	/// <summary>
	/// アニメーション変更
	/// </summary>
	/// <param name="modelHandle">モデルハンドル</param>
	/// <param name="tag">変更するアニメーションタグ</param>
	/// <param name="loop">ループフラグ</param>
	/// <param name="blendRate">ブレンドレート</param>
	void ChangeAnimation(int& modelHandle, int tag, bool loop,float blendRate);

	/// <summary>
	/// 連続で切り替えたいときのアニメーション変更
	/// </summary>
	/// <param name="modelHandle">モデルハンドル</param>
	/// <param name="tag1">1つ目のアニメーションタグ</param>
	/// <param name="tag2">2つ目のアニメーションタグ</param>
	/// <param name="rate1">1つ目のブレンドレート</param>
	/// <param name="rate2">2つ目のブレンドレート</param>
	void ChangeAnimationConnect(int& modelHandle, int tag1, int tag2, float rate1,float rate2);

	/// <summary>
	/// アニメーションが終了したことを知らせる
	/// </summary>
	/// <returns>アニメーション終了フラグ</returns>
	bool GetEndAnimFlag();

	/// <summary>
	/// 現行のアニメーションタグを返す
	/// </summary>
	/// <returns>現行のアニメーション</returns>
	int GetAnimTag();


	int GetAnimeFlame();

private:

	// デフォルトのブレンドレート
	float _defaultRate = 0.0f;

	// コネクト先のブレンドレートを保存する
	float _rate2 = 0.0f;

	// ブレンドレート
	float _blendRateSave = 0.0f;

	// 再生時間
	float _flameCount = 0.0f;

	// 総フレーム数
	float _maxFlame = 0.0f;

	// アニメーションインデックス
	int _attachIndex1 = 0;
	int _attachIndex2 = 0;

	// 再生中のアニメーション
	int _playAnimation = 0;

	// 連続で再生するアニメーション
	std::vector<int> _connectAnimation;

	// 連続で再生するアニメーションの進行状況
	int _animationState = 0;
	
	// 現行のアニメーションタグ
	int m_runTag = 0;

	// ループ再生フラグ
	bool _loopFlag = false;

	// デフォルトアニメーションタグ
	int _defaultTag = 0;

	// アニメーションコネクトフラグ
	bool _connectFlag = false;

	// アニメーションブレンドレート
	float _blendRate = 0.0f;

	// アニメーション終了フラグ
	bool _endAnimFlag = true;

};

struct CapsuleData
{
	// 半径
	float Radius;

	// 現フレームの座標
	Vec3 PointA;
	Vec3 PointB;

	// 前フレームの座標
	Vec3 FrontPointA;
	Vec3 FrontPointB;
};

class CapsuleCollision
{
public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CapsuleCollision();

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CapsuleCollision();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="radius">半径</param>
	/// <param name="height">高さ</param>
	void InitCapsule(Vec3 pos, float radius, float height);

	/// <summary>
	/// 有効化
	/// </summary>
	void Aactivation();

	/// <summary>
	/// 無効化
	/// </summary>
	void Invalidation();

	/// <summary>
	/// 有効フラグを取得する
	/// </summary>
	/// <returns>有効であればtrue</returns>
	bool IsActivation();

	/// <summary>
	/// コリジョンに値をセットする
	/// </summary>
	/// <param name="pos">座標</param>
	void Set(Vec3 pos);

	/// <summary>
	/// コリジョンの高さをセットする
	/// </summary>
	/// <param name="height">高さ</param>
	void SetHeight(float height);

	/// <summary>
	/// 線分間の距離を求める
	/// </summary>
	/// <param name="p1">線分1の上点</param>
	/// <param name="q1">線分1の下点</param>
	/// <param name="p2">線分2の上点</param>
	/// <param name="q2">線分2の下点</param>
	/// <returns>距離</returns>
	float CapsuleDistance(const Vec3& p1, const Vec3& q1, const Vec3& p2, const Vec3& q2) const;

	/// <summary>
	/// カプセルを表示する
	/// </summary>
	void DrawCapsule() const;

	CapsuleData _capsuleData;
private:
	float _height = 0;
	bool _valid = true;	// 基本有効化されている
};

class BoxCollision
{
public:
	BoxCollision();
	virtual ~BoxCollision();

	Vec3 Center;
	Vec3 HalfSize;
	std::vector<Vec3> axes;
private:

};

