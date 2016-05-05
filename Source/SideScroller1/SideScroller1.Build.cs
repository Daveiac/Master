// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SideScroller1 : ModuleRules
{
	public SideScroller1(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Paper2D", "UMG"});

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "HeadMountedDisplay" });
    }
}
