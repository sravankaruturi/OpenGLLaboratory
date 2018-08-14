#include "ConceptClearColour.h"
#include "../Renderer.h"
#include "../../external_files/ImGUI/imgui.h"

namespace olab {
	namespace concepts {



		ConceptClearColour::ConceptClearColour()
			: colour{0.2f, 0.2f, 0.8f, 1.0f}
		{

		}

		ConceptClearColour::~ConceptClearColour()
		{

		}

		void ConceptClearColour::OnUpdate(float _deltaTime)
		{

		}

		void ConceptClearColour::OnRender()
		{
			GLCall(glClearColor(colour[0], colour[1], colour[2], colour[3]));
			GLCall(glClear(GL_COLOR_BUFFER_BIT));
		}

		void ConceptClearColour::OnImGuiRender()
		{
			ImGui::ColorEdit3("Clear Colour ", colour);
		}

	}
}