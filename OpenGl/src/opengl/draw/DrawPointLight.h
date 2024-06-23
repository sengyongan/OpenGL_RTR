#pragma once
#include"opengl/renderer/Draw.h"

namespace Opengl {
	class DrawPointLight : public Draw {
	public:
		virtual ~DrawPointLight() {}

		virtual void Bind() const override;//绑定顶点数据
		virtual void OnDraw(const std::shared_ptr<Shader>& shader) const override;//绘制

	};
}