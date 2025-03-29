find_package(Git)

if (GIT_EXECUTABLE)
    execute_process(
            COMMAND ${GIT_EXECUTABLE} describe --tags --abbrev=0
            OUTPUT_VARIABLE GIT_DESCRIBE_VERSION
            RESULT_VARIABLE GIT_DESCRIBE_ERROR_CODE
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT GIT_DESCRIBE_ERROR_CODE)
        set(VI_VERSION ${GIT_DESCRIBE_VERSION})
    endif ()
    execute_process(
            COMMAND ${GIT_EXECUTABLE} log -1 --format=%h
            OUTPUT_VARIABLE GIT_CURRENT_HASH
            RESULT_VARIABLE GIT_HASH_ERROR_CODE
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    if (NOT GIT_HASH_ERROR_CODE)
        set(VI_HASH ${GIT_CURRENT_HASH})
    endif ()
endif ()

if (NOT DEFINED VI_VERSION)
    set(VI_VERSION 0.0.0)
endif ()
if (NOT DEFINED VI_HASH)
    set(VI_HASH "0000000")
endif ()

set (vstring "constexpr char vi_current_version[] { \"${VI_VERSION}\" }\;\n"
             "constexpr char    vi_current_hash[] { \"${VI_HASH}\" }\;\n"
)

file(WRITE vi_version.h.tmp ${vstring})

if (NOT EXISTS "vi_version.h")
    set (gen_ver_missing TRUE)
else ()
    set (gen_ver_missing FALSE)
endif()

if (NOT gen_ver_missing)
    file (SHA256 vi_version.h v_hash)
    file (SHA256 vi_version.h.tmp v_tmp_hash)
endif()

if ((gen_ver_missing) OR (NOT ${v_hash} STREQUAL ${v_tmp_hash}))
    file (COPY_FILE vi_version.h.tmp vi_version.h)
endif()
