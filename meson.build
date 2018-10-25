project('SmartTaxiService', 'cpp', default_options: 'cpp_std=c++14')

caf_core = dependency('caf-core')
caf_io = dependency('caf-io')

include_dir = include_directories('include')
include_boost = include_directories('/usr/local/boost_1_67_0')

subdir('src')

executable('taxi', dependencies: taxi_dep)
