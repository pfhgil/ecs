//
// Created by Ilya on 03.06.2024.
//

#ifndef ECS_TRAITS_H
#define ECS_TRAITS_H


#include <cstdint>

#if defined _MSC_VER
#   define GENERATOR_PRETTY_FUNCTION __FUNCSIG__
#elif defined __clang__ || (defined __GNUC__)
#   define GENERATOR_PRETTY_FUNCTION __PRETTY_FUNCTION__
#endif

using entity_t = std::uint32_t;
using component_type_id_t = std::uint32_t;

unsigned constexpr constexpr_hash(char const* input)
{
    return *input ?
           static_cast<unsigned int>(*input) + 33 * constexpr_hash(input + 1) :
           5381;
}

template<typename T>
constexpr component_type_id_t get_type_id()
{
    return constexpr_hash(GENERATOR_PRETTY_FUNCTION);
}

template<typename T>
struct type_holder
{
    using type = T;
};

template<typename... Types, typename FuncT>
void for_types(FuncT&& func)
{
    (func(type_holder<Types>{ }), ...);
}

#endif //ECS_TRAITS_H
