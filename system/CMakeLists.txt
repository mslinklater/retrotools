add_subdirectory("window")
add_subdirectory("lua")

set(SOURCE_FILES ${SOURCE_FILES}
	${CMAKE_CURRENT_LIST_DIR}/log.cpp 
	${CMAKE_CURRENT_LIST_DIR}/message/messages.cpp 
	${CMAKE_CURRENT_LIST_DIR}/message/messagehelpers.cpp 
	${CMAKE_CURRENT_LIST_DIR}/message/messagecenter.cpp
	${CMAKE_CURRENT_LIST_DIR}/stateserialiser.cpp
	${CMAKE_CURRENT_LIST_DIR}/errorcodes.cpp
	${CMAKE_CURRENT_LIST_DIR}/usercommands.cpp
	${CMAKE_CURRENT_LIST_DIR}/application.cpp
	PARENT_SCOPE)

set(TESTRUNNER_FILES ${TESTRUNNER_FILES}
	${CMAKE_CURRENT_LIST_DIR}/message/message_test.cpp 
	PARENT_SCOPE)
