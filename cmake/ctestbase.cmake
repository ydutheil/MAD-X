execute_process(COMMAND cat ${SOURCEDIR}/${TEST_NAME}.madx COMMAND ${TEST_PROG}
   OUTPUT_FILE "${TEST_NAME}.out" WORKING_DIRECTORY ${SOURCEDIR} RESULT_VARIABLE HAD_ERROR)

# We must make a list so it is understood as multiple
# input arguments..
string(REGEX REPLACE " " ";" _TEST_OUTPUT ${TEST_OUTPUT})

if(HAD_ERROR)
    message(FATAL_ERROR "Test failed with error ${HAD_ERROR}")
    else()
    execute_process(COMMAND ${NUMDIFF} -q -b -c -l -n -t ${TEST_NAME} ${_TEST_OUTPUT} WORKING_DIRECTORY ${SOURCEDIR} RESULT_VARIABLE NUMDIFF_ERROR)
    if(NUMDIFF_ERROR)
       message(FATAL_ERROR "Test failed with numdiff error ${NUMDIFF_ERROR}")
    endif()
endif()


