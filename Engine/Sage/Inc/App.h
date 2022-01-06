#pragma once

#include "AppState.h"

namespace Sage
{
	struct AppConfig
	{
		std::wstring appName;
		int appWidth = 1280;
		int appLength = 720;
		uint32_t debugDrawLimit = 64000u;
	};

	class App
	{
	public:
		template <class StateType, class = std::void_t<std::is_base_of<AppState, StateType>>>
		void AddState(std::string name)
		{
			auto result = mAppStates.emplace(std::move(name), std::make_unique<StateType>());
			if (mCurrentState == nullptr)
			{
				LOG("App -- Starting state: %s", result.first->first.c_str());
				mCurrentState = result.first->second.get();
			}
		}
		void ChangeState(const std::string& name);

		void Run(AppConfig config);
		void Quit() { mRunning = false; }

	private:
		Core::Window mWindow;
		bool mRunning = false;

		std::map<std::string, std::unique_ptr<AppState>> mAppStates;
		AppState* mCurrentState = nullptr;
		AppState* mNextState = nullptr;
	};
}