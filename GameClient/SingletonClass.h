#pragma once

/// <summary>
/// 상속받는 클래스는 싱글톤이 된다.
/// </summary>
/// <typeparam name="T">싱글톤으로 지정할 타입</typeparam>
template <typename T>
class SingletonClass
{
private:
	static T* _instance;

	static void Destroy();
protected:
	SingletonClass();

public:
	static T* GetInstance();
};

template <typename T>
T* SingletonClass<T>::_instance = nullptr;

template<typename T>
inline SingletonClass<T>::SingletonClass()
{
	atexit(Destroy);
}

template<typename T>
inline T* SingletonClass<T>::GetInstance()
{
	if (!_instance)
		_instance = new T;
	return _instance;
}

template<typename T>
inline void SingletonClass<T>::Destroy()
{
	if (_instance)
	{
		delete _instance;
		_instance = nullptr;
	}
}

