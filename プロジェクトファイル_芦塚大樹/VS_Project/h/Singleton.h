#pragma once

// クラスをシングルトンパターンで実装するテンプレート
template<typename T>
class Singleton 
{
public:

    // インスタンスを提供する

    /// <summary>
    /// 継承先のインスタンスを返す
    /// </summary>
    /// <returns>インスタンス</returns>
    static T& GetInstance() {
        static T instance;
        return instance;
    }

protected:
    
    // インスタンスのコピーを制限する
    Singleton(const Singleton&) = delete;
    void operator=(const Singleton&) = delete;

    // コンストラクタとデストラクタの外からのアクセスを制限する
    Singleton() {}
    ~Singleton() {}
};