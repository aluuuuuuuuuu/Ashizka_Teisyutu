#include "Constant.h"
#include <fstream>
#include <sstream>
#include <cassert>

std::map<std::string, Constant::ConstantVariant> Constant::GetConstants()
{
	return Constants;
}

int Constant::GetConstantInt(std::string name) const
{
	auto it = Constants.find(name);
	if (it == Constants.end()) {
		throw std::runtime_error("Constant not found: " + name);
	}

	if (auto ptr = std::get_if<int>(&(it->second))) {
		return *ptr;
	}
	else {
		throw std::runtime_error("Constant is not an int: " + name);
	}
}

float Constant::GetConstantFloat(std::string name) const
{
	auto it = Constants.find(name);
	if (it == Constants.end()) {
		throw std::runtime_error("Constant not found: " + name);
	}

	if (auto ptr = std::get_if<float>(&(it->second))) {
		return *ptr;
	}
	else {
		throw std::runtime_error("Constant is not a float: " + name);
	}
}

bool Constant::GetConstatBool(std::string name) const
{
	auto it = Constants.find(name);
	if (it == Constants.end()) {
		throw std::runtime_error("Constant not found: " + name);
	}

	if (auto ptr = std::get_if<bool>(&(it->second))) {
		return *ptr;
	}
	else {
		throw std::runtime_error("Constant is not a bool: " + name);
	}
}

void Constant::ReadCSV(const std::string& filename)
{
	// ファイルを読み込む
	std::ifstream file(filename);

	// 読み込めなかったらエラーを出力する
	assert(file.is_open());

	// 内容を一時的に保存しておく変数
	std::string line, no, name, type, value, content;

	// 一行目のヘッダーをスキップ
	std::getline(file, line);

	while (std::getline(file, line)) {
		std::stringstream ss(line); // 行を得る

		/*定数名と型と値のみmapに保存する*/

		std::getline(ss, no, ',');  // ナンバー
		std::getline(ss, name, ',');    // 定数名
		std::getline(ss, type, ',');    // 定数型
		std::getline(ss, value, ',');   // 定数の値
		std::getline(ss, content, ','); // 説明

		// 例外処理
		try {
			switch (GetDataType(type)) {
			case _INT: {
				int intValue = std::stoi(value);
				Constants[name] = intValue;
				break;
			}
			case _FLOAT: {
				float floatValue = std::stof(value);
				Constants[name] = floatValue;
				break;
			}
			case _BOOL: {
				std::stringstream boolStream(value);
				bool boolValue;
				boolStream >> std::boolalpha >> boolValue;
				Constants[name] = boolValue;
				break;
			}
			case _STRING: {
				Constants[name] = value;
				break;
			}
			}
			//// 変数の型を推測する
			//auto dataType = GetDataType(type);

			//// 型によって処理を分岐させる

			//if (dataType == _INT) {  // int

			//    // 文字列をint型として読みとる
			//    int intValue = std::stoi(value);

			//    // mapに保存する
			//    Constants[name] = intValue;
			//}
			//else if (dataType == _FLOAT) {   // float

			//    // 文字列をfloat型として読み取る
			//    float floatValue = std::stof(value);

			//    // mapに保存する
			//    Constants[name] = floatValue;
			//}
			//else if (dataType == _BOOL) {    // bool

			//    // 文字列を扱いすくする
			//    std::stringstream boolStream(value);
			//    bool boolValue;

			//    // boolalphaでtrueなどの文字をboolに変換する
			//    boolStream >> std::boolalpha >> boolValue;

			//    // mapに保存する
			//    Constants[name] = boolValue;
			//} 
			//else if (dataType == _STRING) {  // string

			//    // ストリングはそのまま保存する
			//    Constants[name] = value;
			//}
		}

		// 例外
		catch (const std::exception& e) {
			// エラーを出力する
			assert(false);
		}
	}
	file.close();
}

DataType Constant::GetDataType(const std::string& type)
{
	// 文字列に応じて変数型を返す
	if (type == "_INT") return _INT;
	if (type == "_FLOAT") return _FLOAT;
	if (type == "_BOOL") return _BOOL;
	if (type == "_STRING") return _STRING;
}
