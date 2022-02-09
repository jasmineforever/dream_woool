#pragma once
template<typename T>
class Singleton
{
public:
	static T& GetInstance()
	{
		static T value;
		return value;
	}
	virtual ~Singleton(){}
protected:
	Singleton(){}
};


