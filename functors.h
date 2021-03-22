#pragma once

template<typename R = void, typename... args>
class FuncS
{
public:
    FuncS(R(*f)(args...) = NULL)
    {
        fptr = f;
    }
    R operator()(args... arg)
    {
        return (*fptr)(arg...);
    }
private:
    R(*fptr)(args...) = NULL;
};

template<typename T, typename R = void, typename... args>
class Func
{
public:
    Func(T* obj = NULL, R(T::*f)(args...) = NULL)
    {
        _obj = obj;
        fptr = f;
    }
    R operator()(args... arg)
    {
        return (_obj->*fptr)(arg...);
    }
private:
    R(T::*fptr)(args...) = NULL;
    T* _obj = NULL;
};
