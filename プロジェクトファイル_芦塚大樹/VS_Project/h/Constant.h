#pragma once
#include <map>
#include <string>
#include <variant>
#include <tchar.h>

#define INT_ 0
#define FLOAT_ 0.0f
#define BOOL_ false

// •Ï”Œ^
enum DataType { _INT, _FLOAT, _BOOL, _STRING };

// ’è”ŠÇ—ƒNƒ‰ƒX
class Constant
{
	// ’è”‚ğŠi”[‚·‚é‚½‚ß‚ÌŒ^
	using ConstantVariant = std::variant<int, float, bool, std::string>;

public:

	/// <summary>
	/// ’è”map‚ğ‚»‚Ì‚Ü‚Ü•Ô‚·
	/// </summary>
	/// <returns>’è”map”z—ñ</returns>
	std::map<std::string, ConstantVariant> GetConstants();

	/// <summary>
	/// intŒ^‚Ì’è”‚ğ•Ô‚·
	/// </summary>
	/// <param name="name">’è”–¼</param>
	/// <returns>’è”‚Ì’l</returns>
	int GetConstantInt(std::string name) const;

	/// <summary>
	/// floatŒ^‚Ì’è”‚ğ•Ô‚·
	/// </summary>
	/// <param name="name">’è”–¼</param>
	/// <returns>’è”‚Ì’l</returns>
	float GetConstantFloat(std::string name) const;

	/// <summary>
	/// boolŒ^‚Ì’è”‚ğ•Ô‚·
	/// </summary>
	/// <param name="name">’è”–¼</param>
	/// <returns>’è”‚Ì’l</returns>
	bool GetConstatBool(std::string name) const;

protected:

	/// <summary>
	/// CSV‚©‚ç’è”‚ğ“Ç‚İ‚Ş
	/// </summary>
	/// <param name="filename">ƒtƒ@ƒCƒ‹ƒpƒX</param>
	void ReadCSV(const std::string& filename);

private:

	/// <summary>
	/// ˆø”‚Ì•¶š—ñ‚©‚ç•Ï”Œ^‚ğ„‘ª‚·‚é
	/// </summary>
	/// <param name="type">_INT‚È‚Ç‚ÌŒ^–¼‚Ì•¶š—ñ</param>
	/// <returns>DataType‚Ìƒf[ƒ^Œ^‚Ì‚¢‚¸‚ê‚©</returns>
	DataType GetDataType(const std::string& type);

	// ’è”‚ğŠi”[‚·‚émap”z—ñ
	std::map<std::string, ConstantVariant> Constants;
};
