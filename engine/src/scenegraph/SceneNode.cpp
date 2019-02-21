// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#include <storm/engine/scenegraph/DirtyDefs.hpp>
#include <storm/engine/scenegraph/SceneNode.hpp>

using namespace storm::engine;

/////////////////////////////////////
/////////////////////////////////////
SceneNode::SceneNode(Scene &graph, std::string_view name)
      : m_graph{graph}, m_id{next_id++}, m_name{name} {
}

/////////////////////////////////////
/////////////////////////////////////
SceneNode::~SceneNode() = default;

/////////////////////////////////////
/////////////////////////////////////
SceneNode::SceneNode(SceneNode &&) = default;

/////////////////////////////////////
/////////////////////////////////////
SceneNode &SceneNode::operator=(SceneNode &&) = default;

/////////////////////////////////////
/////////////////////////////////////
void SceneNode::addChild(SceneNode &child) {
    child.m_parents.emplace_back(*this);

    auto &child_ref = m_children.emplace_back(child);

    child_ref.get().notify(NodeEvent::ADDED);

    if(observerPtr()) child_ref.get().setObserver(observerPtr());
}

/////////////////////////////////////
/////////////////////////////////////
void SceneNode::removeChild(SceneNode &child) {
    auto it = std::find_if(std::begin(m_children), std::end(m_children), [&](const auto &child_) {
        return child.id() == child_.get().id();
    });

    if(it == std::end(m_children)) return;

    auto parent_it = std::find_if(
      std::begin(child.parents()), std::end(child.parents()), [this](const auto &parent_) {
          return id() == parent_.get().id();
      });

    child.m_parents.erase(parent_it);

    m_children.erase(it);

    child.notify(NodeEvent::REMOVED);
}

////////////////////////////////////////
////////////////////////////////////////
void SceneNode::setObserver(NodeObserver::RawPtr observer_) {
    registerObserver(observer_);

    for(auto &i : m_children) { i.get().setObserver(observerPtr()); }
}

////////////////////////////////////////
////////////////////////////////////////
void SceneNode::notify(NodeEvent event) noexcept {
    Subject::notify(std::move(event), {*this, dirtyValue() | UPDATE_NODE_STATE});
}
