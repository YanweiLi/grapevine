//
//  non_copyable.h
//  grapevine
//
//  Created by li_yanwei on 2016/10/21.
//  Copyright © 2016年 li_yanwei. All rights reserved.
//

#ifndef CPP_0X_NON_COPYABLE_H_
#define CPP_0X_NON_COPYABLE_H_

namespace cpp0x
{
    class noncopyable
    {
    public:
        noncopyable(){}
        
    private:
        noncopyable(noncopyable const&) = delete;
        noncopyable& operator= (noncopyable const&) = delete;
    };
}

#endif /* CPP_0X_NON_COPYABLE_H_ */
