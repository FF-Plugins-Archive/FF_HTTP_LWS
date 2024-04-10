namespace UnrealBuildTool.Rules
{
    using System.IO;

    public class Pthreads : ModuleRules
    {
        public Pthreads(ReadOnlyTargetRules Target) : base(Target)
        {
    		Type = ModuleType.External;
            bEnableUndefinedIdentifierWarnings = false;
            bEnableExceptions = true;
            bUseRTTI = true;

            if (Target.Platform == UnrealTargetPlatform.Win64)
            {
                PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "Win64", "include"));
                
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Win64", "lib", "pthreadVC2.lib"));
                PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "Win64", "lib", "libpthreadGC2.a"));

                RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "Win64", "lib", "pthreadVC2.dll"));
                RuntimeDependencies.Add(Path.Combine(ModuleDirectory, "Win64", "lib", "pthreadGC2.dll"));
                PublicDelayLoadDLLs.Add("pthreadVC2.dll");
                PublicDelayLoadDLLs.Add("pthreadGC2.dll");
            }
        }
    }
}
