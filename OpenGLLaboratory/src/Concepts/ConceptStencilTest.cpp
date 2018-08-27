#include "ConceptStencilTest.h"
#include "../../external_files/ImGUI/imgui.h"
#include "../Renderer.h"

namespace olab
{
	namespace concepts
	{
		ConceptStencilTest::ConceptStencilTest()
		{
			GLCall(glEnable(GL_STENCIL_TEST));
		}

		void ConceptStencilTest::OnImGuiRender()
		{
			ConceptModelViewProjection::OnImGuiRender();

			ImGui::Checkbox("Select Object", &cubeSelected);

		}

		void ConceptStencilTest::OnUpdate(float _deltaTime)
		{
			GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
			ConceptModelViewProjection::OnUpdate(_deltaTime);

		}
	}
}
