#pragma once

#include "compiler.h"

// TODO: Dirty to put this here, because deadlock header dependencies. Come up with a solution.
divec_error_t divePreprocessProgram(dive_program_t program, dive_build_log_t build_log) noexcept;
