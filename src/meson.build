srcs = [
  'main.cpp',
  'Tools/Haversine.cpp',
  'TaxiServer/TaxiServer.cpp',
  'TaxiServer/TaxiWorker.cpp',
  'TaxiService/TaxiService.cpp',
  'TaxiService/TaxiServiceCore.cpp',
  'ClientServer/ClientServer.cpp',
  'ClientServer/ClientWorker.cpp',
  'ClientService/ClientService.cpp',
  'ClientService/ClientServiceCore.cpp'
]

taxi_dep = declare_dependency(sources: srcs, include_directories: [include_dir,
  include_boost], dependencies: [caf_core, caf_io])
