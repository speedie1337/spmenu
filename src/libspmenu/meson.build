# libspmenu
project('spmenu', 'cpp', version : '3.4')
cc = meson.get_compiler('cpp')

project_source_files = [
  'libspmenu.cpp',
]

project_dependencies = [
]

build_args = [
  '-DVERSION=' + meson.project_version(),
]

so_version = meson.project_version()

lib_shared = shared_library('spmenu', project_source_files, version : so_version, install : true)
