set(effect_SRCS
    template_effect.cpp
    plugin.cpp
)

kconfig_add_kcfg_files(effect_SRCS template_effect_config.kcfgc)
kcoreaddons_add_plugin(${CMAKE_PROJECT_NAME} SOURCES ${effect_SRCS} INSTALL_NAMESPACE "kwin/effects/plugins/")

target_link_libraries(${CMAKE_PROJECT_NAME}
    KWin::kwin
    Qt6::CorePrivate
    Qt6::DBus
    KF6::ConfigGui
    KF6::GlobalAccel
    KF6::I18n
)