#include "translate_gizmo.h"
#include "components/transform_2d_component.h"
#include "halley/core/game/scene_editor_interface.h"
#include "halley/core/graphics/painter.h"
using namespace Halley;

void TranslateGizmo::update(Time time, const SceneEditorInputState& inputState)
{
	hover = (inputState.mousePos - pos).length() < 5.0f / getZoom();

	if (hover && inputState.leftClickPressed) {
		holding = true;
		startOffset = pos - inputState.mousePos;
	}

	if (holding) {
		pos = inputState.mousePos + startOffset;
		if (!inputState.leftClickHeld) {
			holding = false;
		}
	}
}

void TranslateGizmo::draw(Painter& painter) const
{
	if (visible) {
		const float zoom = getZoom();
		painter.drawCircle(pos, 5.0f / zoom, 1.0f / zoom, hover ? Colour4f(1, 1, 1) : Colour4f(0.5f, 0.5f, 0.5f));
	}
}

void TranslateGizmo::onEntityChanged()
{
	auto transform = getTransform();
	if (transform) {
		pos = transform->getGlobalPosition();
		visible = true;
	} else {
		visible = false;
	}
}

