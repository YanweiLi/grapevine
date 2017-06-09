#ifndef __SRLZ_STANDIN_H_77BA134C_33D3_469B_B1F1_684B631B09E7__
#define __SRLZ_STANDIN_H_77BA134C_33D3_469B_B1F1_684B631B09E7__


#include "./srlz_base.h"

NS_SERIALIZATION_LITE_BEGIN

class standin
{
public:
	// inline static const char* static_type() {return "";}
	virtual const char* type() const = 0;
	virtual ~standin(){}
};

template<typename T>
std::shared_ptr<T> standin_cast(std::shared_ptr<standin> const& src)
{
	if (src && strcmp(src->type(), T::static_type()) == 0)
		return std::static_pointer_cast<T>(src);
	else
		return std::shared_ptr<T>();
}


NS_SERIALIZATION_LITE_END


#endif//__SRLZ_STANDIN_H_77BA134C_33D3_469B_B1F1_684B631B09E7__
