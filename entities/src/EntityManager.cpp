#include <storm/entities/EntityManager.hpp>

using namespace storm::entities;

/////////////////////////////////////
/////////////////////////////////////
EntityManager::EntityManager() : m_next_valid_entity(1) {
}

/////////////////////////////////////
/////////////////////////////////////
EntityManager::~EntityManager() = default;

/////////////////////////////////////
/////////////////////////////////////
Entity EntityManager::makeEntity() {
    m_entities.insert(m_next_valid_entity++);

    return *(--std::end(m_entities));
}

/////////////////////////////////////
/////////////////////////////////////
void EntityManager::destroyEntity(Entity entity) {
    auto it = std::find(std::begin(m_entities), std::end(m_entities), entity);

    if(hasEntity(entity)) {
        auto itMap = m_components.find(entity);
        if(hasComponent(entity)) m_components.erase(itMap);

        m_entities.erase(it);

        removeFromSystems(entity);
    }
}

/////////////////////////////////////
/////////////////////////////////////
bool EntityManager::hasEntity(Entity entity) {
    auto it = std::find(std::begin(m_entities), std::end(m_entities), entity);

    if(it == std::end(m_entities)) return false;

    return true;
}

/////////////////////////////////////
/////////////////////////////////////
bool EntityManager::hasComponent(Entity entity) {
    auto it = m_components.find(entity);

    if(it == std::end(m_components)) return false;

    return true;
}

/////////////////////////////////////
/////////////////////////////////////
bool EntityManager::hasComponent(Entity entity, Component::Type type) {
    auto it = std::find_if(
      std::begin(m_components[ entity ]), std::end(m_components[ entity ]), [type](auto &i) {
          if(i.first == type) return true;

          return false;
      });

    return (it != std::end(m_components[ entity ]));
}

/////////////////////////////////////
/////////////////////////////////////
std::vector<Component *> EntityManager::components(Entity entity) {
    if(!hasEntity(entity) || !hasComponent(entity)) return {};

    std::vector<Component *> vec;
    for(auto &i : m_components[ entity ]) { vec.push_back(i.second.get()); }

    return vec;
}

void EntityManager::preUpdateSystems() {
    for(auto &i : m_systems) i->preUpdate();
}

/////////////////////////////////////
/////////////////////////////////////
void EntityManager::updateSystems(std::uint64_t delta) {
    for(auto &i : m_systems) i->update(delta);
}

/////////////////////////////////////
/////////////////////////////////////
void EntityManager::postUpdateSystems() {
    for(auto &i : m_systems) i->postUpdate();
}

/////////////////////////////////////
/////////////////////////////////////
void EntityManager::purposeToSystems(Entity e) {
    for(auto &s : m_systems) {
        bool is_reliable = true;
        for(auto t : s->componentsUsed()) {
            if(!hasComponent(e, t)) {
                is_reliable = false;
                break;
            }
        }

        if(is_reliable) s->addEntity(e);
    }
}

/////////////////////////////////////
/////////////////////////////////////
void EntityManager::removeFromSystems(Entity e) {
    for(auto &s : m_systems) { s->removeEntity(e); }
}

/////////////////////////////////////
/////////////////////////////////////
void EntityManager::getNeededEntities(System &system) {
    for(auto e : m_entities) {
        bool is_reliable = true;
        for(auto t : system.componentsUsed()) {
            if(!hasComponent(e, t)) {
                is_reliable = false;
                break;
            }
        }

        if(is_reliable) system.addEntity(e);
    }
}
