#include "MyEffect.h"
#include "EffekseerForDXLib.h"
#include "EffectManager.h"

MyEffect::MyEffect(int effect,Vec3 pos)
{
	_playHandle = PlayEffekseer3DEffect(EffectManager::GetInstance().GetEffectHandle(effect));
	SetPosPlayingEffekseer3DEffect(_playHandle, pos.x, pos.y, pos.z);
}

MyEffect::~MyEffect()
{
}

void MyEffect::Update(Vec3 pos)
{
	// エフェクトの移動
	SetPosPlayingEffekseer3DEffect(_playHandle, pos.x, pos.y, pos.z);
}

void MyEffect::StopEffect()
{
	StopEffekseer3DEffect(_playHandle);
}

void MyEffect::StartEffect()
{
	StopEffekseer3DEffect(_playHandle);
}

void MyEffect::SetScaleEffect(Vec3 scale)
{
	// エフェクトのスケールを設定
	SetScalePlayingEffekseer3DEffect(_playHandle, scale.x, scale.y, scale.z);
}
