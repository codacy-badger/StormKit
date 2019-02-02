#include <storm/entities/System.hpp>
#include <storm/entities/EntityManager.hpp>


using namespace storm::entities;

System::System(storm::entities::EntityManager &manager, std::uint32_t priority, std::set<Component::Type> &&types)
	: m_manager(manager), m_priority(priority), m_types(std::move(types)) {

}
System::System(storm::entities::EntityManager &manager, std::uint32_t priority, const std::set<Component::Type> &types)
	: m_manager(manager), m_priority(priority), m_types(types) {

}

void System::preUpdate() {

}

void System::postUpdate() {

}

void System::addEntity(Entity e) {
	m_entities.insert(e);
}

void System::removeEntity(Entity e) {
	m_entities.erase(e);
}
