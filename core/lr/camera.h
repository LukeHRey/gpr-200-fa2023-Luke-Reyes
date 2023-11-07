#pragma once
#include "transformations.h"
namespace lr {
	struct Camera {
		ew::Vec3 position; //Camera body position
		ew::Vec3 target; //Position to look at
		float fov; //Vertical field of view in degrees
		float aspectRatio; //Screen width / Screen height
		float nearPlane; //Near plane distance (+Z)
		float farPlane; //Far plane distance (+Z)
		bool orthographic; //Perspective or orthographic?
		float orthoSize; //Height of orthographic frustum
		ew::Mat4 ViewMatrix() {
			return lr::LookAt(position, target, ew::Vec3(0.0, 1.0, 0.0));
		}; //World->View
		ew::Mat4 ProjectionMatrix() {
			if (orthographic) {
				return lr::Orthographic(orthoSize, aspectRatio, nearPlane, farPlane);
			}
			else {
				return lr::Perspective(fov, aspectRatio, nearPlane, farPlane);
			}
		}; //View->Clip
	};
}