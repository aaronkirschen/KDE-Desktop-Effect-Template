file(READ "${INPUT_FILE}" HEADER_CONTENT)

string(REGEX MATCH "EffectName[ \t\r\n]*=[ \t\r\n]*\"([^\"]*)\"" _ "${HEADER_CONTENT}")
set(EFFECT_NAME "${CMAKE_MATCH_1}")

string(REGEX MATCH "EffectDescription[ \t\r\n]*=[ \t\r\n]*\"([^\"]*)\"" _ "${HEADER_CONTENT}")
set(EFFECT_DESCRIPTION "${CMAKE_MATCH_1}")

string(REGEX MATCH "EffectCategory[ \t\r\n]*=[ \t\r\n]*\"([^\"]*)\"" _ "${HEADER_CONTENT}")
set(EFFECT_CATEGORY "${CMAKE_MATCH_1}")

string(REGEX MATCH "EffectEnabledByDefault[ \t\r\n]*=[ \t\r\n]*(true|false)" _ "${HEADER_CONTENT}")
set(EFFECT_ENABLED_BY_DEFAULT "${CMAKE_MATCH_1}")

string(REGEX MATCH "EffectLicense[ \t\r\n]*=[ \t\r\n]*\"([^\"]*)\"" _ "${HEADER_CONTENT}")
set(EFFECT_LICENSE "${CMAKE_MATCH_1}")

string(REGEX MATCH "EffectServiceTypes[ \t\r\n]*=[ \t\r\n]*\\{([^}]*)\\}" _ "${HEADER_CONTENT}")
string(REPLACE "\"" "" EFFECT_SERVICE_TYPES "${CMAKE_MATCH_1}")
string(STRIP "${EFFECT_SERVICE_TYPES}" EFFECT_SERVICE_TYPES)
string(REPLACE ", " "," EFFECT_SERVICE_TYPES "${EFFECT_SERVICE_TYPES}")
string(REPLACE "," "\",\"" EFFECT_SERVICE_TYPES "${EFFECT_SERVICE_TYPES}")
set(EFFECT_SERVICE_TYPES "\"${EFFECT_SERVICE_TYPES}\"")

string(REGEX MATCH "EffectConfigModule[ \t\r\n]*=[ \t\r\n]*\"([^\"]*)\"" _ "${HEADER_CONTENT}")
set(EFFECT_CONFIG_MODULE "${CMAKE_MATCH_1}")

set(AUTHORS "")

string(REGEX MATCH "Authors[ \t\r\n]*=[ \t\r\n]*\{([^{}\]*\{[^}]*\}[^{}\]*)+\}[ \t\r\n]*,?[ \t\r\n]*\}?[ \t\r\n]*;?" AUTHORS_MATCH "${HEADER_CONTENT}")

string(REGEX MATCH "\\{[^}]*\\}[ \t\r\n]*,?[ \t\r\n]*\\}?" AUTHORS_CONTENT "${AUTHORS_MATCH}")
string(STRIP "${AUTHORS_CONTENT}" AUTHORS_CONTENT)

string(REGEX MATCHALL "\\{\"([^\"]*)\",[ \t\r\n]*\"([^\"]*)\"\\}" AUTHORS_MATCHES "${AUTHORS_MATCH}")

set(AUTHORS "")
foreach(AUTHOR_MATCH IN LISTS AUTHORS_MATCHES)
    string(REGEX REPLACE ".*\\{\"([^\"]*)\".*" "\\1" AUTHOR_EMAIL "${AUTHOR_MATCH}")
    string(REGEX REPLACE ".*\"([^\"]*)\"\\}.*" "\\1" AUTHOR_NAME "${AUTHOR_MATCH}")

    if(AUTHORS STREQUAL "")
        set(AUTHORS "{
                \"Email\": \"${AUTHOR_EMAIL}\",
                \"Name\": \"${AUTHOR_NAME}\"
            }")
    else()
        set(AUTHORS "${AUTHORS},
            {
                \"Email\": \"${AUTHOR_EMAIL}\",
                \"Name\": \"${AUTHOR_NAME}\"
            }")
    endif()
endforeach()

configure_file("${METADATA_TEMPLATE}" "${OUTPUT_FILE}" @ONLY)