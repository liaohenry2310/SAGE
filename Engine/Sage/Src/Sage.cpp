#include "Precompiled.h"
#include "Sage.h"

Sage::App& Sage::MainApp()
{
	static App sApp;
	return sApp;
}