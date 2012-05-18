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
    EnumVariable('BUILDCONFIG', "Build configuration", 'debug', allowed_values=('debug', 'release', 'profile')),
    PathVariable('BOOST_ROOT', "Path to Boost root (e.g., 'boost_1_xy_z')", None),
    ('MARCH', "-march option for the C++ compiler", None)
)
env = Environment(variables=vars)
Help(vars.GenerateHelpText(env))

## Boost location.
if not env.has_key('BOOST_ROOT'):
    if not os.environ.has_key('BOOST_ROOT'):
        print("*** ERROR: Undefined BOOST_ROOT")
        Exit(2)
    env['BOOST_ROOT'] = os.environ.get('BOOST_ROOT')
print("BOOST_ROOT =", env['BOOST_ROOT'])

env.Append(CPPPATH=['#'])
env.Append(CPPPATH=[env['BOOST_ROOT']])

platform     = env['PLATFORM']
build_config = env['BUILDCONFIG']

## Set platform- and architecture-specific variables.
## This should be updated as needed.
plat_arch = platform
cpp_defines = []
cc_flags = ''
cxx_flags = ''
link_flags = ''
if build_config != 'debug':
    cpp_defines.extend(['NDEBUG'])
if platform == 'win32':
    print("*** ERROR: Unknown CXX:", env['CXX'])
    Exit(2)
    cpp_defines.extend(['NOMINMAX'])
    cxx_flags += ' /nologo /errorReport:none'
    cxx_flags += ' /EHsc /GR'
    cxx_flags += ' /Wall /WL'
    link_flags += ' /NOLOGO /ERRORREPORT:NONE'
    if build_config == 'debug':
        cc_flags   += ' /MDd'
        cxx_flags  += ' /Od /Zi /RTCc /RTC1 /MDd'
        link_flags += ' /DEBUG'
    else:
        cc_flags   += ' /MD'
        cxx_flags  += ' /O2 /GL /MD /Gy'
        link_flags += ' /LTCG'
elif platform == 'posix':
    if env['CXX'] == 'g++':
        cxxversion = tuple(map(int, env['CXXVERSION'].split('.')))
        if env.has_key('MARCH'):
            march = env['MARCH']
            plat_arch = march
            cxx_flags += ' -march=' + march
            # Intel
            if march in ['pentium-mmx', 'pentium2',
                         'pentium3', 'pentium3m',
                         'pentium-m', 'pentium4', 'pentium4m',
                         'prescott',
                         'nocona', 'core2']:
                while True:
                    cxx_flags += ' -mmmx'
                    if march in ['pentium-mmx', 'pentium2']: break
                    cxx_flags += ' -mfpmath=sse -msse'
                    if march in ['pentium3', 'pentium3m']: break
                    cxx_flags += ' -msse2'
                    if march in ['pentium-m', 'pentium4', 'pentium4m']: break
                    cxx_flags += ' -msse3'
                    if march in ['prescott']: break
                    cxx_flags += ' -mssse3'
                    if march in ['nocona', 'core2']: break
                    assert False
            # AMD
            elif march in ['k6',
                           'k6-2', 'k6-3', 'athlon', 'athlon-tbird',
                           'athlon-4', 'athlon-xp', 'athlon-mp',
                           'k8', 'opteron', 'athlon64', 'athlon-fx',
                           'amdfam10']:
                while True:
                    cxx_flags += ' -mmmx'
                    if march in ['k6']: break
                    cxx_flags += ' -m3dnow'
                    if march in ['k6-2', 'k6-3', 'athlon', 'athlon-tbird']: break
                    cxx_flags += ' -mfpmath=sse -msse'
                    if march in ['athlon-4', 'athlon-xp', 'athlon-mp']: break
                    cxx_flags += ' -msse2 -m64'
                    if march in ['k8', 'opteron', 'athlon64', 'athlon-fx']: break
                    cxx_flags += ' -msse3 -msse4a -mabm'
                    if march in ['amdfam10']: break
                    assert False
        cxx_flags += ' -Wall' \
                     ' -Woverloaded-virtual' \
                     ' -Wsign-promo' \
                     ' -Winit-self' \
                     ' -Wmissing-include-dirs' \
                     ' -Wunused-parameter' \
                     ' -Wstrict-aliasing=2' \
                     ' -Wextra' \
                     ' -Wshadow' \
                     ' -Wpointer-arith' \
                     ' -Wwrite-strings'
        if (4,3,6) <= cxxversion < (4,7,0):
            cxx_flags += ' -Wno-c++0x-compat'
        if (4,7,0) <= cxxversion:
            cxx_flags += ' -Wno-c++11-compat'
        if build_config == 'debug':
            cxx_flags += ' -g'
            link_flags += ' -g'
        else:
            # -fprefetch-loop-arrays ?
            cxx_flags += ' -O3 -funroll-loops'
        if build_config == 'profile':
            cxx_flags += ' -pg'
            link_flags += ' -pg'
    else:
        print("Unknown CXX:", env['CXX'])
        Exit(2)
else:
    print("Unknown PLATFORM:", platform)
    Exit(2)
env.Append(CPPDEFINES=cpp_defines)
env.Append(CCFLAGS=cc_flags)
env.Append(CXXFLAGS=cxx_flags)
env.Append(LINKFLAGS=link_flags)

env['PLATARCH'] = plat_arch

## Put all intermediate build files under #build.
variant_dir = os.path.join('build', plat_arch, build_config)
VariantDir(variant_dir, '.', duplicate=0)

## Install all libraries under #lib.
lib_install_dir = os.path.join('#lib', plat_arch, build_config)
env.Append(LIBPATH=[lib_install_dir])
env.Append(LIBS=['sake_core'])

## Necessary for sake::timer.
if platform == 'posix':
    env.Append(LIBS=['rt'])

prog_suffix = '_' + plat_arch + '_' + build_config

## Recursively search for cpp files starting from root.
def rglob(root, pattern):
    result = []
    ## Somewhat complicated by the use of variant builds...
    scons_root = Dir('#').abspath
    abs_root = os.path.join(scons_root, Dir(root).srcnode().path)
    for dirpath, dirnames, filenames in os.walk(abs_root):
        result.extend(os.path.join(os.path.relpath(dirpath, abs_root), f)
            for f in fnmatch.filter(filenames, pattern))
    return result

## env.Library + env.Install
def LibraryInstall(env, target, source):
    lib = env.Library(target, source)
    env.Install(lib_install_dir, lib)

## env.Program + env.InstallAs
def ProgramInstall(env, target, source):
    prog = env.Program(target, source)
    env.InstallAs(os.path.join('#' + Dir('.').srcnode().path, target + prog_suffix), prog)

## Recursively search for SConscript files starting from root.
def find_SConscripts(root):
    for dirpath, dirnames, filenames in os.walk(root):
        for f in fnmatch.filter(filenames, 'SConscript'):
            print(os.path.join(variant_dir, dirpath, f))
            SConscript(os.path.join(variant_dir, dirpath, f))

Export('env', 'rglob', 'LibraryInstall', 'ProgramInstall')
find_SConscripts('libs')
find_SConscripts('unit_test')
