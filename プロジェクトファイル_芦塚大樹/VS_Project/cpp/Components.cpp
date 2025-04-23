#include "Components.h"
#include "DxLib.h"
#include <cmath>
#include <algorithm>

// トランスフォーム

Transform::Transform()
{
}

Transform::~Transform()
{
}

Transform& Transform::GetTransformInstance()
{
	return *this;
}

// モデル

Model::Model() :
	_modelHandle(0),
	m_textureHandle(0)
{
}

Model::~Model()
{
	MV1DeleteModel(_modelHandle);
	DeleteGraph(m_textureHandle);
}

void Model::InitModel(int modelHandle, int textureHandle)
{
	_modelHandle = modelHandle;
	m_textureHandle = textureHandle;

	// テクスチャで使用するグラフィックハンドルを変更する
	MV1SetTextureGraphHandle(_modelHandle, 0, textureHandle, FALSE);
}

void Model::InitModel(int modelHandle)
{
	_modelHandle = modelHandle;
}

void Model::UpdateModel(Transform& transform)
{
	// 変化させる行列
	MATRIX modelMtx;

	// モデルを拡大する
	MATRIX scaleMtx = MGetScale(transform.Scale.VGet());

	// モデルの回転を設定
	MATRIX rotMtx = MGetRotY(transform.Angle.y);

	// 行列を乗算する
	modelMtx = MMult(scaleMtx, rotMtx);

	// モデルの座標を更新する
	MATRIX transMtx = MGetTranslate(transform.Position.VGet());

	// 行列を乗算する
	modelMtx = MMult(modelMtx, transMtx);

	// モデルハンドルに反映
	MV1SetMatrix(_modelHandle, modelMtx);
}

void Model::DrawModel() const
{
	// モデルの描画
	MV1DrawModel(_modelHandle);
}

int Model::GetModelHandle() const
{
	return _modelHandle;
}

int Model::GetTextureHandle() const
{
	return m_textureHandle;
}

// アニメーション

Animation::Animation()
{
}

Animation::~Animation()
{
}

void Animation::InitAnimation(int& modelHandle, int tag, float rate)
{
	_defaultRate = rate;

	_defaultTag = tag;

	_loopFlag = true;

	_endAnimFlag = false;

	_attachIndex1 = MV1AttachAnim(modelHandle, tag, -1, true);
	_attachIndex2 = MV1AttachAnim(modelHandle, tag, -1, true);
	_maxFlame = MV1GetAttachAnimTotalTime(modelHandle, _attachIndex1);

	// 再生中のアニメーションのタグを保存する
	_playAnimation = tag;
}

void Animation::UpdateAnimation(int& modelHandle, float count)
{
	// 前のフレームでアニメーションが終了していたらデフォルトに戻す
	if (_endAnimFlag) {
		ChangeAnimation(modelHandle, _defaultTag, true, _defaultRate);
	}

	// 再生時間を進める
	_flameCount += count;

	// ブレンドレートを加算していく
	if (_blendRate >= 1.0f) {
		MV1SetAttachAnimBlendRate(modelHandle, _attachIndex1, 1.0f);
	}
	else {
		_blendRate += _blendRateSave;

		MV1SetAttachAnimBlendRate(modelHandle, _attachIndex2, 1.0f - _blendRate);

		MV1SetAttachAnimBlendRate(modelHandle, _attachIndex1, _blendRate);
	}

	// 再生時間がアニメーションの総再生時間に達したとき
	if (_flameCount >= _maxFlame) {

		// コネクトフラグがtrueだったら次のアニメーションをスタートさせる
		if (_connectFlag) {
			_animationState++;
			ChangeAnimation(modelHandle, _connectAnimation[_animationState], true, _rate2);
			//_connectAnimation.clear();
			_animationState = 0;

			// コネクトフラグを下げる
			_connectFlag = false;
		}
		// ループフラグがtrueだったらループさせる
		else if (_loopFlag) {
			_flameCount = 0.0f;
		}
		// 完全に終了したらフラグを立てる
		else {
			_endAnimFlag = true;
		}
	}

	// アニメーション更新
	int a =MV1SetAttachAnimTime(modelHandle, _attachIndex1, _flameCount);

	int n = 0;
}

void Animation::UpdateAnimationOnce(int& modelHandle, float count)
{

	// 再生時間を進める
	_flameCount += count;

	// ブレンドレートを加算していく
	if (_blendRate >= 1.0f) {
		MV1SetAttachAnimBlendRate(modelHandle, _attachIndex1, 1.0f);
	}
	else {
		_blendRate += _blendRateSave;

		MV1SetAttachAnimBlendRate(modelHandle, _attachIndex2, 1.0f - _blendRate);

		MV1SetAttachAnimBlendRate(modelHandle, _attachIndex1, _blendRate);
	}

	// 再生時間がアニメーションの総再生時間に達したとき
	if (_flameCount >= _maxFlame) {

		// コネクトフラグがtrueだったら次のアニメーションをスタートさせる
		if (_connectFlag) {
			_animationState++;
			ChangeAnimation(modelHandle, _connectAnimation[_animationState], true, _rate2);
			//_connectAnimation.clear();
			_animationState = 0;

			// コネクトフラグを下げる
			_connectFlag = false;
		}
		// ループフラグがtrueだったらループさせる
		else if (_loopFlag) {
			_flameCount = 0.0f;
		}
		// 完全に終了したらフラグを立てる
		else {
			_endAnimFlag = true;
		}
	}

	// アニメーション更新
	int a = MV1SetAttachAnimTime(modelHandle, _attachIndex1, _flameCount);

	int n = 0;


}

/// <summary>
/// アニメーション変更
/// </summary>
/// <param name="modelHandle">モデルハンドル</param>
/// <param name="tag">アニメーションタグ</param>
/// <param name="loop">ループフラグ</param>
/// <param name="blendRate">ブレンドレート</param>
void Animation::ChangeAnimation(int& modelHandle, int tag, bool loop, float blendRate)
{
	// 再生するアニメーションを変更する
	if (tag != _playAnimation) {
		// 現行のアニメーションをデタッチする
		MV1DetachAnim(modelHandle, _attachIndex2);

		// ループフラグを保存
		_loopFlag = loop;

		// ブレンドレートを変更
		_blendRateSave = blendRate;

		// 再生時間を最初に戻す
		_flameCount = 0.0f;

		// 行っていたアニメーションを保存
		_attachIndex2 = _attachIndex1;

		// アニメーションを変更
		_attachIndex1 = MV1AttachAnim(modelHandle, tag);

		// アニメーションのフレーム数を保存
		_maxFlame = MV1GetAttachAnimTotalTime(modelHandle, _attachIndex1);

		// 再生中のアニメーションのタグを保存する
		_playAnimation = tag;

		// アニメーション終了フラグをfalseに
		_endAnimFlag = false;

		// ブレンドレートを初期化する
		_blendRate = 0.0f;
	}
}

// アニメーションを連続させたいときの変更関数
void Animation::ChangeAnimationConnect(int& modelHandle, int tag1, int tag2, float rate1, float rate2)
{
	_endAnimFlag = false;
	_connectAnimation.push_back(tag1);
	_connectAnimation.push_back(tag2);
	ChangeAnimation(modelHandle, _connectAnimation[_animationState], false, rate1);
	_connectFlag = true;
}

bool Animation::GetEndAnimFlag()
{
	return _endAnimFlag;
}

int Animation::GetAnimTag()
{
	return _playAnimation;
}

int Animation::GetAnimeFlame()
{
	return static_cast<int>(_flameCount);
}

// カプセル

CapsuleCollision::CapsuleCollision()
{
}

CapsuleCollision::~CapsuleCollision()
{
}

void CapsuleCollision::InitCapsule(Vec3 pos, float radius, float height)
{
	_capsuleData.Radius = radius;
	_height = height;

	Vec3 vecA = Vec3{ pos.x,pos.y + _capsuleData.Radius,pos.z };
	Vec3 vecB = Vec3{ pos.x,pos.y + _capsuleData.Radius + _height,pos.z };

	_capsuleData.PointA = vecA;
	_capsuleData.PointB = vecB;
}

void CapsuleCollision::Aactivation()
{
	_valid = true;
}

void CapsuleCollision::Invalidation()
{
	_valid = false;
}

bool CapsuleCollision::IsActivation()
{
	return _valid;
}

void CapsuleCollision::Set(Vec3 pos)
{
	// 座標を保存しておく
	_capsuleData.FrontPointA = _capsuleData.PointA;
	_capsuleData.FrontPointB = _capsuleData.PointB;

	// 新しい座標をセットする
	Vec3 vecA = Vec3{ pos.x,pos.y + _capsuleData.Radius,pos.z };
	Vec3 vecB = Vec3{ pos.x,pos.y + _capsuleData.Radius + _height,pos.z };

	_capsuleData.PointA = vecA;
	_capsuleData.PointB = vecB;
}

void CapsuleCollision::SetHeight(float height)
{
	_height = height;
}

float CapsuleCollision::CapsuleDistance(const Vec3& p1, const Vec3& q1, const Vec3& p2, const Vec3& q2) const
{
	Vec3 d1 = q1 - p1;
	Vec3 d2 = q2 - p2;
	Vec3 r = p1 - p2;
	float a = d1.dot(d1);
	float e = d2.dot(d2);
	float f = d2.dot(r);

	float s, t;
	const float epsilon = static_cast<float>(1e-6);
	float c = d1.dot(r);
	float b = d1.dot(d2);
	float denom = a * e - b * b;

	// マクロを無効化して競合を防ぐ

	if (denom != 0) {
		s = (b * f - c * e) / denom;
	}
	else {
		s = 0.0f;
	}
	s = (std::max)(0.0f, (std::min)(1.0f, s));
	t = (b * s + f) / e;

	if (t < 0.0f) {
		t = 0.0f;
		s = (std::max)(0.0f, (std::min)(1.0f, -c / a));
	}
	else if (t > 1.0f) {
		t = 1.0f;
		s = (std::max)(0.0f, (std::min)(1.0f, (b - c) / a));
	}

	Vec3 closestPoint1 = p1 + d1 * s;
	Vec3 closestPoint2 = p2 + d2 * t;
	return (closestPoint1 - closestPoint2).Length();
}

void CapsuleCollision::DrawCapsule() const
{
	if (_valid) {
		DrawCapsule3D(_capsuleData.PointA.VGet(), _capsuleData.PointB.VGet(), _capsuleData.Radius, 4, 0xff0000, 0xff0000, false);
	}
}

// ボックスコリジョンクラス

BoxCollision::BoxCollision()
{
}

BoxCollision::~BoxCollision()
{
}

