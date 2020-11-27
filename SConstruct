from SConsStandard import EnvMod

EnsurePythonVersion(3, 5)

env = EnvMod.getEnvironment()
env.withConan()
env.withCompilationDB()

database = None

env.SConscript("src/SConscript", variant_dir = "src", duplicate = 0)
