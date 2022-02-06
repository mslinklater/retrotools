add_subdirectory("window")
add_subdirectory("lua")

set(SOURCE_FILES ${SOURCE_FILES}
	${CMAKE_CURRENT_LIST_DIR}/log.cpp 
	${CMAKE_CURRENT_LIST_DIR}/commands.cpp 
	${CMAKE_CURRENT_LIST_DIR}/commandhelpers.cpp 
	${CMAKE_CURRENT_LIST_DIR}/commandcenter.cpp
	${CMAKE_CURRENT_LIST_DIR}/stateserialiser.cpp
	${CMAKE_CURRENT_LIST_DIR}/errorcodes.cpp
	${CMAKE_CURRENT_LIST_DIR}/usercommands.cpp
	${CMAKE_CURRENT_LIST_DIR}/application.cpp
	PARENT_SCOPE)

