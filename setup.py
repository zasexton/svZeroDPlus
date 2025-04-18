import os
import shutil
from setuptools import setup
from cmake_setuptools import CMakeExtension, CMakeBuildExt

class CustomCMakeBuild(CMakeBuildExt):
    def run(self):
        super().run()

        build_temp = os.path.abspath(self.build_temp)
        target_dir = os.path.abspath(os.path.dirname(self.get_ext_fullpath("pysvzerod")))
        print(f"Looking for pysvzerod in build tree: {build_temp}")
        print(f"Installing into: {target_dir}")

        found = False
        for root, dirs, files in os.walk(build_temp):
            for file in files:
                if file.startswith("pysvzerod") and file.endswith((".pyd", ".so")):
                    src = os.path.join(root, file)
                    dst = os.path.join(target_dir, file)
                    print(f"Copying built extension: {src} → {dst}")
                    os.makedirs(target_dir, exist_ok=True)
                    shutil.copyfile(src, dst)
                    found = True
                    break
            if found:
                break
        if not found:
            raise RuntimeError("Could not find built pysvzerod extension")

setup(
    ext_modules=[CMakeExtension("pysvzerod")],
    cmdclass={"build_ext": CustomCMakeBuild}
)
