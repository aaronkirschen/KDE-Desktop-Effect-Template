/*
    SPDX-FileCopyrightText: 2015 Robert Metsäranta <therealestrob@gmail.com>
    SPDX-FileCopyrightText: 2024 Aaron Kirschen <aaronkirschen@gmail.com>

    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "template_effect.h"

KWIN_EFFECT_FACTORY_SUPPORTED_ENABLED(
    TemplateEffect, "metadata.json",
    return TemplateEffect::supported();,
    return TemplateEffect::enabledByDefault();)

#include "plugin.moc"
