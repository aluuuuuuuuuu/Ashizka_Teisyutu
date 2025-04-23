#pragma once
#include<cmath>
#include "DxLib.h"

class Vec3
{
public:
	float x;
	float y;
	float z;
public:
	Vec3() :
		x(0.0f),
		y(0.0f),
		z(0.0f)
	{

	}

	Vec3(float X, float Y, float Z) :
		x(X),
		y(Y),
		z(Z)
	{

	}

	Vec3(int X, int Y, int Z) :
		x(static_cast<float>(X)),
		y(static_cast<float>(Y)),
		z(static_cast<float>(Z))
	{

	}

	Vec3(int num) :
		x(static_cast<float>(num)),
		y(static_cast<float>(num)),
		z(static_cast<float>(num))
	{

	}

	Vec3(float num) :
		x(num),
		y(num),
		z(num)
	{

	}

	Vec3(VECTOR vec) :
		x(vec.x),
		y(vec.y),
		z(vec.z) 
	{
	}

	// int型にキャストして返す
	int intX() const {
		return static_cast<int>(x);
	}

	int intY() const {
		return static_cast<int>(y);
	}

	int intZ() const {
		return static_cast<int>(z);
	}

	////*演算子のオーバーロード*////


	Vec3 operator =(float num) {
		x = num;
		y = num;
		z = num;
		return *this;
	}

	Vec3 operator =(int num) {
		x = static_cast<float>(num);
		y = static_cast<float>(num);
		z = static_cast<float>(num);
		return *this;
	}

	Vec3 operator = (VECTOR vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
		return *this;
	}

	//足し算

	Vec3 operator +() const {
		return *this;
	}

	Vec3 operator +(Vec3 vec) const {
		return Vec3{ x + vec.x,y + vec.y,z + vec.z };
	}

	Vec3 operator +=(Vec3 vec) {
		x += vec.x;
		y += vec.y;
		z += vec.z;
		return *this;
	}

	Vec3 operator +(float num) const {
		return Vec3{ x + num,y + num,z + num };
	}

	Vec3 operator +(int num) const {
		return Vec3{ x + static_cast<float>(num),y + static_cast<float>(num),z + static_cast<float>(num) };
	}

	Vec3 operator +=(float num) {
		x += num;
		y += num;
		z += num;
		return *this;
	}

	Vec3 operator +=(int num) {
		x += static_cast<float>(num);
		y += static_cast<float>(num);
		z += static_cast<float>(num);
		return *this;
	}

	//引き算

	Vec3 operator -() const {
		return Vec3{ -x,-y,-z };
	}

	Vec3 operator -(Vec3 vec) const {
		return Vec3{ x - vec.x,y - vec.y,z - vec.z };
	}

	Vec3 operator -=(Vec3 vec) {
		x -= vec.x;
		y -= vec.y;
		z -= vec.z;
		return *this;
	}

	Vec3 operator -(float num) const {
		return Vec3{ x - num,y - num,z - num };
	}

	Vec3 operator -(int num) const {
		return Vec3{ x - static_cast<float>(num),y - static_cast<float>(num),z - static_cast<float>(num) };
	}

	Vec3 operator -=(float num) {
		x -= num;
		y -= num;
		z -= num;
		return *this;
	}

	Vec3 operator -=(int num) {
		x -= static_cast<float>(num);
		y -= static_cast<float>(num);
		z -= static_cast<float>(num);
		return *this;
	}

	//掛け算
	Vec3 operator *(float num) const {
		return Vec3{ x * num,y * num,z * num };
	}

	Vec3 operator *(int num) const {
		return Vec3{ x * static_cast<float>(num),y * static_cast<float>(num),z * static_cast<float>(num) };
	}

	Vec3 operator *=(float num) {
		x *= num;
		y *= num;
		z *= num;
		return *this;
	}

	Vec3 operator *=(int num) {
		x *= static_cast<float>(num);
		y *= static_cast<float>(num);
		z *= static_cast<float>(num);
		return *this;
	}

	//割り算
	Vec3 operator /(float num) const {
		return Vec3{ x / num,y / num,z / num };
	}

	Vec3 operator /(int num) const {
		return Vec3{ x / static_cast<float>(num),y / static_cast<float>(num),z / static_cast<float>(num) };
	}

	Vec3 operator /=(float num) {
		x /= num;
		y /= num;
		z /= num;
		return *this;
	}

	Vec3 operator /=(int num) {
		x /= static_cast<float>(num);
		y /= static_cast<float>(num);
		z /= static_cast<float>(num);
		return *this;
	}

	//ベクトルの長さの二乗を求める
	float SqLength() const {
		return x * x + y * y + z * z;
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
		z /= len;
	}

	//自信を正規化したベクトルを取得する
	Vec3 GetNormalized() const {
		float len = Length();

		//長さ0以下なら(0,0)を返す
		//引数ありにしている理由
		//引数なしコンストラクタの修正に強くするため
		if (len <= 0.0f) return Vec3{ 0.0f,0.0f,0.0f };

		return Vec3{ x / len,y / len,z / len };
	}

	// 自信をdxlibのVECTORにして返す
	VECTOR VGet() const {
		return DxLib::VGet(x, y, z);
	}

	// 自身や他のベクトルとの内積をとる
	float dot(const Vec3& other) const {
		return x * other.x + y * other.y + z * other.z;
	}

	// 2つのベクトルの内積を求める
	float dot(const Vec3& a, const Vec3& b) const {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	// クロス席を求める
	Vec3 Cross(const Vec3& other) {
		return Vec3{
			y * other.z - z * other.y,
			z * other.x - x * other.z,
			x * other.y - y * other.x
		};
	}
};

