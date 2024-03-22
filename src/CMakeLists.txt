set(effect_SRCS
    template_effect.cpp
    plugin.cpp
)

kconfig_add_kcfg_files(effect_SRCS template_effect_config.kcfgc)
kcoreaddons_add_plugin(kwin4_effect_templateeffect SOURCES ${effect_SRCS} INSTALL_NAMESPACE "kwin/effects/plugins/")

find_package(KF6GlobalAccel REQUIRED)
find_package(KF6I18n REQUIRED)

target_link_libraries(kwin4_effect_templateeffect
    KWin::kwin
    Qt6::CorePrivate
    Qt6::DBus
    KF6::ConfigGui
    KF6::GlobalAccel
    KF6::I18n
)

install(DIRECTORY shaders DESTINATION ${KDE_INSTALL_DATADIR}/kwin)


