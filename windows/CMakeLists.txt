add_subdirectory("resources")

set(SOURCE_FILES ${SOURCE_FILES}
	${CMAKE_CURRENT_LIST_DIR}/disasmwindow.cpp 
	${CMAKE_CURRENT_LIST_DIR}/helpaboutwindow.cpp 
	${CMAKE_CURRENT_LIST_DIR}/helpcommandswindow.cpp 
	${CMAKE_CURRENT_LIST_DIR}/logwindow.cpp 
	${CMAKE_CURRENT_LIST_DIR}/mainwindow.cpp 
	${CMAKE_CURRENT_LIST_DIR}/resourceswindow.cpp 
	${CMAKE_CURRENT_LIST_DIR}/riotwindow.cpp 
	${CMAKE_CURRENT_LIST_DIR}/symbolwindow.cpp 
	${CMAKE_CURRENT_LIST_DIR}/systemwindow.cpp 
	${CMAKE_CURRENT_LIST_DIR}/tiawindow.cpp 
	PARENT_SCOPE)
	