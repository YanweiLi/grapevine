#ifndef SRC_XPARAM_MODEL_XINRES_INTERFACE_H_
#define SRC_XPARAM_MODEL_XINRES_INTERFACE_H_

namespace xparam
{
    class XInRes
    {
    protected:
        virtual ~XInRes(){};
        virtual void addref() const = 0;
        virtual void release() const = 0;
        friend void intrusive_ptr_add_ref(XInRes* p);
        friend void intrusive_ptr_release(XInRes* p);
    };

    inline void intrusive_ptr_add_ref(XInRes* p)
    {
        p->addref();
    }
    inline void intrusive_ptr_release(XInRes* p)
    {
        p->release();
    }

} // namespace xparam


#endif  // SRC_XPARAM_MODEL_XINRES_INTERFACE_H_