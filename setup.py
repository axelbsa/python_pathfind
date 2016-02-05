from distutils.core import setup, Extension


# To Build: python setup.py build_ext -i

module1 = Extension('pathfinding',
        include_dirs = ['/usr/local/include'],
        libraries = ['pthread'],
        sources = ['src/main.cpp', 'src/astar.cpp'])

setup(name='PackageName',
        version='1.0',
        description='C-Astar_implementation',
        ext_modules=[module1])
