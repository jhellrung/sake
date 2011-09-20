################################################################################
## SConstruct
##
## Copyright 2011, Jeffrey Hellrung.
## Distributed under the Boost Software License, Version 1.0.  (See accompanying
## file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
################################################################################

import fnmatch
import os

vars = Variables('SConstruct.variables', ARGUMENTS)
vars.AddVariables(
    ('BUILD_CONFIG', "Build configuration (e.g., 'release' or 'debug')", 'debug'),
    ('MARCH', "-march", None),
    ('MCPU', "-mcpu", None),
    ('MTUNE', "-mtune", None),
    PathVariable('BOOST_ROOT', "Path to Boost root (e.g., 'boost_1_xy_z')", None)
)
env = Environment(variables=vars)
Help(vars.GenerateHelpText(env))

if env['BOOST_ROOT'] is None:
    env['BOOST_ROOT'] = os.environ.get('BOOST_ROOT')

#ARCHITECTURE
#CPPDEFINES
#CPPFLAGS
#CPPPATH
#CXXFLAGS
#ENV
#LIBPATH
#LIBS
#LINKFLAGS
#PLATFORM

env.Append(CPPPATH=['#'])
env.Append(CPPPATH=[env['BOOST_ROOT']])

if env['PLATFORM'] == 'win32':
else:
    print "Unknown PLATFORM:", env['PLATFORM']

variant_dir = os.path.join('build', env['PLATFORM'], env['BUILD_CONFIG'])
VariantDir(variant_dir, '.', duplicate=0)

lib_install_dir = os.path.join('libs', env['PLATFORM'], env['BUILD_CONFIG'])
prog_suffix = '_' + env['PLATFORM'] + '_' + env['BUILD_CONFIG']

def auto_Library(env, target, source):
    lib = env.Library(targe, source)
    env.Install(lib_install_dir, lib)

def auto_Program(env, target, source):
    prog = env.Program(target, source)
    env.InstallAs(os.path.join(Dir('.').srcnode().path, target, prog_suffix), prog)

def find_SConscripts(env, root):
    for dirpath, dirnames, filenames in os.walk(root):
        for f in fnmatch.filter(filenames, 'SConscript'):
            print os.path.join(variant_dir, dirpath, f)
            env.SConscript(os.path.join(variant_dir, dirpath, f))

Export('env', 'auto_Library', 'auto_Program')
find_SConscripts(env, 'lib')
find_SConscripts(env, 'unit_tests')
