#pragma once

namespace Sage::Graphics
{
	class Camera
	{
	public:
		enum class ProjectionMode
		{
			Perspective,
			Orthographic
		};

		void SetMode(ProjectionMode mode) { mProjectionMode = mode; }

		void SetPosition(const Math::Vector3& position);
		void SetDirection(const Math::Vector3& direction);
		void LookAt(const Math::Vector3& target);

		//Perspective parameters
		void SetFov(float fov);
		void SetAspectRatio(float ratio);

		//Orthographic parameters
		void SetSize(float width, float height);

		void SetNearPlane(float nearPlane);
		void SetFarPlane(float farPlane);

		// 3 degrees of freedom for translations.
		void Walk(float distance);
		void Strafe(float distance);
		void Rise(float distance);

		// 2 degrees of freedom for rotaitons.
		void Yaw(float radian);   //Rotate the camera on its Y-axis/Upward-axis
		void Pitch(float radian); //Rotate the camera on its X-axis/Sideway-axis

		// 1 degree of freedom for field of view.
		void Zoom(float amount);

		const Math::Vector3& GetPosition() const { return mPosition; }
		const Math::Vector3& GetDirection() const { return mDirection; }

		Math::Matrix4 GetViewMatrix() const;
		Math::Matrix4 GetProjectionMatrix() const;

		Math::Matrix4 GetPerspectiveMatrix() const;
		Math::Matrix4 GetOrthographicMatrix() const;

	private:
		ProjectionMode mProjectionMode = ProjectionMode::Perspective;

		Math::Vector3 mPosition = Math::Vector3::Zero;
		Math::Vector3 mDirection = Math::Vector3::ZAxis;

		// 0 aspect ratio = use back buffer dimesion
		float mFov = 60.0f * Math::Constants::DegToRad;
		float mAspectRatio = 0.0f;

		// 0 width or height = use back buffer dimesion
		float mWidth = 0.0f;
		float mHeight = 0.0f;

		float mNearPlane = 0.1f;
		float mFarPlane = 10000.0f;
	};
}