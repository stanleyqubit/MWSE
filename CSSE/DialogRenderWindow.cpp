#include "DialogRenderWindow.h"

#include "LogUtil.h"
#include "MathUtil.h"
#include "MemoryUtil.h"
#include "WindowsUtil.h"

#include "NIAVObject.h"
#include "NIBound.h"
#include "NICamera.h"
#include "NICollisionSwitch.h"
#include "NIGeometry.h"
#include "NIMatrix33.h"
#include "NINode.h"
#include "NIPick.h"

#include "CSCell.h"
#include "CSDataHandler.h"
#include "CSGameFile.h"
#include "CSLandTexture.h"
#include "CSRecordHandler.h"
#include "CSReference.h"
#include "CSStatic.h"

#include "Settings.h"
#include "RenderWindowSceneGraphController.h"
#include "RenderWindowSelectionData.h"
#include "RenderWindowWidgets.h"

namespace se::cs::dialog::render_window {
	static WORD lastRenderWindowPosX = 0;
	static WORD lastRenderWindowPosY = 0;

	using gObjectMove = memory::ExternalGlobal<float, 0x6CE9B4>;
	using gObjectRotate = memory::ExternalGlobal<float, 0x6CE9B0>;

	using gCameraPan = memory::ExternalGlobal<float, 0x6CE9C0>;
	using gCameraRotate = memory::ExternalGlobal<float, 0x6CE9B8>;
	using gCameraZoom = memory::ExternalGlobal<float, 0x6CE9BC>;

	using gSnapGrid = memory::ExternalGlobal<int, 0x6CE9A8>;
	using gSnapAngleInDegrees = memory::ExternalGlobal<int, 0x6CE9AC>;
	using gCumulativeRotationValues = memory::ExternalGlobal<NI::Vector3, 0x6CF760>;

	using gRenderWindowPick = memory::ExternalGlobal<NI::Pick, 0x6CF528>;

	using gCurrentCell = memory::ExternalGlobal<Cell*, 0x6CF7B8>;

	using gIsHoldingV = memory::ExternalGlobal<bool, 0x6CF789>;
	using gIsHoldingX = memory::ExternalGlobal<bool, 0x6CF786>;
	using gIsHoldingY = memory::ExternalGlobal<bool, 0x6CF787>;
	using gIsHoldingZ = memory::ExternalGlobal<bool, 0x6CF788>;
	using gIsScaling = memory::ExternalGlobal<bool, 0x6CF785>;

	using gRenderNextFrame = memory::ExternalGlobal<bool, 0x6CF78D>;

	namespace RenderControlFlags {
		enum RenderControlFlags : DWORD {
			SnapToGrid = 0x1,
			SnapToAngle = 0x2,
			AllowRenderWindowCellLoads = 0x4,
			SkipInitialCellLoad = 0x10,
		};
	}
	using gRenderControlFlags = memory::ExternalGlobal<DWORD, 0x6CE9A4>;
	using gAutoSaveTime = memory::ExternalGlobal<int, 0x6CEA38>;

	// Convenience function to see if X, Y, or Z are held down.
	bool isHoldingAxisKey() {
		return gIsHoldingX::get() || gIsHoldingY::get() || gIsHoldingZ::get();
	}

	bool isGridSnapping() {
		return gRenderControlFlags::get() & RenderControlFlags::SnapToGrid;
	}

	bool isAngleSnapping() {
		return gRenderControlFlags::get() & RenderControlFlags::SnapToAngle;
	}

	struct NetImmerseInstance {
		struct VirtualTable {
			void* dtor; // 0x0
			void* setRegistryKey; // 0x4
			void* setWindowWidth; // 0x8
			void* setWindowHeight; // 0xC
			void* setBitDepth; // 0x10
			void* setBackBuffers; // 0x14
			void* setMultisampleEnable; // 0x18
			void* setMultisamples; // 0x1C
			void* setFullScreen; // 0x20
			void(__thiscall* setPixelShaderEnabled)(NetImmerseInstance*, bool); // 0x24
			void* setStencil; // 0x28
			void* setMipmap; // 0x2C
			void* setMipmapSkipLevel; // 0x30
			void* setHardwareTL; // 0x34
			void* setMultipass; // 0x38
			void* setVertexProcessing; // 0x3C
			void* setSwapEffect; // 0x40
			void* setRefreshRate; // 0x44
			void* setPresentInterval; // 0x48
			void* setAdapterID; // 0x4C
			void(__thiscall* setGamma)(NetImmerseInstance*, float); // 0x50
			void* readSettings; // 0x54
			void* writeSettings; // 0x58
			void* createRenderer; // 0x5C
			void* createRendererFromSettings; // 0x60
		};
		VirtualTable* vTable; // 0x0
		const char* ownerString; // 0x4
		int windowWidth; // 0x8
		int windowHeight; // 0xC
		int screenDepth; // 0x10
		int backBuffers; // 0x14
		int multiSamples; // 0x18
		bool fullscreen; // 0x1C
		bool stencil; // 0x1D
		bool mipmap; // 0x1E
		bool hardware; // 0x1F
		bool pixelShader; // 0x20
		bool multiPass; // 0x21
		bool screenShotsEnabled; // 0x22
		int vertexProcessing; // 0x24
		int swapEffect; // 0x28
		int refreshRate; // 0x2C
		int adapter; // 0x30
		int mipmapSkipLevel; // 0x34
		int presentationInterval; // 0x38
		float gamma; // 0x3C
		NI::Vector3 unknown_0x40;
		NI::Pointer<NI::Node> node; // 0x4C
		NI::Renderer* renderer; // 0x50
		int unknown_0x54;
		HWND parentWindow; // 0x58
		int unknown_0x5C;
		int unknown_0x60;
	};
	static_assert(sizeof(NetImmerseInstance) == 0x64, "CS::NetImmerseInstance failed size validation");
	static_assert(sizeof(NetImmerseInstance::VirtualTable) == 0x64, "CS::NetImmerseInstance's virtual table failed size validation");

	//
	// Patch: Use world rotation values unless ALT is held.
	//

	const auto TES3_CS_OriginalRotationLogic = reinterpret_cast<bool(__cdecl*)(void*, SelectionData::Target*, int, SelectionData::RotationAxis)>(0x4652D0);
	bool __cdecl Patch_ReplaceRotationLogic(void* unknown1, SelectionData::Target* firstTarget, int relativeMouseDelta, SelectionData::RotationAxis rotationAxis) {
		using windows::isKeyDown;

		// Allow holding ALT modifier to do vanilla behavior.
		bool useWorldAxisRotation = settings.render_window.use_world_axis_rotations_by_default;
		if (isKeyDown(VK_MENU)) {
			useWorldAxisRotation = !useWorldAxisRotation;
		}

		if (!useWorldAxisRotation) {
			return TES3_CS_OriginalRotationLogic(unknown1, firstTarget, relativeMouseDelta, rotationAxis);
		}

		if (relativeMouseDelta == 0) {
			return false;
		}

		auto selectionData = SelectionData::get();

		if (!isKeyDown('X') && !isKeyDown('Y')) {
			rotationAxis = SelectionData::RotationAxis::Z;
		}

		auto widgets = SceneGraphController::get()->widgets;
		if (isHoldingAxisKey()) {
			widgets->setPosition(selectionData->bound.center);
			widgets->show();
		}

		const auto rotationSpeed = gObjectRotate::get();
		auto& cumulativeRot = gCumulativeRotationValues::get();
		switch (rotationAxis) {
		case SelectionData::RotationAxis::X:
			widgets->setAxis(WidgetsAxis::X);
			cumulativeRot.x += relativeMouseDelta * rotationSpeed * 0.1f;
			break;
		case SelectionData::RotationAxis::Y:
			widgets->setAxis(WidgetsAxis::Y);
			cumulativeRot.y += relativeMouseDelta * rotationSpeed * 0.1f;
			break;
		case SelectionData::RotationAxis::Z:
			widgets->setAxis(WidgetsAxis::Z);
			cumulativeRot.z += relativeMouseDelta * rotationSpeed * 0.1f;
			break;
		}

		const auto snapAngle = math::degreesToRadians((float)gSnapAngleInDegrees::get());
		const bool isSnapping = (isAngleSnapping() || isKeyDown(VK_LCONTROL)) && (snapAngle != 0.0f);

		NI::Vector3 orientation = cumulativeRot;
		if (isSnapping) {
			orientation.x = std::roundf(orientation.x / snapAngle) * snapAngle;
			orientation.y = std::roundf(orientation.y / snapAngle) * snapAngle;
			orientation.z = std::roundf(orientation.z / snapAngle) * snapAngle;
		}

		// Due to snapping these may have been set to 0, in which case no need to do anything else.
		if (orientation.x == 0.0f
			&& orientation.y == 0.0f
			&& orientation.z == 0.0f)
		{
			return 0;
		}

		// Restart accumulating process.
		cumulativeRot.x = 0;
		cumulativeRot.y = 0;
		cumulativeRot.z = 0;

		NI::Matrix33 userRotation;
		userRotation.fromEulerXYZ(orientation.x, orientation.y, orientation.z);

		for (auto target = selectionData->firstTarget; target; target = target->next) {
			auto reference = target->reference;

			reference->setAsEdited();

			// Disallow XY rotations on actors and northmarkers.
			auto doRotations = true;
			if (rotationAxis != SelectionData::RotationAxis::Z && !reference->baseObject->canRotateOnAllAxes()) {
				doRotations = false;
			}

			if (doRotations) {
				auto& oldRotation = *reference->sceneNode->localRotation;
				auto newRotation = userRotation * oldRotation;

				// Slightly modified toEulerXYZ that does not do factorization.
				{
					orientation.y = asin(-newRotation.m0.z);
					if (cos(orientation.y) != 0) {
						orientation.x = atan2(newRotation.m1.z, newRotation.m2.z);
						orientation.z = atan2(newRotation.m0.y, newRotation.m0.x);
					}
					else {
						orientation.x = atan2(newRotation.m2.x, newRotation.m2.y);
						orientation.z = 0;
					};
				}

				if (isSnapping && (target == selectionData->firstTarget)) {
					// Snapping the new rotation after adjustments were applied.
					// So we must only snap the *current* axis and not all them.
					switch (rotationAxis) {
					case SelectionData::RotationAxis::X:
						orientation.x = std::roundf(orientation.x / snapAngle) * snapAngle;
						break;
					case SelectionData::RotationAxis::Y:
						orientation.y = std::roundf(orientation.y / snapAngle) * snapAngle;
						break;
					case SelectionData::RotationAxis::Z:
						orientation.z = std::roundf(orientation.z / snapAngle) * snapAngle;
						break;
					}

					// Ensure the matrix is also snapped.
					newRotation.fromEulerXYZ(orientation.x, orientation.y, orientation.z);

					// Ensure all targets use the snapped rotation.
					userRotation = newRotation * oldRotation.transpose();
				}

				math::standardizeAngleRadians(orientation.x);
				math::standardizeAngleRadians(orientation.y);
				math::standardizeAngleRadians(orientation.z);

				reference->yetAnotherOrientation = orientation;
				reference->orientationNonAttached = orientation;

				NI::Matrix33 referenceHandledRotations;
				reference->updateRotationMatrixForRaceAndSex(referenceHandledRotations);
				reference->sceneNode->setLocalRotationMatrix(&referenceHandledRotations);
			}

			// Rotate positions.
			if (selectionData->numberOfTargets > 1) {
				auto p = reference->position - selectionData->bound.center;
				reference->position = (userRotation * p) + selectionData->bound.center;
				reference->unknown_0x10 = reference->position;
				reference->sceneNode->localTranslate = reference->position;
				DataHandler::get()->updateLightingForReference(reference);
			}

			reference->sceneNode->update(0.0f, true, true);
		}

		return true;
	}

	//
	// Patch: Improve multi-reference scaling.
	//

	void __cdecl Patch_ReplaceScalingLogic(RenderController* renderController, SelectionData::Target* firstTarget, int mouseDelta) {
		using windows::isKeyDown;

		auto selectionData = SelectionData::get();
		bool useGroupScaling = settings.render_window.use_group_scaling;
		if (!useGroupScaling || selectionData->numberOfTargets == 1) {
			const auto VanillaScalingHandler = reinterpret_cast<void(__cdecl*)(RenderController*, SelectionData::Target*, int)>(0x404949);
			VanillaScalingHandler(renderController, firstTarget, mouseDelta);
			return;
		}

		// Clamp such that no reference will be scaled beyond supported bounds. [0.5, 2.0]
		auto delta = mouseDelta * 0.01f;
		for (auto target = firstTarget; target; target = target->next) {
			//const auto scale = target->reference->sceneNode->localScale;
			const auto scale = target->reference->getScale();
			delta = std::clamp(delta, 0.5f - scale, 2.0f - scale);
		}
		if (fabs(delta) <= 1e-5) {
			return;
		}

		// Scale all selected references and their positions relative to selection center.
		const auto center = selectionData->bound.center;
		const auto factor = 1.0 + delta;

		for (auto target = firstTarget; target; target = target->next) {
			auto reference = target->reference;

			// Update scale.
			//reference->sceneNode->localScale *= factor;
			reference->setScale(reference->getScale() * factor);

			// Update position.
			const auto relativePosition = reference->position - center;
			reference->position = center + (relativePosition * factor);

			reference->unknown_0x10 = reference->position;
			reference->sceneNode->localTranslate = reference->position;
			reference->sceneNode->update(0.0f, true, true);

			DataHandler::get()->updateLightingForReference(reference);

			reference->setAsEdited();
		}
		selectionData->recalculateBound();
	}

	//
	// Patch: Fix drop-to-surface (F)
	//

	float __cdecl Patch_FixDropToSurface_GetLowestZ(NI::Node* node) {
		// Force update deforms.
		node->updateWorldDeforms();

		// Overwritten code.
		return node->getLowestVertexZ();
	}

	void __cdecl Patch_FixDropToSurface_GetLowVertices(const NI::Node* node, float nearToZ) {
		constexpr auto NEAR_THRESHOLD = 1.0f;
		const auto gNearVertexArray = *reinterpret_cast<NI::TArray<NI::Vector3*>**>(0x6CF7C4);

		for (const auto& child : node->children) {
			if (child) {
				// Calculate for geometry.
				if (child->isInstanceOfType(NI::RTTIStaticPtr::NiTriBasedGeom)) {
					auto asGeometry = static_cast<const NI::Geometry*>(child.get());

					// Ignore particles.
					if (asGeometry->isInstanceOfType(NI::RTTIStaticPtr::NiParticles)) {
						continue;
					}

					// Add nearby vertices.
					for (auto i = 0u; i < asGeometry->modelData->vertexCount; ++i) {
						const auto vertex = &asGeometry->worldVertices[i];
						if (std::abs(vertex->z - nearToZ) <= NEAR_THRESHOLD) {
							gNearVertexArray->push_back(vertex);
						}
					}

					continue;
				}

				// Recursively call for child nodes.
				if (child->isInstanceOfType(NI::RTTIStaticPtr::NiNode)) {
					auto asNode = static_cast<const NI::Node*>(child.get());
					Patch_FixDropToSurface_GetLowVertices(asNode, nearToZ);
					continue;
				}
			}
		}
	}

	bool __fastcall Patch_FixDropToSurface_PickObjects(NI::Pick* pick, DWORD _EDX_, NI::Vector3* origin, NI::Vector3* direction, bool append, float maxDistance) {
		// Perform pick with skin deforms instead.
		return pick->pickObjectsWithSkinDeforms(origin, direction, append, maxDistance);
	}

	//
	// Patch: Improve moving objects.
	//

	//
	// Patch: Allow alt-dragging objects to snap to surfaces.
	//
	enum class SnappingAxis {
		POSITIVE_X,
		NEGATIVE_X,
		POSITIVE_Y,
		NEGATIVE_Y,
		POSITIVE_Z,
		NEGATIVE_Z,
	};

	SnappingAxis snappingAxis = SnappingAxis::POSITIVE_Z;

	int Patch_AlignToSurfaceDragMovementLogic(RenderController* renderController, SelectionData::Target* firstTarget, int dx, int dy, bool lockX, bool lockY, bool lockZ) {
		using se::math::M_PIf;

		// Currently requires a single selection.
		auto selectionData = SelectionData::get();
		if (selectionData->numberOfTargets != 1) {
			return 0;
		}

		auto rendererPicker = &gRenderWindowPick::get();
		auto rendererController = RenderController::get();
		auto sceneGraphController = SceneGraphController::get();

		// Cache picker settings we care about.
		const auto& previousRoot = rendererPicker->root;
		const auto previousReturnNormal = rendererPicker->returnNormal;
		const auto previousPickType = rendererPicker->pickType;

		// Make changes to the pick that we need to.
		rendererPicker->root = sceneGraphController->sceneRoot;
		rendererPicker->returnNormal = true;
		rendererPicker->pickType = NI::PickType::FIND_ALL;

		auto reference = selectionData->firstTarget->reference;
		reference->sceneNode->setAppCulled(true);

		NI::Vector3 origin;
		NI::Vector3 direction;
		if (rendererController->camera->windowPointToRay(lastRenderWindowPosX, lastRenderWindowPosY, origin, direction)) {
			direction.normalize();

			if (rendererPicker->pickObjects(&origin, &direction)) {
				auto firstResult = rendererPicker->getFirstUnskinnedResult();
				if (firstResult) {
					auto object = reference->baseObject;

					auto refSnappingAxis = snappingAxis;
					if (!object->canRotateOnAllAxes()) {
						refSnappingAxis = SnappingAxis::POSITIVE_Z;
					}

					reference->setAsEdited();

					// Set position.
					NI::Vector3 offset;
					const auto scale = reference->getScale();
					switch (refSnappingAxis) {
					case SnappingAxis::POSITIVE_X:
						offset = firstResult->normal * std::abs(object->boundingBox.min.x) * scale;
						break;
					case SnappingAxis::NEGATIVE_X:
						offset = firstResult->normal * std::abs(object->boundingBox.max.x) * scale;
						break;
					case SnappingAxis::POSITIVE_Y:
						offset = firstResult->normal * std::abs(object->boundingBox.min.y) * scale;
						break;
					case SnappingAxis::NEGATIVE_Y:
						offset = firstResult->normal * std::abs(object->boundingBox.max.y) * scale;
						break;
					case SnappingAxis::POSITIVE_Z:
						offset = firstResult->normal * std::abs(object->boundingBox.min.z) * scale;
						break;
					case SnappingAxis::NEGATIVE_Z:
						offset = firstResult->normal * std::abs(object->boundingBox.max.z) * scale;
						break;
					}

					reference->position = firstResult->intersection + offset;
					reference->unknown_0x10 = reference->position;
					reference->sceneNode->localTranslate = reference->position;

					selectionData->bound.center = reference->position;

					// Set rotation.
					if (object->canRotateOnAllAxes()) {
						NI::Vector3 orientation;
						switch (refSnappingAxis) {
						case SnappingAxis::POSITIVE_X:
						case SnappingAxis::NEGATIVE_X:
							orientation.x = 1.0f;
							break;
						case SnappingAxis::POSITIVE_Y:
						case SnappingAxis::NEGATIVE_Y:
							orientation.y = 1.0f;
							break;
						case SnappingAxis::POSITIVE_Z:
						case SnappingAxis::NEGATIVE_Z:
							orientation.z = 1.0f;
							break;
						}

						NI::Matrix33 rotation;
						rotation.toRotationDifference(orientation, firstResult->normal);

						// Flip axis if it's negative snapping.
						NI::Matrix33 flipMatrix;
						switch (refSnappingAxis) {
						case SnappingAxis::NEGATIVE_X:
							flipMatrix.toRotationY(M_PIf);
							break;
						case SnappingAxis::NEGATIVE_Y:
							flipMatrix.toRotationZ(M_PIf);
							break;
						case SnappingAxis::NEGATIVE_Z:
							flipMatrix.toRotationX(M_PIf);
							break;
						default:
							flipMatrix.toIdentity();
						}
						rotation = rotation * flipMatrix;
						rotation.toEulerXYZ(&orientation);

						// Restore the original Z orientation.
						switch (refSnappingAxis) {
						case SnappingAxis::POSITIVE_Z:
						case SnappingAxis::NEGATIVE_Z:
							orientation.z = reference->yetAnotherOrientation.z;
							rotation.fromEulerXYZ(orientation.x, orientation.y, orientation.z);
							break;
						}
						
						math::standardizeAngleRadians(orientation.x);
						math::standardizeAngleRadians(orientation.y);
						math::standardizeAngleRadians(orientation.z);

						reference->yetAnotherOrientation = orientation;
						reference->orientationNonAttached = orientation;

						reference->sceneNode->setLocalRotationMatrix(&rotation);
					}
					
					reference->sceneNode->update(0.0f, true, true);

					// Update lighting data.
					DataHandler::get()->updateLightingForReference(reference);
				}
			}
		}

		reference->sceneNode->setAppCulled(false);

		// Restore pick settings.
		rendererPicker->clearResults();
		rendererPicker->root = previousRoot;
		rendererPicker->returnNormal = previousReturnNormal;
		rendererPicker->pickType = previousPickType;
		
		return 1;
	}

	struct MovementContext {
		NI::Vector3 basePosition;
		NI::Vector3 cursorOffset;
	};
	static auto movementContext = std::optional<MovementContext>();

	const auto DefaultDragMovementFunction = reinterpret_cast<int(__cdecl*)(RenderController*, SelectionData::Target*, int, int, bool, bool, bool)>(0x464B70);
	int __cdecl Patch_ReplaceDragMovementLogic(RenderController* renderController, SelectionData::Target* firstTarget, int dx, int dy, bool lockX, bool lockY, bool lockZ) {
		using windows::isKeyDown;

		auto selectionData = SelectionData::get();
		auto lastTarget = selectionData->getLastTarget();
		if (!lastTarget) {
			return 0;
		}

		// Respect the "Use Legacy Object Movement" setting.
		if (settings.render_window.use_legacy_object_movement) {
			return DefaultDragMovementFunction(renderController, firstTarget, dx, dy, lockX, lockY, lockZ);
		}

		// When holding alt perform align-to-surface behavior.
		if (isKeyDown(VK_MENU)) {
			return Patch_AlignToSurfaceDragMovementLogic(renderController, firstTarget, dx, dy, lockX, lockY, lockZ);
		}

		// Calculate raycast origin/direction from cursor.
		NI::Vector3 rayOrigin;
		NI::Vector3 rayDirection;
		auto camera = RenderController::get()->camera;
		if (!camera->windowPointToRay(lastRenderWindowPosX, lastRenderWindowPosY, rayOrigin, rayDirection)) {
			return 0;
		}

		// Ensure selection center is correct. Currently some other functions don't update it. (F key)
		selectionData->recalculateCenter();
		
		// Calculate the plane that we will raycast against.
		auto planeOrigin = selectionData->bound.center;
		auto planeNormal = NI::Vector3(0, 0, 1);

		// Preserve the cursor offset and starting position.
		if (!movementContext.has_value()) {
			MovementContext context;
			auto pick = SceneGraphController::get()->objectPick;
			if (pick->pickObjectsWithSkinDeforms(&rayOrigin, &rayDirection)) {
				context.basePosition = pick->results.at(0)->intersection;
				context.cursorOffset = context.basePosition - planeOrigin;
			}
			else {
				context.basePosition = planeOrigin;
				context.cursorOffset = NI::Vector3();
			}
			movementContext = std::move(context);
		}
		auto& context = movementContext.value();
		planeOrigin = planeOrigin + context.cursorOffset;

		// Align the plane to the locked axis if applicable.
		auto widgets = SceneGraphController::get()->widgets;
		bool isAxisLocked = lockX || lockY || lockZ;
		if (isAxisLocked) {
			planeNormal = camera->worldDirection;
			if (lockX) {
				planeNormal.x = 0.0f;
				widgets->setAxis(WidgetsAxis::X);
			}
			if (lockY) {
				planeNormal.y = 0.0f;
				widgets->setAxis(WidgetsAxis::Y);
			}
			if (lockZ) {
				planeNormal.z = 0.0f;
				widgets->setAxis(WidgetsAxis::Z);
			}
			planeNormal.normalize();
			widgets->show();
		}

		// Clamp the plane angle if we're close to parellel with the camera.
		auto fixedNormal = planeNormal;
		auto fixedOrigin = planeOrigin;
		if (!isAxisLocked) {
			auto direction = (context.basePosition - camera->worldTransform.translation);
			direction.normalize();

			const float angle = planeNormal.dotProduct(&direction);
			const float minAngle = se::math::degreesToRadians(15);
			if (fabs(angle) <= minAngle) {
				// Clamping to `minAngle` about the camera X axis.
				NI::Matrix33 m;
				m.toRotation(angle < 0.0f ? -minAngle : minAngle, camera->worldRight);
				fixedNormal = m * planeNormal;
				fixedOrigin = context.basePosition;
			}
		}

		// Calculate the intersection.
		auto [distance, intersection] = math::rayPlaneIntersection(rayOrigin, rayDirection, fixedOrigin, fixedNormal);
		if (distance == -1.0f) {
			return 0;
		}

		// Apply axis restrictions.
		if (!isAxisLocked) {
			intersection.z = planeOrigin.z;
		}
		else if (lockX) {
			intersection.y = planeOrigin.y;
			intersection.z = planeOrigin.z;
		}
		else if (lockY) {
			intersection.x = planeOrigin.x;
			intersection.z = planeOrigin.z;
		}
		else if (lockZ) {
			intersection.x = planeOrigin.x;
			intersection.y = planeOrigin.y;
		}

		// Apply grid snap.
		auto forceSnapping = isKeyDown(VK_LCONTROL);
		if (isGridSnapping() || forceSnapping) {
			auto increment = gSnapGrid::get();
			if (increment != 0.0f) {
				// "Unlocked" movement defaults to XY axis.
				auto lockXY = !isAxisLocked;
				// Legacy grid snap also locks the Z axis by default.
				auto legacyLockZ = !isAxisLocked && settings.render_window.use_legacy_grid_snap;
				// Offset so the last reference will be on the grid.
				auto p = intersection + (lastTarget->reference->position - planeOrigin);
				if (lockX || lockXY) {
					intersection.x -= p.x - (std::roundf(p.x / increment) * increment);
				}
				if (lockY || lockXY) {
					intersection.y -= p.y - (std::roundf(p.y / increment) * increment);
				}
				if (lockZ || legacyLockZ) {
					intersection.z -= p.z - (std::roundf(p.z / increment) * increment);
				}
			}
		}

		// We probably don't want to be sending things off into far distant cells.
		// Can happen unintentionally if camera direction is parallel with movement axis.
		if (intersection.distance(&planeOrigin) > 8192.0f) {
			return 0;
		}

		// Update positions.
		for (auto target = selectionData->firstTarget; target; target = target->next) {
			auto reference = target->reference;
			reference->position = intersection + (reference->position - planeOrigin);
			reference->unknown_0x10 = reference->position;
			reference->sceneNode->localTranslate = reference->position;
			reference->sceneNode->update(0.0f, true, true);

			DataHandler::get()->updateLightingForReference(reference);

			reference->setAsEdited();
		}

		selectionData->recalculateCenter();
		widgets->setPosition(lastTarget->reference->position);

		return 0;
	}

	//
	// Patch: Make clicking things near skinned objects not painful.
	//

	Reference* __cdecl Patch_FixPickAgainstSkinnedObjects(SceneGraphController* sgController, RenderController* renderController, int screenX, int screenY) {
		NI::Vector3 origin;
		NI::Vector3 direction;
		if (!renderController->camera->windowPointToRay(screenX, screenY, origin, direction)) {
			return nullptr;
		}

		if (!sgController->objectPick->pickObjectsWithSkinDeforms(&origin, &direction)) {
			return nullptr;
		}

		Reference* closestRef = nullptr;
		auto closestDistance = std::numeric_limits<float>::max();
		
		for (auto& result : sgController->objectPick->results) {
			if (result == nullptr) {
				continue;
			}

			if (result->distance < closestDistance) {
				closestRef = result->object->getTes3Reference(true);
				closestDistance = result->distance;
			}
		}

		return closestRef;
	}

	//
	// Patch: Allow custom FOV.
	//

	bool __fastcall Patch_AllowCustomFOV(NetImmerseInstance* self, DWORD _EDX_, NI::Camera* camera, int width, int height, float renderDistance) {
		auto& frustum = camera->viewFrustum;
		frustum.setFOV(settings.render_window.fov, float(height) / float(width));
		frustum.near = std::clamp(renderDistance / 10000.0f, 0.1f, 12.0f);
		frustum.far = renderDistance;

		camera->port = { 0.0f, 1.0f, 1.0f, 0.0f };

		camera->scene = self->node;
		camera->update();

		return true;
	}

	//
	// Patch: Allow antialiasing in the render window
	//

	byte __fastcall Patch_AllowCustomMultisamples(void* self, DWORD _edx_, unsigned int a2, unsigned int a3, char a4, int multiSamples, int a6, unsigned int a7, unsigned int a8, int a9, unsigned int a10, int a11) {
		multiSamples = settings.render_window.multisamples;

		auto fn = reinterpret_cast<byte(__thiscall*)(void*, unsigned int, unsigned int, char, int, int, unsigned int, unsigned int, int, unsigned int, int)>(0x58ECC0);
		return fn(self, a2, a3, a4, multiSamples, a6, a7, a8, a9, a10, a11);
	}

	//
	// Fix camera positioning around objects with too high of a bound radius.
	//

	NI::Node* __fastcall Patch_LimitCameraOrbitRadius(Reference* reference) {
		// Force an update call. This fixes world bounds...
		auto node = reference->sceneNode;
		node->update();

		// Overwritten code.
		return node;
	}

	//
	// Patch: Allow custom markers to be toggled.
	//

	void SetCullOnTriEditorMarkers(NI::AVObject* object, bool cull) {
		if (object->isInstanceOfType(NI::RTTIStaticPtr::NiNode)) {
			for (auto& child : static_cast<NI::Node*>(object)->children) {
				if (child) {
					SetCullOnTriEditorMarkers(child, cull);
				}
			}
		}
		else if (object->name) {
			if (_strnicmp(object->name, "Tri EditorMarker", 16) == 0) {
				object->setAppCulled(cull);
			}
		}
	}

	void __fastcall PatchEditorMarkers(Reference* reference, bool cull) {
		if (reference->sceneNode == nullptr) {
			return;
		}

		if (reference->isMarker()) {
			reference->sceneNode->setAppCulled(cull);
		}
		else if (reference->sceneNode->hasStringDataWithValue("MRK")) {
			SetCullOnTriEditorMarkers(reference->sceneNode, cull);

			auto editorMarker = reference->sceneNode->getObjectByName("EditorMarker");
			if (editorMarker) {
				editorMarker->setAppCulled(cull);
			}
		}
	}

	__declspec(naked) void PatchEditorMarkers_Setup() {
		__asm {
			mov edx, [esp + 0x18 + 0x4]
		}
	}
	constexpr auto PatchEditorMarkers_Setup_Size = 0x4u;

	//
	// Patch: When hiding objects (Shift+C) in terrain editing mode (H), do not hide the terrain editing circle.
	//

	void __fastcall PatchAddBlankTexturingProperty(NI::LinkedList<NI::Property*>* properties, DWORD _EDX_, NI::Property* prop) {
		const auto avObject = memory::getMainStructureFromOffset<NI::AVObject>(properties, offsetof(NI::AVObject, propertyNode));
		
		// Add existing property.
		avObject->attachProperty(prop);

		// Add an empty texturing property.
		auto texturingProp = new NI::TexturingProperty();
		avObject->attachProperty(texturingProp);
	}

	void __fastcall PatchFixMaterialPropertyColors(NI::LinkedList<NI::Property*>* properties, DWORD _EDX_, NI::MaterialProperty* prop) {
		const auto color = NI::Color(1.0f, 0.0f, 0.0f);
		prop->ambient = color;
		prop->diffuse = color;
		prop->specular = color;
		prop->emissive = color;

		const auto avObject = memory::getMainStructureFromOffset<NI::AVObject>(properties, offsetof(NI::AVObject, propertyNode));
		avObject->attachProperty(prop);
	}

	//
	// Patch: Extend Render Window message handling.
	//

	static std::optional<LRESULT> PatchDialogProc_OverrideResult = {};

	constexpr auto landscapeEditWindowId = 203;

	bool PickLandscapeTexture(HWND hWnd) {
		auto editorWindow = memory::MemAccess<HWND>::Get(0x6CE95C);
		if (!editorWindow) {
			return false;
		}

		// Make sure that we're not in color mode.
		if (IsDlgButtonChecked(editorWindow, 1008)) {
			return false;
		}

		auto& rendererPicker = gRenderWindowPick::get();
		auto rendererController = RenderController::get();
		auto sceneGraphController = SceneGraphController::get();

		// Cache picker settings we care about.
		const auto previousRoot = rendererPicker.root;
		const auto previousPickType = rendererPicker.pickType;

		// Make changes to the pick that we need to.
		rendererPicker.root = sceneGraphController->landscapeRoot;
		rendererPicker.pickType = NI::PickType::FIND_ALL;

		NI::Vector3 origin;
		NI::Vector3 direction;
		if (rendererController->camera->windowPointToRay(lastRenderWindowPosX, lastRenderWindowPosY, origin, direction)) {
			direction.normalize();

			if (rendererPicker.pickObjects(&origin, &direction)) {
				auto firstResult = rendererPicker.getFirstUnskinnedResult();
				if (firstResult->object) {
					auto texturingProperty = firstResult->object->getTexturingProperty();
					if (texturingProperty) {
						auto baseMap = texturingProperty->getBaseMap();
						if (baseMap && baseMap->texture) {
							LVITEMA queryData = {};
							queryData.mask = LVIF_PARAM;
							auto textureList = GetDlgItem(editorWindow, 1492);
							int textureCount = ListView_GetItemCount(textureList);
							for (int row = 0; row < textureCount; ++row) {
								queryData.iItem = row;
								if (ListView_GetItem(textureList, &queryData)) {
									auto landTexture = reinterpret_cast<LandTexture*>(queryData.lParam);
									if (landTexture) {
										if (landTexture->texture == baseMap->texture) {
											ListView_SetItemState(textureList, row, LVIS_SELECTED, LVIS_SELECTED);
											ListView_EnsureVisible(textureList, row, TRUE);
											break;
										}
									}
								}
							}
						}
					}
				}
			}
		}

		// Restore pick settings.
		rendererPicker.clearResults();
		rendererPicker.root = previousRoot;
		rendererPicker.pickType = previousPickType;

		return true;
	}

	void hideSelectedReferences() {
		auto selectionData = SelectionData::get();
		
		for (auto target = selectionData->firstTarget; target; target = target->next) {
			auto node = target->reference->sceneNode;
			if (node) {
				node->addExtraData(new NI::StringExtraData("xHID"));
				node->setAppCulled(true);
				node->update();
			}
		}

		selectionData->clear();
	}

	void unhideNode(NI::Node* node) {
		auto hideFlag = node->getStringDataWithValue("xHID");
		if (hideFlag) {
			node->removeExtraData(hideFlag);
			node->setAppCulled(false);
			node->update();
		}

		for (auto& child : node->children) {
			if (child && child->isInstanceOfType(NI::RTTIStaticPtr::NiNode)) {
				unhideNode(static_cast<NI::Node*>(child.get()));
			}
		}
	}

	void unhideAllReferences() {
		unhideNode(SceneGraphController::get()->objectRoot);
	}

	void saveRenderStateToQuickStart() {
		auto renderController = RenderController::get();
		auto dataHandler = DataHandler::get();
		auto recordHandler = dataHandler->recordHandler;
		auto& quickstart = settings.quickstart;

		// Restore quickstart state to be empty.
		settings.quickstart = Settings_t::QuickstartSettings();

		// Only store some data if we have a current cell.
		auto currentCell = gCurrentCell::get();
		if (currentCell) {
			quickstart.load_cell = true;

			// Store camera position.
			quickstart.position[0] = renderController->camera->worldTransform.translation.x;
			quickstart.position[1] = renderController->camera->worldTransform.translation.y;
			quickstart.position[2] = renderController->camera->worldTransform.translation.z;

			// Store camera orientation as euler angle.
			NI::Vector3 orientationVector;
			renderController->node->getLocalRotationMatrix()->toEulerXYZ(&orientationVector);
			quickstart.orientation[0] = orientationVector.x;
			quickstart.orientation[1] = orientationVector.y;
			quickstart.orientation[2] = orientationVector.z;

			// If we're in an interior, store the cell ID.
			if (currentCell == dataHandler->currentInteriorCell) {
				quickstart.cell = gCurrentCell::get()->getObjectID();
			}
			else {
				quickstart.cell.clear();
			}
		}
		else {
			quickstart.load_cell = false;
			quickstart.position = { 0.0f, 0.0f, 0.0f };
			quickstart.orientation = { 0.0f, 0.0f, 0.0f };
			quickstart.cell.clear();
		}

		// Store the active game file list.
		quickstart.data_files.clear();
		for (auto i = 0; i < recordHandler->activeModCount; ++i) {
			quickstart.data_files.push_back(recordHandler->activeGameFiles[i]->fileName);
		}
		
		// Also store the current file set as active.
		if (recordHandler->activeFile) {
			quickstart.active_file = recordHandler->activeFile->fileName;
		}

		quickstart.enabled = true;

		settings.save();
	}

	void clearRenderStateFromQuickStart() {
		auto renderController = RenderController::get();
		auto& quickstart = settings.quickstart;

		quickstart.enabled = false;
		quickstart.data_files.clear();
		quickstart.active_file.clear();
		quickstart.cell.clear();
		quickstart.position = { 0.0f, 0.0f, 0.0f };
		quickstart.orientation = { 0.0f, 0.0f, 0.0f };

		settings.save();
	}

	void showContextAwareActionMenu(HWND hWndRenderWindow) {
		auto menu = CreatePopupMenu();
		if (menu == NULL) {
			return;
		}

		auto recordHandler = DataHandler::get()->recordHandler;
		auto selectionData = SelectionData::get();
		const bool hasReferencesSelected = selectionData->numberOfTargets > 0;

		enum ContextMenuId {
			RESERVED_ERROR,
			RESERVED_NO_CALLBACK,
			HIDE_SELECTION,
			RESTORE_HIDDEN_REFERENCES,
			SET_SNAPPING_AXIS_POSITIVE_X,
			SET_SNAPPING_AXIS_NEGATIVE_X,
			SET_SNAPPING_AXIS_POSITIVE_Y,
			SET_SNAPPING_AXIS_NEGATIVE_Y,
			SET_SNAPPING_AXIS_POSITIVE_Z,
			SET_SNAPPING_AXIS_NEGATIVE_Z,
			USE_GROUP_SCALING,
			USE_LEGACY_OBJECT_MOVEMENT,
			USE_WORLD_AXIS_ROTATION,
			SAVE_STATE_TO_QUICKSTART,
			CLEAR_STATE_FROM_QUICKSTART,
		};

		/*
		* Reserved hotkeys:
		*	H: Hide Selection
		*	R: Restore Hidden References
		*	S: Set Snapping Axis
		*	W: Toggle world axis rotation
		*   M: Toggle legacy object movement
		*/

		MENUITEMINFO menuItem = {};
		menuItem.cbSize = sizeof(MENUITEMINFO);
		unsigned int index = 0;

		MENUITEMINFO subMenuSnappingAxis = {};
		subMenuSnappingAxis.cbSize = sizeof(MENUITEMINFO);
		subMenuSnappingAxis.hSubMenu = CreateMenu();

		{
			unsigned int subIndex = 0;

			menuItem.wID = SET_SNAPPING_AXIS_POSITIVE_X;
			menuItem.fMask = MIIM_FTYPE | MIIM_CHECKMARKS | MIIM_STRING | MIIM_ID;
			menuItem.fType = MFT_STRING | MFT_RADIOCHECK;
			menuItem.fState = (snappingAxis == SnappingAxis::POSITIVE_X) ? MFS_CHECKED : MFS_UNCHECKED;
			menuItem.dwTypeData = (LPSTR)"+&X";
			menuItem.hbmpChecked = NULL;
			menuItem.hbmpUnchecked = NULL;
			InsertMenuItemA(subMenuSnappingAxis.hSubMenu, subIndex++, TRUE, &menuItem);

			menuItem.wID = SET_SNAPPING_AXIS_NEGATIVE_X;
			menuItem.fMask = MIIM_FTYPE | MIIM_CHECKMARKS | MIIM_STRING | MIIM_ID;
			menuItem.fType = MFT_STRING | MFT_RADIOCHECK;
			menuItem.fState = (snappingAxis == SnappingAxis::NEGATIVE_X) ? MFS_CHECKED : MFS_UNCHECKED;
			menuItem.dwTypeData = (LPSTR)"-X";
			InsertMenuItemA(subMenuSnappingAxis.hSubMenu, subIndex++, TRUE, &menuItem);

			menuItem.wID = SET_SNAPPING_AXIS_POSITIVE_Y;
			menuItem.fMask = MIIM_FTYPE | MIIM_CHECKMARKS | MIIM_STRING | MIIM_ID;
			menuItem.fType = MFT_STRING | MFT_RADIOCHECK;
			menuItem.fState = (snappingAxis == SnappingAxis::POSITIVE_Y) ? MFS_CHECKED : MFS_UNCHECKED;
			menuItem.dwTypeData = (LPSTR)"+&Y";
			InsertMenuItemA(subMenuSnappingAxis.hSubMenu, subIndex++, TRUE, &menuItem);

			menuItem.wID = SET_SNAPPING_AXIS_NEGATIVE_Y;
			menuItem.fMask = MIIM_FTYPE | MIIM_CHECKMARKS | MIIM_STRING | MIIM_ID;
			menuItem.fType = MFT_STRING | MFT_RADIOCHECK;
			menuItem.fState = (snappingAxis == SnappingAxis::NEGATIVE_Y) ? MFS_CHECKED : MFS_UNCHECKED;
			menuItem.dwTypeData = (LPSTR)"-Y";
			InsertMenuItemA(subMenuSnappingAxis.hSubMenu, subIndex++, TRUE, &menuItem);

			menuItem.wID = SET_SNAPPING_AXIS_POSITIVE_Z;
			menuItem.fMask = MIIM_FTYPE | MIIM_CHECKMARKS | MIIM_STRING | MIIM_ID;
			menuItem.fType = MFT_STRING | MFT_RADIOCHECK;
			menuItem.fState = (snappingAxis == SnappingAxis::POSITIVE_Z) ? MFS_CHECKED : MFS_UNCHECKED;
			menuItem.dwTypeData = (LPSTR)"+&Z";
			InsertMenuItemA(subMenuSnappingAxis.hSubMenu, subIndex++, TRUE, &menuItem);

			menuItem.wID = SET_SNAPPING_AXIS_NEGATIVE_Z;
			menuItem.fMask = MIIM_FTYPE | MIIM_CHECKMARKS | MIIM_STRING | MIIM_ID;
			menuItem.fType = MFT_STRING | MFT_RADIOCHECK;
			menuItem.fState = (snappingAxis == SnappingAxis::NEGATIVE_Z) ? MFS_CHECKED : MFS_UNCHECKED;
			menuItem.dwTypeData = (LPSTR)"-Z";
			InsertMenuItemA(subMenuSnappingAxis.hSubMenu, subIndex++, TRUE, &menuItem);

			CheckMenuRadioItem(subMenuSnappingAxis.hSubMenu, 0, 5, (UINT)snappingAxis, MF_BYPOSITION);
		}

		menuItem.wID = RESERVED_NO_CALLBACK;
		menuItem.fMask = MIIM_SUBMENU | MIIM_TYPE;
		menuItem.fType = MFT_STRING;
		menuItem.fState = hasReferencesSelected ? MFS_ENABLED : MFS_DISABLED;
		menuItem.hSubMenu = subMenuSnappingAxis.hSubMenu;
		menuItem.dwTypeData = (LPSTR)"Set &Snapping Axis";
		InsertMenuItemA(menu, index++, TRUE, &menuItem);

		menuItem.wID = USE_GROUP_SCALING;
		menuItem.fMask = MIIM_FTYPE | MIIM_CHECKMARKS | MIIM_STRING | MIIM_ID;
		menuItem.fType = MFT_STRING;
		menuItem.fState = (settings.render_window.use_group_scaling) ? MFS_CHECKED : MFS_UNCHECKED;
		menuItem.dwTypeData = (LPSTR)"Use Group Scaling";
		InsertMenuItemA(menu, index++, TRUE, &menuItem);
		CheckMenuItem(menu, USE_GROUP_SCALING, (settings.render_window.use_group_scaling) ? MFS_CHECKED : MFS_UNCHECKED);

		menuItem.wID = USE_LEGACY_OBJECT_MOVEMENT;
		menuItem.fMask = MIIM_FTYPE | MIIM_CHECKMARKS | MIIM_STRING | MIIM_ID;
		menuItem.fType = MFT_STRING;
		menuItem.fState = (settings.render_window.use_legacy_object_movement) ? MFS_CHECKED : MFS_UNCHECKED;
		menuItem.dwTypeData = (LPSTR)"Use Legacy Object &Movement";
		InsertMenuItemA(menu, index++, TRUE, &menuItem);
		CheckMenuItem(menu, USE_LEGACY_OBJECT_MOVEMENT, (settings.render_window.use_legacy_object_movement) ? MFS_CHECKED : MFS_UNCHECKED);

		menuItem.wID = USE_WORLD_AXIS_ROTATION;
		menuItem.fMask = MIIM_FTYPE | MIIM_CHECKMARKS | MIIM_STRING | MIIM_ID;
		menuItem.fType = MFT_STRING;
		menuItem.fState = (settings.render_window.use_world_axis_rotations_by_default) ? MFS_CHECKED : MFS_UNCHECKED;
		menuItem.dwTypeData = (LPSTR)"Use &World Rotation Axis";
		InsertMenuItemA(menu, index++, TRUE, &menuItem);
		CheckMenuItem(menu, USE_WORLD_AXIS_ROTATION, (settings.render_window.use_world_axis_rotations_by_default) ? MFS_CHECKED : MFS_UNCHECKED);

		menuItem.wID = RESERVED_NO_CALLBACK;
		menuItem.fMask = MIIM_FTYPE | MIIM_ID;
		menuItem.fType = MFT_SEPARATOR;
		InsertMenuItemA(menu, index++, TRUE, &menuItem);

		menuItem.wID = HIDE_SELECTION;
		menuItem.fMask = MIIM_FTYPE | MIIM_STRING | MIIM_ID | MIIM_STATE;
		menuItem.fType = MFT_STRING;
		menuItem.fState = hasReferencesSelected ? MFS_ENABLED : MFS_DISABLED;
		menuItem.dwTypeData = (LPSTR)"&Hide Selection";
		InsertMenuItemA(menu, index++, TRUE, &menuItem);

		menuItem.wID = RESTORE_HIDDEN_REFERENCES;
		menuItem.fMask = MIIM_FTYPE | MIIM_STRING | MIIM_ID | MIIM_STATE;
		menuItem.fType = MFT_STRING;
		menuItem.fState = MFS_ENABLED;
		menuItem.dwTypeData = (LPSTR)"&Restore Hidden References";
		InsertMenuItemA(menu, index++, TRUE, &menuItem);

		menuItem.wID = RESERVED_NO_CALLBACK;
		menuItem.fMask = MIIM_FTYPE | MIIM_ID;
		menuItem.fType = MFT_SEPARATOR;
		InsertMenuItemA(menu, index++, TRUE, &menuItem);

		menuItem.wID = SAVE_STATE_TO_QUICKSTART;
		menuItem.fMask = MIIM_FTYPE | MIIM_STRING | MIIM_ID | MIIM_STATE;
		menuItem.fType = MFT_STRING;
		menuItem.fState = (recordHandler->activeModCount > 0) ? MFS_ENABLED : MFS_DISABLED;
		menuItem.dwTypeData = (LPSTR)"Save State to QuickStart";
		InsertMenuItemA(menu, index++, TRUE, &menuItem);

		menuItem.wID = CLEAR_STATE_FROM_QUICKSTART;
		menuItem.fMask = MIIM_FTYPE | MIIM_STRING | MIIM_ID | MIIM_STATE;
		menuItem.fType = MFT_STRING;
		menuItem.fState = settings.quickstart.enabled ? MFS_ENABLED : MFS_DISABLED;
		menuItem.dwTypeData = (LPSTR)"Clear QuickStart";
		InsertMenuItemA(menu, index++, TRUE, &menuItem);

		POINT p;
		GetCursorPos(&p);

		// Due to where this message is, prevent annoying beep sound when the tracking of the menu consumes shit.
		MSG msg;
		PeekMessage(&msg, NULL, WM_CHAR, WM_CHAR, PM_REMOVE);

		auto result = TrackPopupMenuEx(menu, TPM_LEFTALIGN | TPM_TOPALIGN | TPM_RETURNCMD | TPM_LEFTBUTTON | TPM_NOANIMATION | TPM_VERTICAL, p.x, p.y, hWndRenderWindow, NULL);
		
		switch (result) {
		case RESERVED_ERROR:
			break;
		case HIDE_SELECTION:
			hideSelectedReferences();
			break;
		case RESTORE_HIDDEN_REFERENCES:
			unhideAllReferences();
			break;
		case SET_SNAPPING_AXIS_POSITIVE_X:
			snappingAxis = SnappingAxis::POSITIVE_X;
			break;
		case SET_SNAPPING_AXIS_NEGATIVE_X:
			snappingAxis = SnappingAxis::NEGATIVE_X;
			break;
		case SET_SNAPPING_AXIS_POSITIVE_Y:
			snappingAxis = SnappingAxis::POSITIVE_Y;
			break;
		case SET_SNAPPING_AXIS_NEGATIVE_Y:
			snappingAxis = SnappingAxis::NEGATIVE_Y;
			break;
		case SET_SNAPPING_AXIS_POSITIVE_Z:
			snappingAxis = SnappingAxis::POSITIVE_Z;
			break;
		case SET_SNAPPING_AXIS_NEGATIVE_Z:
			snappingAxis = SnappingAxis::NEGATIVE_Z;
			break;
		case USE_GROUP_SCALING:
			settings.render_window.use_group_scaling = !settings.render_window.use_group_scaling;
			settings.save();
			break;
		case USE_LEGACY_OBJECT_MOVEMENT:
			settings.render_window.use_legacy_object_movement = !settings.render_window.use_legacy_object_movement;
			settings.save();
			break;
		case USE_WORLD_AXIS_ROTATION:
			settings.render_window.use_world_axis_rotations_by_default = !settings.render_window.use_world_axis_rotations_by_default;
			settings.save();
			break;
		case SAVE_STATE_TO_QUICKSTART:
			saveRenderStateToQuickStart();
			break;
		case CLEAR_STATE_FROM_QUICKSTART:
			clearRenderStateFromQuickStart();
			break;
		default:
			log::stream << "Unknown render window context menu ID " << result << " used!" << std::endl;
		}

		// Cleanup our menus.
		DestroyMenu(menu);
		DestroyMenu(subMenuSnappingAxis.hSubMenu);

		// We also stole paint stuff, so repaint.
		SendMessage(hWndRenderWindow, WM_PAINT, 0, 0);
		PatchDialogProc_OverrideResult = TRUE;
	}

	void PatchDialogProc_BeforeLMouseButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		movementContext.reset();
	}

	void PatchDialogProc_BeforeRMouseButtonDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		constexpr auto comboPickLandscapeTexture = MK_CONTROL | MK_RBUTTON;
		if ((wParam & comboPickLandscapeTexture) == comboPickLandscapeTexture) {
			if (PickLandscapeTexture(hWnd)) {
				PatchDialogProc_OverrideResult = TRUE;
			}
		}
	}

	void PatchDialogProc_BeforeSetCameraPosition(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		if (RenderController::get()->node == nullptr) {
			PatchDialogProc_OverrideResult = FALSE;
		}
	}

	void PatchDialogProc_AfterKeyDown(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (wParam) {
		case 'Q':
			showContextAwareActionMenu(hWnd);
			break;
		}
	}

	void PatchDialogProc_AfterKeyUp_XYZ(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		// Hide widgets if they are no longer needed.
		auto widgets = SceneGraphController::get()->widgets;
		if (!isHoldingAxisKey() && widgets->isShown()) {
			widgets->hide();
			movementContext.reset();
			gRenderNextFrame::set(true);
		}
	}

	void PatchDialogProc_AfterKeyUp(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		switch (wParam) {
		case 'X':
		case 'Y':
		case 'Z':
			PatchDialogProc_AfterKeyUp_XYZ(hWnd, msg, wParam, lParam);
			break;
		}
	}

	void PatchDialogProc_AfterInitDialog(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		// Initialize widget controller.
		auto sgController = SceneGraphController::get();
		sgController->widgets = new WidgetsController();
		sgController->sceneRoot->attachChild(sgController->widgets->root);
	}

	namespace CustomWindowMessage {
		constexpr UINT SetCameraPosition = 0x40Eu;
	}

	LRESULT CALLBACK PatchDialogProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		PatchDialogProc_OverrideResult.reset();

		switch (msg) {
		case WM_MOUSEMOVE:
			lastRenderWindowPosX = LOWORD(lParam);
			lastRenderWindowPosY = HIWORD(lParam);
			break;
		case WM_LBUTTONDOWN:
			PatchDialogProc_BeforeLMouseButtonDown(hWnd, msg, wParam, lParam);
			break;
		case WM_RBUTTONDOWN:
			PatchDialogProc_BeforeRMouseButtonDown(hWnd, msg, wParam, lParam);
			break;
		case CustomWindowMessage::SetCameraPosition:
			PatchDialogProc_BeforeSetCameraPosition(hWnd, msg, wParam, lParam);
			break;
		}

		if (PatchDialogProc_OverrideResult) {
			return PatchDialogProc_OverrideResult.value();
		}

		// Call original function.
		const auto CS_RenderWindowDialogProc = reinterpret_cast<WNDPROC>(0x45A3F0);
		auto vanillaResult = CS_RenderWindowDialogProc(hWnd, msg, wParam, lParam);

		switch (msg) {
		case WM_KEYDOWN:
			PatchDialogProc_AfterKeyDown(hWnd, msg, wParam, lParam);
			break;
		case WM_KEYUP:
			PatchDialogProc_AfterKeyUp(hWnd, msg, wParam, lParam);
			break;
		case WM_INITDIALOG:
			PatchDialogProc_AfterInitDialog(hWnd, msg, wParam, lParam);
			break;
		}

		return PatchDialogProc_OverrideResult.value_or(vanillaResult);
	}

	//
	//
	//

	void installPatches() {
		using memory::genCallEnforced;
		using memory::genCallUnprotected;
		using memory::genJumpEnforced;
		using memory::genPushEnforced;
		using memory::writeDoubleWordEnforced;
		using memory::writePatchCodeUnprotected;

		// Patch: Extend SceneGraphController structure.
		static_assert(sizeof(SceneGraphController) < INT8_MAX);
		genPushEnforced(0x4473FD, (BYTE)sizeof(SceneGraphController));
		genJumpEnforced(0x403855, 0x449930, reinterpret_cast<DWORD>(SceneGraphController::initialize));

		// Patch: Use world rotation values.
		genJumpEnforced(0x403D41, 0x4652D0, reinterpret_cast<DWORD>(Patch_ReplaceRotationLogic));

		// Patch: Improve multi-reference scaling.
		genCallEnforced(0x45EE3A, 0x404949, reinterpret_cast<DWORD>(Patch_ReplaceScalingLogic));

		// Patch: Improve drag-move logic.
		genJumpEnforced(0x401F4B, 0x464B70, reinterpret_cast<DWORD>(Patch_ReplaceDragMovementLogic));

		// Patch: Improve drop-to-surface logic by ignoring particles and skinned geometry.
		genCallEnforced(0x46687F, 0x403AD5, reinterpret_cast<DWORD>(Patch_FixDropToSurface_GetLowestZ));
		genJumpEnforced(0x402CE8, 0x4665F0, reinterpret_cast<DWORD>(Patch_FixDropToSurface_GetLowVertices));
		genCallEnforced(0x46697E, 0x5B35B0, reinterpret_cast<DWORD>(Patch_FixDropToSurface_PickObjects));

		// Patch: Make clicking things near skinned objects not painful.
		genJumpEnforced(0x404980, 0x463F30, reinterpret_cast<DWORD>(Patch_FixPickAgainstSkinnedObjects));

		// Patch: Allow custom FOV.
		genJumpEnforced(0x4028FB, 0x40BE60, reinterpret_cast<DWORD>(Patch_AllowCustomFOV));

		// Patch: Allow custom multisamples.
		genCallEnforced(0x58DCFD, 0x58ECC0, reinterpret_cast<DWORD>(Patch_AllowCustomMultisamples));
		genCallEnforced(0x58EC30, 0x58ECC0, reinterpret_cast<DWORD>(Patch_AllowCustomMultisamples));

		// Patch: Fix camera positioning around objects with too high of a bound radius.
		genCallEnforced(0x45FCB8, 0x4015BE, reinterpret_cast<DWORD>(Patch_LimitCameraOrbitRadius));

		// Patch: Custom marker toggling code.
		writePatchCodeUnprotected(0x49E8CE, (BYTE*)PatchEditorMarkers_Setup, PatchEditorMarkers_Setup_Size);
		genCallUnprotected(0x49E8CE + PatchEditorMarkers_Setup_Size, reinterpret_cast<DWORD>(PatchEditorMarkers), 0x49E8E9 - 0x49E8CE - PatchEditorMarkers_Setup_Size);
		writePatchCodeUnprotected(0x49E932, (BYTE*)PatchEditorMarkers_Setup, PatchEditorMarkers_Setup_Size);
		genCallUnprotected(0x49E932 + PatchEditorMarkers_Setup_Size, reinterpret_cast<DWORD>(PatchEditorMarkers), 0x49E94D - 0x49E932 - PatchEditorMarkers_Setup_Size);

		// Patch: When hiding objects (Shift+C) in terrain editing mode (H), do not hide the terrain editing circle.
		writeDoubleWordEnforced(0x45F166 + 0x2, 0x12C, 0x134);
		writeDoubleWordEnforced(0x45F39C + 0x2, 0x12C, 0x134);
		writeDoubleWordEnforced(0x45F719 + 0x2, 0x12C, 0x134);
		genCallEnforced(0x45F4ED, 0x4015A0, reinterpret_cast<DWORD>(PatchFixMaterialPropertyColors));
		genCallEnforced(0x45F626, 0x4015A0, reinterpret_cast<DWORD>(PatchAddBlankTexturingProperty));

		// Patch: Extend Render Window message handling.
		genJumpEnforced(0x4020EF, 0x45A3F0, reinterpret_cast<DWORD>(PatchDialogProc));
	}
}
