// Copyright (C) 2019 Arthur LAURENT <arthur.laurent4@gmail.com>
// This file is subject to the license terms in the LICENSE file
// found in the top-level of this distribution

#pragma once


#include <storm/engine/render/ForwardDeclarations.hpp>
#include <storm/engine/render/CommandBuffer.hpp>
#include <storm/engine/render/VertexBuffer.hpp>
#include <storm/engine/render/IndexBuffer.hpp>
#include <storm/engine/graphics/Geometry.hpp>

namespace storm::engine {
	struct Drawable {
		void setGeometry(const Geometry &geometry) {
			this->geometry = geometry;
		}
		
		void setGeometry(Geometry &&geometry) {
			this->geometry = std::move(geometry);
		}
		
		void setProgram(Program &program) {
			this->program = std::addressof(program);
		}
		
		Geometry              geometry;
		Program              *program = nullptr;
		
		PipelineState pipeline_state;
		BindingState binding_state;
		
		friend struct std::hash<storm::engine::Drawable>;
	};
}
