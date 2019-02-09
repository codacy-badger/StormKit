#include "GeoInstancedNode.hpp"

namespace storm::engine {
/*	template <typename T>
	std::uint32_t GeoInstancedNode::addInstance(T &&transform) {
		static_assert(std::is_same_v<std::decay_t<T>, Transform>, "T need to be a Transform");

		auto instance = std::make_shared<Instance>(Instance{ std::forward<T>(transform) });

		auto id = m_next_instance_id++;
		m_instances.emplace(id, std::move(instance));

		return id;
	}

	template <typename T, typename M>
	std::uint32_t GeoInstancedNode::addInstance(T &&transform, M &&material) {
		static_assert(std::is_same_v<std::decay_t<T>, Transform>, "T need to be a Transform");
		static_assert(std::is_same_v<std::decay_t<M>, Material>, "M need to be a Material");

		auto instance = std::make_shared<Instance>(Instance{ std::forward<T>(transform), std::forward<M>(material) });
		auto id = m_next_instance_id++;
		m_instances.emplace(id, std::move(instance));

		return id;
	}

	template <typename T>
	void GeoInstancedNode::updateInstance(std::uint32_t id, T &&transform) {
		static_assert(std::is_same_v<std::decay_t<T>, Transform>, "T need to be a Transform");

		auto &instance = m_instances.at(id);

		instance->transform = std::forward<T>(transform);
	}

	template <typename T, typename M>
	void GeoInstancedNode::updateInstance(std::uint32_t id, T &&transform, M &&material) {
		static_assert(std::is_same_v<std::decay_t<T>, Transform>, "T need to be a Transform");
		static_assert(std::is_same_v<std::decay_t<M>, Material>, "M need to be a Material");

		auto &instance = m_instances.at(id);

		instance->transform = std::forward<T>(transform);
		instance->material  = std::forward<M>(material);
	}*/
}
