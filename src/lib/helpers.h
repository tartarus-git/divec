#pragma once

#include <utility>
#include <cstdlib>

#include "compiler_error.h"

namespace helpers {

	template <typename original_t, typename derivative_t>
	struct maybe_const {
		constexpr bool value = false;
	};

	template <typename original_t>
	struct maybe_const<original_t, const original_t>
	struct maybe_const {
		constexpr bool value = true;
	};

	typename <typename original_t, typename derivative_t>
	concept maybe_const_t = maybe_const<original_t, derivative_t>::value;

	template <typename element_t, maybe_const_c<element_t> potentially_const_element_t>
	divec_error_t push_to_c_dynamic_array(element_t *&array, size_t length, size_t capacity, potentially_const_element_t &&new_element) noexcept {

		if (length == capacity) {
			// TODO: make the increment step either variable or constant but a bit bigger
			element_t *new_array = std::realloc(array, (capacity += 1) * sizeof(element_t));
			if (new_array == nullptr) { return divec_error_t::OUT_OF_MEMORY; }

			array = new_array;

			for (size_t i = length; i < capacity; i++) { new (array + i) element_t; }
		}

		array[length++] = std::forward<potentially_const_element_t>(new_element);

		return divec_error_t::SUCCESS;

	}

	template <typename element_t>
	divec_error_t resize_c_dynamic_array(element_t *&array, size_t old_length, size_t new_length, size_t capacity) noexcept {

		// TODO: Think through this function again, but I think given the constraints this should be the best solution.

		for (size_t i = new_length; i < old_length; i++) { array[i].~element_t(); }

		element_t *new_array = std::realloc(array, capacity);
		if (new_array == nullptr) {
			if (new_length < old_length) { return divec_error_t::CORRUPTED_STATE; }
			return divec_error_t::OUT_OF_MEMORY;
		}

		for (size_t i = old_length; i < new_length; i++) { new (array + i) element_t; }

		return divec_error_t::SUCCESS;

	}

	template <typename element_t>
	divec_error_t free_c_dynamic_array(element_t *array, size_t length) noexcept {
		for (size_t i = 0; i < length; i++) { array[i].~element_t(); }
		std::free(array);
	}

}
