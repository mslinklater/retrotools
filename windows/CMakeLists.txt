add_subdirectory("resources")

set(SOURCE_FILES ${SOURCE_FILES}
	${CMAKE_CURRENT_LIST_DIR}/helpaboutwindow.cpp 
	${CMAKE_CURRENT_LIST_DIR}/helpcommandswindow.cpp 
	${CMAKE_CURRENT_LIST_DIR}/logwindow.cpp 
	${CMAKE_CURRENT_LIST_DIR}/mainwindow.cpp 
	${CMAKE_CURRENT_LIST_DIR}/resourceswindow.cpp 
	${CMAKE_CURRENT_LIST_DIR}/luawindow.cpp 
	PARENT_SCOPE)
	