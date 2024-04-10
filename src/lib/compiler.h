#pragma once

#include "compiler_error.h"
#include "build_log.h"
#include "program.h"

divec_error_t diveCompileProgram_inner(dive_program_t program, dive_build_log_t build_log) noexcept;
