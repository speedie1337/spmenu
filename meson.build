project(
  'spmenu',
  'c',
  version : '"1.1"',
  default_options : ['warning_level=3']
)

cc = meson.get_compiler('c')

project_source_files = [
  'libs/libdrw/drw.c',
  'spmenu.c',
  'libs/sl/main.c',
]

project_dependencies = [
  dependency('x11'),
  dependency('xrender'),
  dependency('freetype2'),
  dependency('xft'),
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
  get_option('opt'),
]

if get_option('imlib2') and get_option('openssl')
  project_dependencies += [ dependency('imlib2') ]
  project_dependencies += [ dependency('openssl') ]
  build_args += [ '-DIMAGE' ]
endif

if get_option('fribidi')
  project_dependencies += [ dependency('fribidi') ]
  build_args += [ '-DRTL' ]
endif

if get_option('xinerama')
  project_dependencies += [ dependency('xinerama') ]
  build_args += [ '-DXINERAMA' ]
endif

if get_option('pango') and get_option('pangoxft')
  project_dependencies += [ dependency('pango') ]
  project_dependencies += [ dependency('pangoxft') ]
  build_args += [ '-DPANGO' ]
else
  project_dependencies += [ dependency('fontconfig') ]
endif

if get_option('libconfig')
  project_dependencies += [ dependency('libconfig') ]
  build_args += [ '-DCONFIG' ]
endif

if get_option('xresources')
  build_args += [ '-DXRESOURCES' ]
endif

project_target = executable(
  meson.project_name(),
  project_source_files, install : true,
  dependencies: project_dependencies,
  c_args : build_args,
)

install_man('spmenu.1')
install_data(sources : 'docs/docs.md', install_dir : 'share/spmenu')
install_data(sources : 'docs/code-docs.md', install_dir : 'share/spmenu')
install_data(sources : 'docs/example.Xresources', install_dir : 'share/spmenu')
install_data(sources : 'docs/spmenu.conf', install_dir : 'share/spmenu')
install_data(sources : 'scripts/spmenu_run', install_dir : 'bin')
install_data(sources : 'scripts/spmenu_test', install_dir : 'bin')

test('spmenu', project_target)
