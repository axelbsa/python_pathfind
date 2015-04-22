from distutils.core import setup, Extension


# To Build: python setup.py build_ext -i

module1 = Extension('pathfinding', sources = ['src/main.cpp', 'src/common.c'])

setup(name='PackageName',
        version='1.0',
        description='C-Astar_implementation',
        ext_modules=[module1])
