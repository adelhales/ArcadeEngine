// Copyright (c) 2025 Adel Hales

#pragma once

#include <type_traits>

template <class E> // requires std::is_enum_v<E>
class Flags
{
private:
    using T = std::underlying_type_t<E>;
    T flags_ = 0;

public:
    void Set(E flag)        { flags_ |=  (T)flag; }
    void Unset(E flag)      { flags_ &= ~(T)flag; }
    void Toggle(E flag)     { flags_ ^=  (T)flag; }
    bool Has(E flag) const  { return flags_ & (T)flag; }
    void Clear()            { flags_ = 0; }
};