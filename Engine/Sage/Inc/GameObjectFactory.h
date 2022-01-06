#pragma once

namespace Sage
{
	class GameObject;

	using MakeOverride = std::function<bool(const char*, const rapidjson::Value&, GameObject& gameObject)>;

	namespace GameObjectFactory
	{
		void SetMakeOverride(MakeOverride makeOverride);
		void Make(std::filesystem::path templateFile, GameObject& gameObject);
	};
}