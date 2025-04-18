from setuptools import setup
from cmake_setuptools import CMakeExtension, CMakeBuildExt

setup(
    ext_modules=[CMakeExtension("pysvzerod", install=True)],
    cmdclass={"build_ext": CMakeBuildExt},
)
