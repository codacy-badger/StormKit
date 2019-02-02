#pragma once
#include <storm/core/Platform.hpp>
#include <storm/entities/Entity.hpp>
#include <storm/entities/System.hpp>
#include <storm/entities/Component.hpp>
#include <storm/core/NonCopyable.hpp>

#include <map>
#include <vector>
#include <memory>
#include <set>

namespace storm::entities {
	class STORM_PUBLIC EntityManager final : public storm::core::NonCopyable {
		public:
			explicit EntityManager();

			EntityManager(EntityManager &&) = default;
			EntityManager &operator=(EntityManager &&) = default;

			~EntityManager();

			Entity makeEntity();
			void destroyEntity(Entity entity);
			bool hasEntity(Entity entity);
			bool hasComponent(Entity entity);

			template <typename T>
			T &addComponent(Entity entity);

			template <typename T>
			void destroyComponent(Entity entity);

			template <typename T>
			bool hasComponent(Entity entity);

			bool hasComponent(Entity entity, Component::Type type);

			template <typename T>
			std::vector<Entity> entitiesWithComponent();

			template <typename T>
			T &getComponent(Entity entity);

			std::vector<Component*> components(Entity entity);

			template <typename T>
			std::vector<T*> componentsOfType();

			template <typename T, typename... Args>
			T &addSystem(Args&&... args);


			void preUpdateSystems();
			void updateSystems(std::uint64_t delta);
			void postUpdateSystems();

			inline std::size_t numberOfEntities() const noexcept { return m_entities.size(); }
		private:
			void purposeToSystems(Entity e);
			void removeFromSystems(Entity e);
			void getNeededEntities(System &system);

			Entity m_next_valid_entity;
			std::set<Entity> m_entities;
            std::set<System::Ptr, System::Predicate> m_systems;
            std::map<Entity, std::map<Component::Type, Component::Ptr>> m_components;
	};
}

#include "EntityManager.tpp"
