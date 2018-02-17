/*
 
 -----
 
*/

#pragma once

#include "ModuleManager.h"

class FJSONSVGPlugin : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

