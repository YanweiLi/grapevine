#ifndef SRC_UTILITY_COMMON_SINGLETON_H_
#define SRC_UTILITY_COMMON_SINGLETON_H_

#include <memory>
#include <mutex>

template <class T>
class Singleton
{
    typedef std::unique_ptr<T>    SingleType;
    typedef Singleton<T>           _This;
public:
    
    static T& instance()
    {
        if(!instance_)
        {
            std::lock_guard<std::mutex> locker (mutex_);
            if(!instance_)
            {
                instance_ = std::make_unique<T>();
            }
        }
        
        return *instance_;
    }
    
    static void free()
    {
        std::lock_guard<std::mutex> locker (mutex_);
        if (0 != instance_.get())
        {
            instance_.reset();
        }
    }
    
protected:
    Singleton(void){}
    ~Singleton<T>(void){}
    
private:
    Singleton(const _This&);
    _This & operator= (const _This &);
    
private:
    static SingleType instance_;
    static std::mutex mutex_;
};

template <class T>
std::unique_ptr<T> Singleton<T>::instance_;

template <class T>
std::mutex Singleton<T>::mutex_;

//Class that will implement the singleton mode,
//must use the macro in it's delare file
#define DECLARE_SINGLETON_CLASS( type ) friend class Singleton< type >
#endif // SRC_UTILITY_COMMON_SINGLETON_H_
