#include <storm/engine/graphics/ProgramPool.hpp>

namespace storm::engine {
	static tools::ResourcesPool<std::string, Program> program_pool;

	tools::ResourcesPool<std::string, Program> &ProgramPool() {
		return program_pool;
	}
}
