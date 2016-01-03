
set(_H ".*\\.h")
set(H  "${_H}$")

set(_CPP ".*\\.cpp")
set(CPP  "${_CPP}$")

set(H_CPP "(${H}|${CPP})")

source_group("Resource files" ".*\\.(rc|ico)")

source_group("api"		"${CMAKE_CURRENT_SOURCE_DIR}/api/${H_CPP}")
source_group("ccmain"	"${CMAKE_CURRENT_SOURCE_DIR}/ccmain/${H_CPP}")
source_group("ccstruct"	"${CMAKE_CURRENT_SOURCE_DIR}/ccstruct/${H_CPP}")
source_group("ccutil"	"${CMAKE_CURRENT_SOURCE_DIR}/ccutil/${H_CPP}")
source_group("classify"	"${CMAKE_CURRENT_SOURCE_DIR}/classify/${H_CPP}")
source_group("cube"		"${CMAKE_CURRENT_SOURCE_DIR}/cube/${H_CPP}")
source_group("cutil"	"${CMAKE_CURRENT_SOURCE_DIR}/cutil/${H_CPP}")
source_group("dict"		"${CMAKE_CURRENT_SOURCE_DIR}/dict/${H_CPP}")
source_group("neural"	"${CMAKE_CURRENT_SOURCE_DIR}/neural_networks/runtime/${H_CPP}")
source_group("opencl"	"${CMAKE_CURRENT_SOURCE_DIR}/opencl/${H_CPP}")
source_group("textord"	"${CMAKE_CURRENT_SOURCE_DIR}/textord/${H_CPP}")
source_group("viewer"	"${CMAKE_CURRENT_SOURCE_DIR}/viewer/${H_CPP}")
source_group("port"		"${CMAKE_CURRENT_SOURCE_DIR}/vs2010/port/${H_CPP}")
source_group("wordrec"	"${CMAKE_CURRENT_SOURCE_DIR}/wordrec/${H_CPP}")
