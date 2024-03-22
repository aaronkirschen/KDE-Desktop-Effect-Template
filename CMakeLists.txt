cmake_minimum_required(VERSION 3.20)

project(kwin4_effect_templateeffect)

set(KF_MIN_VERSION "5.78")
set(QT_MIN_VERSION "6.4.0")
set(QT_MAJOR_VERSION 6)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose Release or Debug" FORCE)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -DQT_NO_DEBUG_OUTPUT")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${ECM_MODULE_PATH}
        ${ECM_KDE_MODULE_DIR}
)

include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

find_package(KF6 REQUIRED COMPONENTS
        ConfigWidgets
        KCMUtils
)

find_package(KWin REQUIRED COMPONENTS
        kwineffects
)
set(KWIN_EFFECT_INCLUDE_FILE "/usr/include/kwin/effect/effect.h")

find_package(epoxy REQUIRED)
find_package(XCB REQUIRED COMPONENTS XCB)

find_package(KWinDBusInterface CONFIG REQUIRED)

add_subdirectory(src)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

execute_process(
        COMMAND sh -c "grep '#define KWIN_EFFECT_API_VERSION_MINOR' ${KWIN_EFFECT_INCLUDE_FILE} | awk '{print \$NF}'"
        OUTPUT_VARIABLE KWIN_EFFECT_API_VERSION_MINOR OUTPUT_STRIP_TRAILING_WHITESPACE
)
message("KWinEffect API Version: ${KWIN_EFFECT_API_VERSION_MINOR}")
message("")