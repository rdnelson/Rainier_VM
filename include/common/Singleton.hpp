#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__

template<class T>
class Singleton {

public:
	Singleton() : mSingleton(0) {}
	~Singleton() { delete mSingleton; }

	T* operator()() {
		if(!mSingleton)
			mSingleton = new T();
		return mSingleton;
	}
	T* operator->() {
		if(!mSingleton)
			mSingleton = new T();
		return mSingleton;
	}
private:

	T* mSingleton;
};

#endif
