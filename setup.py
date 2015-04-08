from distutils.core import setup, Extension


# To Build: python setup.py build
#           then copy so file to current dir
 
module1 = Extension('astar', sources = ['src/main.cpp'])
 
setup (name = 'PackageName',
        version = '1.0',
        description = 'axlASTAR',
        ext_modules = [module1])
