srcs = [
  'main.cpp',
  'Server.cpp',
  'Worker.cpp',
  'Service.cpp',
  'ServiceCore.cpp'
]

taxi_dep = declare_dependency(sources: srcs, include_directories: [include_dir,
  include_boost], dependencies: [caf_core, caf_io])
