#ifndef __SRLZ_SMARTPTR_H_B1E139F7_07BE_4D8F_9AF8_AC2BCDE55F42__
#define __SRLZ_SMARTPTR_H_B1E139F7_07BE_4D8F_9AF8_AC2BCDE55F42__

#include "./srlz_base.h"
#include "./srlz_ptr.h"
#include <memory>

NS_SERIALIZATION_LITE_BEGIN

//////////////////////////////////////////////////////////////////////////
// std::auto_ptr

template <class Archive, class T>
inline void serialize_load(Archive const& ar, std::auto_ptr<T>& sp)
{
	T* p = sp.get();
		serialize_pointer_load((T*)0, ar, p);
	if (sp.get() != p)
	{
		ptr_creator<T>::destroy(sp.get());
		sp.reset(p);
	}
}

template <class Archive, class T>
inline void serialize_save(Archive& ar, std::auto_ptr<T> const& sp)
{
	serialize_pointer_save((T*)0, ar, sp.get());
}

//////////////////////////////////////////////////////////////////////////
// std::shared_ptr

template <long Specialized> struct shared_ptr_replacer;
template<> struct shared_ptr_replacer<1>
{
	template <typename T>
	inline static void exec(std::shared_ptr<T>& sp, T* p)
	{
		sp.reset(p, ptr_creator<T>::destroy);
	}
};
template<> struct shared_ptr_replacer<0>
{
	template <typename T>
	inline static void exec(std::shared_ptr<T>& sp, T* p)
	{
		sp.reset(p);
	}
};
template <class Archive, class T>
inline void serialize_load(Archive const& ar, std::shared_ptr<T>& sp)
{
	T* p = sp.get();
	serialize_pointer_load((T*)0, ar, p);
	if (sp.get() != p)
		shared_ptr_replacer<ptr_creator<T>::specialized>::exec(sp, p);
}

template <class Archive, class T>
inline void serialize_save(Archive& ar, std::shared_ptr<T>const& sp)
{
	serialize_pointer_save((T*)0, ar, sp.get());
}
//
////////////////////////////////////////////////////////////////////////////
//// intrusive_ptr
//template <class Archive, class T>
//inline void serialize_load(Archive const& ar, std::intrusive_ptr<T>& sp)
//{
//	// 指针替换规范：serialize_load所直接或间接调用的函数，不得删除原指针，
//	// 只能留给serialize_load来删除，主要是智能指针的删除方法完全不同。
//	T* p = sp.get();
//	serialize_pointer_load((T*)0, ar, p);
//	if (sp.get() != p)
//		sp.swap(std::intrusive_ptr<T>(p, false));
//}
//
//template <class Archive, class T>
//inline void serialize_save(Archive& ar, std::intrusive_ptr<T>const& sp)
//{
//	serialize_pointer_save((T*)0, ar, sp.get());
//}

//////////////////////////////////////////////////////////////////////////
// unique_ptr

template <class Archive, class T>
inline void serialize_load(Archive const& ar, std::unique_ptr<T>& sp)
{
	T* p = sp.get();
	serialize_pointer_load((T*)0, ar, p);
	if (sp.get() != p)
	{
		ptr_creator<T>::destroy(sp.get());
		sp.reset(p);
	}
}

template <class Archive, class T>
inline void serialize_save(Archive& ar, std::unique_ptr<T> const& sp)
{
	serialize_pointer_save((T*)0, ar, sp.get());
}


NS_SERIALIZATION_LITE_END
#endif//__SRLZ_SMARTPTR_H_B1E139F7_07BE_4D8F_9AF8_AC2BCDE55F42__
