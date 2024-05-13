#pragma once

#include <utility>
#include <cstdlib>
#include <new>
#include <string>
#include <tuple>

#include "compiler_error.h"

namespace helpers {

	template <typename original_t, typename derivative_t>
	struct maybe_const_maybe_ref {
		static constexpr bool value = false;
	};

	// These two cases are technically useless because the type that we are
	// using this on below will never not have a reference on it.
	// But I like this more when it's more general like this.
	// Also good in case things change in the standard in the future.
	template <typename original_t>
	struct maybe_const_maybe_ref<original_t, original_t> {
		static constexpr bool value = true;
	};

	template <typename original_t>
	struct maybe_const_maybe_ref<original_t, const original_t> {
		static constexpr bool value = true;
	};

	template <typename original_t>
	struct maybe_const_maybe_ref<original_t, original_t&> {
		static constexpr bool value = true;
	};

	template <typename original_t>
	struct maybe_const_maybe_ref<original_t, const original_t&> {
		static constexpr bool value = true;
	};

	template <typename original_t>
	struct maybe_const_maybe_ref<original_t, original_t&&> {
		static constexpr bool value = true;
	};

	template <typename original_t>
	struct maybe_const_maybe_ref<original_t, const original_t&&> {
		static constexpr bool value = true;
	};

	template <typename derivative_t, typename original_t>
	concept maybe_const_maybe_ref_c = maybe_const_maybe_ref<original_t, derivative_t>::value;

	template <typename element_t, maybe_const_maybe_ref_c<element_t> potentially_const_maybe_ref_element_t>
	divec_error_t push_to_c_dynamic_array(element_t *&array, size_t &length, size_t &capacity, potentially_const_maybe_ref_element_t &&new_element) noexcept {

		if (length == capacity) {
			// TODO: make the increment step either variable or constant but a bit bigger
			element_t *new_array = (element_t*)std::realloc(array, (capacity + 1) * sizeof(element_t));
			if (new_array == nullptr) { return divec_error_t::OUT_OF_MEMORY; }

			array = new_array;
			capacity += 1;

			// This is done when resizing the array instead of when adding to the array on purpose.
			// std::vector does it the correct way, where it constructs when you add things to it,
			// instead of when resizing (at least I strongly believe that that's what it does).
			// We CAN'T do that safely, because this array can be used from other code in a multitude of ways,
			// and constructing something more than once is UB.
			// Since these are supposed to be C constructs anyway, and as such don't need to be constructed
			// at specific times, we can do the constructing when we allocate new memory without messing anything
			// up. This method allows us to guarantee that we'll be constructing in a memory location that
			// hasn't been constructed before. If the user is stupid and constructs over our constructions
			// for some reason, then that's on him. He shouldn't do that though because whats the point?
			// The usual use-case doesn't need him to do that, and any other use-case doesn't either because
			// as I said this is supposed to be used with C constructs anyway.
			// Bottom-line, this decision isn't a bad one, I'd say it's definitely better than the alternative.
			for (size_t i = length; i < capacity; i++) { new (array + i) element_t; }
		}

		array[length++] = std::forward<potentially_const_maybe_ref_element_t>(new_element);

		return divec_error_t::SUCCESS;

	}

	template <typename element_t>
	divec_error_t resize_c_dynamic_array(element_t *&array,
					     size_t old_capacity,
					     size_t new_capacity) noexcept
	{

		// TODO: Think through this function again, but I think given the constraints this should be the best solution.

		// Skipping sanity checks in the name of efficiency. This function needs to be fast.

		for (size_t i = new_capacity; i < old_capacity; i++) { array[i].~element_t(); }

		element_t *new_array = (element_t*)std::realloc(array, new_capacity);
		if (new_array == nullptr) {
			if (new_capacity < old_capacity) { return divec_error_t::CATASTROPHIC_FAILURE; }
			return divec_error_t::OUT_OF_MEMORY;
		}
		array = new_array;		// <-- super important, don't let anything use the old array past realloc!

		for (size_t i = old_capacity; i < new_capacity; i++) { new (array + i) element_t; }

		return divec_error_t::SUCCESS;

	}

	template <typename element_t>
	divec_error_t free_c_dynamic_array(element_t *array, size_t capacity) noexcept {
		for (size_t i = 0; i < capacity; i++) { array[i].~element_t(); }
		std::free(array);
		return divec_error_t::SUCCESS;
	}

	divec_error_t convert_size_t_to_string(size_t input, std::string &output) noexcept;

	std::tuple<size_t, size_t> get_line_column_from_text_position(const char *text, size_t position) noexcept;

}
