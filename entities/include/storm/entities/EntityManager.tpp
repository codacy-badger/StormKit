#include <algorithm>
#include <cassert>
#include <memory>
#include <storm/core/Assert.hpp>
#include <storm/entities/EntityManager.hpp>
#include <storm/entities/System.hpp>

namespace storm::entities {
    template<typename T>
    T &EntityManager::addComponent(Entity entity) {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
        static_assert(T::TYPE != Component::INVALID_TYPE, "T must have T::type defined");

        ASSERT(hasEntity(entity), "entity param need to be existante entity");

        auto component = std::make_unique<T>();

        auto type                      = T::TYPE;
        m_components[ entity ][ type ] = std::move(component);

        purposeToSystems(entity);

        return getComponent<T>(entity);
    }

    template<typename T>
    void EntityManager::destroyComponent(Entity entity) {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
        static_assert(T::TYPE != Component::INVALID_TYPE, "T must have T::type defined");

        ASSERT(hasEntity(entity), "entity param need to be existante entity");
        ASSERT(hasComponent<T>(entity), "this entity don't have this component");

        auto it = std::find_if(
          std::begin(m_components[ entity ]), std::end(m_components[ entity ]), [](auto &i) {
              if(i.first == T::TYPE) return true;

              return false;
          });

        m_components[ entity ].erase(it);
    }

    template<typename T>
    bool EntityManager::hasComponent(Entity entity) {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
        static_assert(T::TYPE != Component::INVALID_TYPE, "T must have T::type defined");

        auto it = std::find_if(
          std::begin(m_components[ entity ]), std::end(m_components[ entity ]), [](auto &i) {
              if(i.first == T::TYPE) return true;

              return false;
          });

        return (it != std::end(m_components[ entity ]));
    }

    template<typename T>
    std::vector<Entity> EntityManager::entitiesWithComponent() {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
        static_assert(T::TYPE != Component::INVALID_TYPE, "T must have T::type defined");

        std::vector<Entity> entities;

        std::for_each(
          std::begin(m_components), std::end(m_components), [&entities](const auto &pair) {
              const auto &map = pair.second;
              auto it = std::find_if(std::begin(map), std::end(map), [](const auto &component) {
                  if(component.first == T::TYPE) return true;

                  return false;
              });

              if(it != std::end(map)) entities.push_back(pair.first);
          });

        return entities;
    }

    template<typename T>
    T &EntityManager::getComponent(Entity entity) {
        static_assert(std::is_base_of<Component, T>::value, "T must be a Component");
        static_assert(T::TYPE != Component::INVALID_TYPE, "T must have T::type defined");

        ASSERT(hasComponent<T>(entity), "this entity doesn't have this component");

        auto type = T::TYPE;
        return *dynamic_cast<T *>(m_components[ entity ][ type ].get());
    }

    template<typename T>
    std::vector<T *> EntityManager::componentsOfType() {
        std::vector<T *> vec;
        for(auto i : m_entities) {
            if(hasComponent<T>(i)) { vec.push_back(getComponent<T>(i)); }
        }

        return vec;
    }

    template<typename T, typename... Args>
    T &EntityManager::addSystem(Args &&... args) {
        std::unique_ptr<System> s = std::make_unique<T>(*this, std::forward<Args &&>(args)...);
        T &ref                    = *dynamic_cast<T *>(s.get());
        getNeededEntities(ref);
        m_systems.insert(std::move(s));
        return ref;
    }
} // namespace storm::entities
