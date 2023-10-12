#pragma once
#include <type_traits>

template <typename T, typename U>
struct IsDerivedFrom {
	static constexpr bool value = std::is_base_of<T, U>::value;
};

template <typename T, typename U>
using template_extends = std::enable_if_t<IsDerivedFrom<T, U>::value, int>;

/*
Utilisé comme ça :

// .h
template <typename T, template_extends<T, Component> = 0> T* GetComponent();

// .cpp
template <typename T, template_extends<T, Component>>
T* GameObject::GetComponent()
{
	//...
}
*/