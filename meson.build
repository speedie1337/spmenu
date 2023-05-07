project(
  'spmenu',
  'c',
  version : '"1.1"',
  default_options : ['warning_level=3']
)

imlib2 = true
openssl = true
xresources = true
fribidi = true
libconfig = true
pango = true
pangoxft = true
xinerama = true
opt = '-O2'

cc = meson.get_compiler('c')

project_source_files = [
  'libs/libdrw/draw.c',
  'spmenu.c',
  'libs/sl/main.c',
]

project_dependencies = [
  dependency('x11'),
  dependency('xrender'),
  dependency('freetype2'),
  dependency('xft'),
  dependency('fontconfig'),
  cc.find_library('m', required : false),
]

build_args = [
  '-DVERSION=' + meson.project_version(),

  '-D_DEFAULT_SOURCE',
  '-D_BSD_SOURCE',
  '-D_XOPEN_SOURCE=700L',
  '-D_POSIX_C_SOURCE=200809L',

  '-std=c99',
  '-pedantic',
  '-Wall',
  '-Wno-deprecated-declarations',
  '-Wno-unused-parameter',
  '-Wno-sign-compare',
  opt,
]

if imlib2 and openssl
  project_dependencies += [ dependency('imlib2') ]
  project_dependencies += [ dependency('openssl') ]
  build_args += [ '-DIMAGE' ]
endif

if fribidi
  project_dependencies += [ dependency('fribidi') ]
  build_args += [ '-DRTL' ]
endif

if xinerama
  project_dependencies += [ dependency('xinerama') ]
  build_args += [ '-DXINERAMA' ]
endif

if pango and pangoxft
  project_dependencies += [ dependency('pango') ]
  project_dependencies += [ dependency('pangoxft') ]
  build_args += [ '-DPANGO' ]
endif

if libconfig
  project_dependencies += [ dependency('libconfig') ]
  build_args += [ '-DCONFIG' ]
endif

if xresources
  build_args += [ '-DXRESOURCES' ]
endif

project_target = executable(
  meson.project_name(),
  project_source_files,
  dependencies: project_dependencies,
  install : true,
  c_args : build_args,
)

test('spmenu', project_target)
