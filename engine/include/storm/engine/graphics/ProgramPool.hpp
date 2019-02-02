#pragma once

#include <storm/engine/render/Program.hpp>

#include <storm/tools/ResourcesPool.hpp>

namespace storm::engine {
	STORM_PUBLIC tools::ResourcesPool<std::string, Program> &ProgramPool();
}
