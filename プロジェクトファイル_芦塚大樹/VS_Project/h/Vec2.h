#pragma once
#include <cmath>
#include "DxLib.h"

class Vec2
{
public:
	float x;
	float y;
public:
	Vec2() :
		x(0.0f),
		y(0.0f)
	{

	}

	Vec2(float X, float Y) :
		x(X),
		y(Y)
	{

	}

	Vec2(int X, int Y) :
		x(static_cast<float>(X)),
		y(static_cast<float>(Y))
	{

	}

	Vec2(int num) :
		x(static_cast<float>(num)),
		y(static_cast<float>(num))
	{

	}

	Vec2(float num) :
		x(num),
		y(num)
	{

	}

	// int型にキャストして返す
	int intX() const {
		return static_cast<int>(x);
	}

	int intY() const {
		return static_cast<int>(y);
	}

	////*演算子のオーバーロード*////
	Vec2 operator =(float num) {
		x = num;
		y = num;
		return *this;
	}

	Vec2 operator =(int num) {
		x = static_cast<float>(num);
		y = static_cast<float>(num);
		return *this;
	}

	// 足し算

	Vec2 operator +() const {
		return *this;
	}

	Vec2 operator +(Vec2 vec) const {
		return Vec2{ x + vec.x,y + vec.y };
	}

	Vec2 operator +=(Vec2 vec) {
		x += vec.x;
		y += vec.y;
		return *this;
	}

	Vec2 operator +(float num) const {
		return Vec2{ x + num,y + num };
	}

	Vec2 operator +(int num) const {
		return Vec2{ x + static_cast<float>(num),y + static_cast<float>(num) };
	}

	Vec2 operator +=(float num) {
		x += num;
		y += num;
		return *this;
	}

	Vec2 operator +=(int num) {
		x += static_cast<float>(num);
		y += static_cast<float>(num);
		return *this;
	}

	//引き算

	Vec2 operator -() const {
		return Vec2{ -x,-y };
	}

	Vec2 operator -(Vec2 vec) const {
		return Vec2{ x - vec.x,y - vec.y };
	}

	Vec2 operator -=(Vec2 vec) {
		x -= vec.x;
		y -= vec.y;
		return *this;
	}

	Vec2 operator -(float num) const {
		return Vec2{ x - num,y - num };
	}

	Vec2 operator -(int num) const {
		return Vec2{ x - static_cast<float>(num),y - static_cast<float>(num) };
	}

	Vec2 operator -=(float num) {
		x -= num;
		y -= num;
		return *this;
	}

	Vec2 operator -=(int num) {
		x -= static_cast<float>(num);
		y -= static_cast<float>(num);
		return *this;
	}

	//掛け算
	Vec2 operator *(float num) const {
		return Vec2{ x * num,y * num };
	}

	Vec2 operator *(int num) const {
		return Vec2{ x * static_cast<float>(num),y * static_cast<float>(num) };
	}

	Vec2 operator *=(float num) {
		x *= num;
		y *= num;
		return *this;
	}

	Vec2 operator *=(int num) {
		x *= static_cast<float>(num);
		y *= static_cast<float>(num);
		return *this;
	}

	//割り算
	Vec2 operator /(float num) const {
		return Vec2{ x / num,y / num };
	}

	Vec2 operator /(int num) const {
		return Vec2{ x / static_cast<float>(num),y / static_cast<float>(num) };
	}

	Vec2 operator /=(float num) {
		x /= num;
		y /= num;
		return *this;
	}

	Vec2 operator /=(int num) {
		x /= static_cast<float>(num);
		y /= static_cast<float>(num);
		return *this;
	}

	//ベクトルの長さの二乗を求める
	float SqLength() const {
		return x * x + y * y;
	}

	//ベクトルの長さを求める
	float Length() const {
		return sqrtf(SqLength());
	}

	//自身の正規化を行う
	void Normalize() {
		float len = Length();

		//長さ0以下なら処理を行わない
		if (len <= 0.0f) return;

		x /= len;
		y /= len;
	}

	//自信を正規化したベクトルを取得する
	Vec2 GetNormalized() const {
		float len = Length();

		//長さ0以下なら(0,0)を返す
		//引数ありにしている理由
		//引数なしコンストラクタの修正に強くするため
		if (len <= 0.0f) return Vec2{ 0.0f,0.0f };

		return Vec2{ x / len,y / len };
	}

	// 自身や他のベクトルとの内積をとる
	float dot(const Vec2& other) const {
		return x * other.x + y * other.y ;
	}

	// 2つのベクトルの内積を求める
	float dot(const Vec2& a, const Vec2& b) const {
		return a.x * b.x + a.y * b.y;
	}
};