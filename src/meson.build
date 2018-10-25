srcs = [
  'main.cpp',
  'TaxiServer/TaxiServer.cpp',
  'TaxiServer/TaxiWorker.cpp',
  'TaxiService/TaxiService.cpp',
  'TaxiService/TaxiServiceCore.cpp'
]

taxi_dep = declare_dependency(sources: srcs, include_directories: [include_dir,
  include_boost], dependencies: [caf_core, caf_io])
