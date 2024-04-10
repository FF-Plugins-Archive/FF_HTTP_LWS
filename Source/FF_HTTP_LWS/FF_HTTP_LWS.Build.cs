// Some copyright should be here...

using System;
using System.IO;
using UnrealBuildTool;
public class FF_HTTP_LWS : ModuleRules
{
	public FF_HTTP_LWS(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        bEnableUndefinedIdentifierWarnings = false;
        bEnableExceptions = true;
        bUseRTTI = true;

        PublicDefinitions.Add("HV_STATICLIB=1");

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "zlib",
                "OpenSSL",      // We don't need OpenSSL certificate functions
                "nghttp2",      // We need it for LibCurl
                "libcurl",
            }
            );
			
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
                "Projects",		// We need it for LibHv DLL delayed load.
                "HTTP",
                "HTTPServer",
                "LibWebSocket",
                "Pthreads",
            }
			);
	}
}