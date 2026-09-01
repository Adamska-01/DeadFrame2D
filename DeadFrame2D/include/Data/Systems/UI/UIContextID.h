#pragma once
#include <cstdint>


// TODO: Instead of using these incremented IDs, why not using uintptr_t? uintptr_t should be unique,
// thus thread-safe unlike incremented IDs... (this pattern is used in other parts of the codebase,
// so do some research and evaluate if it is a valid option to use uintptr_t).
namespace DF2D::Data
{
	/** @brief Opaque handle to a UI context (one per Canvas). 0 means invalid. */
	using UIContextID = uint32_t;
}