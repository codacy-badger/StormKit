#pragma once

#include <vector>
#include <storm/engine/graphics/Vertex.hpp>
#include <storm/engine/scenegraph/Node.hpp>

#include <storm/engine/graphics/Instance.hpp>

namespace storm::engine {
	class STORM_PUBLIC GeoInstancedNode : public Node {
		public:
			SUR_Object(GeoInstancedNode)

			/*using MapInstance = std::unordered_map<std::uint32_t,std::shared_ptr<Instance>>;
			using HoldedType = MapInstance;

			~GeoInstancedNode() override = default;

			template <typename T>
			std::uint32_t addInstance(T &&transform);
			template <typename T, typename M>
			std::uint32_t addInstance(T &&transform, M &&material);

			template <typename T>
			void updateInstance(std::uint32_t id, T &&transform);
			template <typename T, typename M>
			void updateInstance(std::uint32_t id, T &&transform, M &&material);

			void removeInstance(std::uint32_t id);

			inline const HoldedType &data() const noexcept { return instances(); }
			inline const HoldedType &instances() const noexcept { return m_instances; }
			void clear();

			inline void commit() { notify(NodeEvent::UPDATED); }
		protected:
			explicit GeoInstancedNode();

			std::uint32_t dirtyValue() const noexcept override;

		private:
			HoldedType m_instances;
			std::uint32_t m_next_instance_id;*/
	};
}

#include "GeoInstancedNode.tpp"
