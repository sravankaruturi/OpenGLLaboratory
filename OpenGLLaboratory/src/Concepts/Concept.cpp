#include "Concept.h"
#include "../../external_files/ImGUI/imgui.h"

namespace olab {
	namespace concepts {



		ConceptsMenu::ConceptsMenu(Concept*& _conceptPointer)
			: currenConcept(_conceptPointer)
		{

		}

		void ConceptsMenu::OnImGuiRender()
		{
			for (auto& it : concepts) {
				if (ImGui::Button(it.first.c_str())) {
					currenConcept = it.second();
				}
			}
		}

	}
}
