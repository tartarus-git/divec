#pragma once

// We DON'T set the underlying type on purpose, because
// we want to be able to add a bunch more error codes in the future
// without changing the type. Changing the type could break the code
// of the user of this library, which is bad.
// TODO: Maybe we should set the type, just set it to uint32_t though, right? Whats wrong with that?
enum class divec_error_t {
	SUCCESS,
	OUT_OF_MEMORY,
	ALREADY_DONE,
	OBJECT_NOT_READY,
	BUILD_ERROR,
	// Even if the next two end up not being used in the dynamic lib code, keep them,
	// to make sure error codes between static and dynamic lib don't get unsynchronized.
	GENERAL_FAILURE,
	CATASTROPHIC_FAILURE	// for meaning of this error code, check divec.h header.
};
