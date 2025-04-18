import os
import shutil
from setuptools import setup
from cmake_setuptools import CMakeExtension, CMakeBuildExt

class CustomCMakeBuild(CMakeBuildExt):
    def run(self):
        super().run()

        # This is where setuptools wants the compiled file to be
        target_dir = os.path.abspath(os.path.dirname(self.get_ext_fullpath("pysvzerod")))
        build_temp = os.path.abspath(self.build_temp)

        print(f"[INFO] Searching for built extension in: {build_temp}")
        print(f"[INFO] Will install extension into: {target_dir}")

        found = False
        for root, dirs, files in os.walk(build_temp):
            for f in files:
                if f.startswith("pysvzerod") and f.endswith((".so", ".pyd", ".dll", ".dylib")):
                    src = os.path.join(root, f)
                    dst = os.path.join(target_dir, f)
                    print(f"[INFO] Copying {src} → {dst}")
                    os.makedirs(target_dir, exist_ok=True)
                    shutil.copy2(src, dst)
                    found = True
                    break
            if found:
                break

        if not found:
            raise RuntimeError("Failed to find built extension module for pysvzerod")

setup(
    ext_modules=[CMakeExtension("pysvzerod")],
    cmdclass={"build_ext": CustomCMakeBuild}
)
