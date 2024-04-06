/*
    SPDX-FileCopyrightText: 2024 Aaron Kirschen <aaronkirschen@gmail.com>
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once

#include <QString>
#include <QKeySequence>

namespace Customization {
    // Unique project name - this must be changed to prevent overwriting other effects made from this template.
    const QString ProjectName = "kwin4_effect_templateeffect";

    // Metadata
    // NOTE: The formatting of this section is important due to how it is processed by CMake to generate metadata.json
    //       The formatting should not be changed unless you also update the CMake code in generate_metadata.cmake.
    const QString EffectName = "TemplateEffect";
    const QString EffectDescription = "Default description; remember to configure metadata in ./src/customization.h";
    const QString EffectCategory = "Accessibility";
    const bool EffectEnabledByDefault = false;
    const QString EffectLicense = "GPL";
    const QStringList EffectServiceTypes = {"KWin/Effect"};
    const QString EffectConfigModule = "kwin4_effect_templateeffect_config";
    const QVector<QPair<QString, QString>> Authors = {
            {"aaronkirschen@gmail.com", "Aaron Kirschen"},
    };

    // Shortcuts
    const QString ToggleActionName = "ToggleTemplateEffect";
    const char *ToggleActionDescription = "Toggle Template Effect on Window";
    const QList<QKeySequence> ToggleActionShortcut = QList<QKeySequence>() << (Qt::CTRL | Qt::ALT | Qt::Key_G);
}