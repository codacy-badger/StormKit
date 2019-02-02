#pragma once
#include <storm/core/NonCopyable.hpp>

#include <cstdint>

#include <storm/core/Platform.hpp>
#include <storm/core/Memory.hpp>
#include <storm/entities/Component.hpp>
#include <storm/entities/Entity.hpp>

#include <set>

namespace storm::entities {
	class EntityManager;
	class STORM_PUBLIC System : public storm::core::NonCopyable {
		public:
			Shared_Object(System)
			Unique_Object(System)

			explicit System(EntityManager &manager, std::uint32_t priority, std::set<Component::Type> &&types);
			explicit System(EntityManager &manager, std::uint32_t priority, const std::set<Component::Type> &types);

			System(System &&) = default;
			System &operator=(System &&) = default;

			virtual ~System() = default;

			virtual void preUpdate();
			virtual void update(std::uint64_t delta) = 0;
			virtual void postUpdate();

			inline std::uint32_t priority() const noexcept { return m_priority; }
			inline const std::set<Component::Type> &componentsUsed() const noexcept { return m_types; }

			void addEntity(Entity e);
			void removeEntity(Entity e);

			struct Predicate {
                bool operator()(const System::Ptr &s1, const System::Ptr  &s2) const noexcept {
					return s1->priority() < s2->priority();
				}
			};

		protected:
			EntityManager &m_manager;
			std::set<Entity> m_entities;

		private:
			std::uint32_t m_priority;
			std::set<Component::Type> m_types;
	};
}
