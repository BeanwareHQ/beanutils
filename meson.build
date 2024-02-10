project('beanutils', 'c',
  version : '0.1.0',
  default_options : ['warning_level=3'])

src_files = [
  'beanutils/array.c',
  'beanutils/logger.c',
]

inc_dirs = include_directories('./beanutils', './')
beanutils_lib = static_library('beanutils',
  sources: src_files,)
beanutils_dep = declare_dependency(link_with: beanutils_lib,
  include_directories: inc_dirs)

# TODO: nicer tests
executable('beanutils_tests', 'tests.c', include_directories: inc_dirs, dependencies: [beanutils_dep])
