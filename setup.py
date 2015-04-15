from distutils.core import setup, Extension


# To Build: python setup.py build
#           then copy so file to current dir
 
module1 = Extension('pathfinding', sources = ['src/main.cpp', 'src/common.c'])
 
setup (name = 'PackageName',
        version = '1.0',
        description = 'axlAstar',
        ext_modules = [module1])
