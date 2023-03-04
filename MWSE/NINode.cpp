#include "NINode.h"

#include "NIDynamicEffect.h"

namespace NI {
	const auto NI_Node_ctor = reinterpret_cast<void(__thiscall*)(const Node*)>(0x6C81E0);

	Node::Node() {
		NI_Node_ctor(this);
	}

	Node::~Node() {
		// Call dtor without deletion.
		vTable.asObject->destructor(this, 0);
	}

	void Node::attachChild(AVObject * child, bool useFirstAvailable) {
		vTable.asNode->attachChild(this, child, useFirstAvailable);
	}

	Pointer<AVObject> Node::detachChild(AVObject * child) {
		Pointer<AVObject> result;
		vTable.asNode->detachChild(this, &result, child);
		return result;
	}

	Pointer<AVObject> Node::detachChildAt(unsigned int index) {
		Pointer<AVObject> result;
		vTable.asNode->detachChildAt(this, &result, index);
		return result;
	}

	Pointer<AVObject> Node::setChildAt(unsigned int index, AVObject* child) {
		Pointer<AVObject> result;
		vTable.asNode->setChildAt(this, &result, index, child);
		return result;
	}

	Pointer<Node> Node::create() {
		return new Node();
	}

	void Node::detachAllChildren() {
		for (auto i = 0u; i < children.size(); ++i) {
			detachChildAt(i);
		}
	}

	const auto NI_Node_AttachEffect = reinterpret_cast<void(__thiscall*)(Node*, DynamicEffect*)>(0x6C91E0);
	void Node::attachEffect(DynamicEffect * effect ) {
		NI_Node_AttachEffect(this, effect);
	}

	const auto NI_Node_DetachEffect = reinterpret_cast<void(__thiscall*)(Node*, DynamicEffect*)>(0x6C9220);
	void Node::detachEffect(DynamicEffect* effect) {
		NI_Node_DetachEffect(this, effect);
	}

	const auto NI_Node_DetachAllEffects = reinterpret_cast<void(__thiscall*)(Node*)>(0x6C92C0);
	void Node::detachAllEffects() {
		NI_Node_DetachAllEffects(this);
	}

	Pointer<DynamicEffect> Node::getEffect(int type) {
		auto effectNode = &effectList;
		while (effectNode) {
			if (effectNode->data && effectNode->data->getType() == type) {
				return effectNode->data;
			}

			effectNode = effectNode->next;
		}

		return nullptr;
	}

	void Node::attachChild_lua(AVObject* child, sol::optional<bool> useFirstAvailable) {
		attachChild(child, useFirstAvailable.value_or(false));
		updateProperties();
	}

	Pointer<AVObject> Node::detachChildAt_lua(size_t index) {
		if (index == 0) {
			throw std::invalid_argument("This function is 1-indexed. Cannot accept a param less than 1.");
		}
		return detachChildAt(index - 1);
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Node)
