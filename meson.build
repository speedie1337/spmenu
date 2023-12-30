project(
  'spmenu',
  'c',
  version : '"3.4"',
  default_options : ['warning_level=3']
)

cc = meson.get_compiler('c')

project_source_files = [
  'src/libs/draw/draw.c',
  'src/spmenu.c',
]

project_dependencies = [
  dependency('cairo'),
  dependency('pango'),
  dependency('pangocairo'),
  cc.find_library('m', required : false),
]

build_args = [
  '-DVERSION=' + meson.project_version(),
  '-Wall',
  '-Wno-unused-parameter',
  '-Wno-sign-compare',
]

if get_option('x11')
  project_dependencies += [ dependency('x11') ]
  project_dependencies += [ dependency('xrender') ]
  build_args += [ '-DX11' ]
endif

if get_option('wayland')
  project_dependencies += [ dependency('wayland-client') ]
  project_dependencies += [ dependency('wayland-scanner') ]
  project_dependencies += [ dependency('wayland-protocols') ]
  project_dependencies += [ dependency('xkbcommon') ]

  run_command('protocols/generate.sh', check : true)

  project_source_files += [ 'src/libs/wl/wlr-layer-shell-unstable-v1-client-protocol.h' ]
  project_source_files += [ 'src/libs/wl/wlr-layer-shell-unstable-v1-protocol.c' ]
  project_source_files += [ 'src/libs/wl/xdg-output-unstable-v1-client-protocol.h' ]
  project_source_files += [ 'src/libs/wl/xdg-output-unstable-v1-protocol.c' ]
  project_source_files += [ 'src/libs/wl/xdg-shell-client-protocol.h' ]
  project_source_files += [ 'src/libs/wl/xdg-shell-protocol.c' ]
  build_args += [ '-DWAYLAND' ]
endif

if get_option('imlib2') and get_option('openssl')
  project_dependencies += [ dependency('imlib2') ]
  project_dependencies += [ dependency('openssl') ]
  build_args += [ '-DIMAGE' ]
endif

if get_option('fribidi')
  project_dependencies += [ dependency('fribidi') ]
  build_args += [ '-DRTL' ]
endif

if get_option('xinerama') and get_option('x11')
  project_dependencies += [ dependency('xinerama') ]
  build_args += [ '-DXINERAMA' ]
endif

if get_option('libconfig')
  project_dependencies += [ dependency('libconfig') ]
  build_args += [ '-DCONFIG' ]
endif

if get_option('xresources') and get_option('x11')
  build_args += [ '-DXRESOURCES' ]
endif

if get_option('regex')
  build_args += [ '-DREGEX' ]
endif

if get_option('fifo')
  build_args += [ '-DFIFO' ]
endif

project_target = executable(
  meson.project_name(),
  project_source_files, install : true,
  dependencies: project_dependencies,
  c_args : build_args,
)

if get_option('man') and get_option('docs')
  install_man('spmenu.1')
  install_man('spmenu_test.1')

  if get_option('run') and get_option('docs')
    install_man('spmenu_run.1')
  endif
endif

if get_option('docs')
  install_data(sources : 'docs/spmenu.conf', install_dir : 'share/spmenu')
  install_data(sources : 'docs/spmenu_run.desktop', install_dir : 'share/applications')
  install_data(sources : 'docs/spmenu_desktop.desktop', install_dir : 'share/applications')
  install_data(sources : 'docs/spmenu_filemanager.desktop', install_dir : 'share/applications')
  install_data(sources : 'docs/spmenu.svg', install_dir : 'share/icons/hicolor/scalable/apps')
endif

if get_option('run')
  install_data(sources : 'scripts/spmenu_run', install_dir : 'bin')
  install_data(sources : 'scripts/spmenu_path', install_dir : 'bin')
  install_data(sources : 'scripts/spmenu_fm', install_dir : 'bin')
  install_data(sources : 'scripts/spmenu_desktop', install_dir : 'bin')
endif

if get_option('test')
  install_data(sources : 'scripts/spmenu_test', install_dir : 'bin')
endif

test('spmenu', project_target)
