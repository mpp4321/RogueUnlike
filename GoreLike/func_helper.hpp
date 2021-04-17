#pragma once

#include "entt.hpp"

template<typename T>
auto create_delegate(T f) {
	entt::delegate<T> d{};
	d.connect(f);
	return d;
}
