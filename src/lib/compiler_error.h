#pragma once

enum class divec_error_t : uint8_t {
	SUCCESS,
	OUT_OF_MEMORY,
	ALREADY_DONE,
	BUILD_ERROR,
	BIND_LIB_FAILED		// Not used in shared lib, used in static lib for bindings. It's here to make sure error codes stay synchronized.
};
