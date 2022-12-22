#pragma once

#include <variant>
#include <string>

using std::string;
using pixel = uint8_t;

struct error {
	string text;
	explicit error (string s) {
		text = s;
	}
};

template<typename T>
using or_error = std::variant<T, error>;

using maybe_error = or_error<std::monostate>;
using ok = std::monostate;

template<typename T>
bool is_error(const or_error<T>& value) noexcept {
	return std::holds_alternative<error>(value);
}

template<typename T>
error get_error(const or_error<T>& value) noexcept {
	return std::get<error>(value);
}

template<typename T>
T get_value(const or_error<T>& value) noexcept {
	return std::get<T>(value);
}

template<typename T>
bool unwrap_to_or_error(const or_error<T>& value, T& dest) noexcept {
	if (is_error(value))
		return true;
	dest = get_value(value);
	return false;
}

template<typename T>
T unwrap_or_print_error(const or_error<T>& value) noexcept {
	if (is_error(value)) {
		std::cerr << get_error(value).text;
		exit(0);
	}
	return get_value(value);
}