install(
    TARGETS acacia-challenge_exe
    RUNTIME COMPONENT acacia-challenge_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
