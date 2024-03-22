/*
    SPDX-FileCopyrightText: 2024 Aaron Kirschen <aaronkirschen@gmail.com>
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "template_effect.h"
#include "template_effect_config.h"

#include <KX11Extras>
#include <QDBusError>
#include <QtDBus/QDBusConnection>

#include <core/output.h>
#include <core/renderviewport.h>
#include <effect/effecthandler.h>
#include <opengl/glutils.h>

#include <KGlobalAccel>
#include <KLocalizedString>
#include <QAction>

TemplateEffect::TemplateEffect()
        : m_inited(false), m_valid(true) {
    reconfigure(ReconfigureAll);

    if (!m_inited) {
        m_valid = loadData();
    }

    setupDBusConnection();

    setupShortcuts();

    connect(KWin::effects, &KWin::EffectsHandler::windowAdded, this,
            &TemplateEffect::slotWindowAdded);
    connect(KWin::effects, &KWin::EffectsHandler::windowClosed, this,
            &TemplateEffect::slotWindowClosed);
}

TemplateEffect::~TemplateEffect() = default;

bool TemplateEffect::supported() {
    return KWin::effects->isOpenGLCompositing();
}

bool TemplateEffect::enabledByDefault() {
    return supported();
}

bool TemplateEffect::isActive() const {
    return m_valid && !m_windowsWithEffect.isEmpty();
}
void TemplateEffect::setupShortcuts() {
    auto *toggleAction = new QAction(this);
    toggleAction->setObjectName(QStringLiteral("ToggleTemplateEffect"));
    toggleAction->setText(i18n("Toggle Template Effect on Window"));
    KGlobalAccel::self()->setDefaultShortcut(toggleAction, QList<QKeySequence>() << (Qt::CTRL | Qt::ALT | Qt::Key_G));
    KGlobalAccel::self()->setShortcut(toggleAction, QList<QKeySequence>() << (Qt::CTRL | Qt::ALT | Qt::Key_G));
    connect(toggleAction, &QAction::triggered, this,
            &TemplateEffect::toggleEffectOnActiveWindow);
}

void TemplateEffect::setupDBusConnection() {
    auto connection = QDBusConnection::sessionBus();
    if (!connection.isConnected()) {
        qWarning("TemplateEffect: Cannot connect to the D-Bus session bus.\n");
        return;
    }

    if (!connection.registerService("org.kde.TemplateEffect")) {
        qWarning("%s\n", qPrintable(connection.lastError().message()));
        return;
    }

    if (!connection.registerObject("/TemplateEffect", this,
                                   QDBusConnection::ExportAllSlots)) {
        qWarning("%s\n", qPrintable(connection.lastError().message()));
        return;
    }
}

bool TemplateEffect::loadData() {
    m_inited = true;
    return m_inited;
}

bool TemplateEffect::shouldApplyEffectOnWindow(KWin::EffectWindow *window) {
    if (m_windowsWithEffect.contains(window)) {
        return true;
    }
    return false;
}

void TemplateEffect::applyEffectOnWindow(KWin::EffectWindow *window) {
    // Apply the effect on the window
    redirect(window);
    qInfo() << "Effect applied on window" << window ;
}

void TemplateEffect::removeEffectFromWindow(KWin::EffectWindow *window) {
    // Remove the effect on the window
    unredirect(window);
    qInfo() << "Effect removed on window" << window ;

}

void TemplateEffect::toggleEffectOnWindow(KWin::EffectWindow *window) {
    if (!m_windowsWithEffect.contains(window)) {
        m_windowsWithEffect.append(window);
    } else {
        m_windowsWithEffect.removeOne(window);
    }
    if (shouldApplyEffectOnWindow(window)) {
        applyEffectOnWindow(window);
    } else {
        removeEffectFromWindow(window);
    }
    window->addRepaintFull();
    qInfo() << "Effect toggled on window" << window ;

}

void TemplateEffect::toggleEffectOnActiveWindow() {
    auto window = KWin::effects->activeWindow();
    if (!window) {
        qWarning("TemplateEffect: No active window to toggle effect on.");
        return;
    }
    toggleEffectOnWindow(window);
}

void TemplateEffect::slotWindowAdded(KWin::EffectWindow *window) {
    if (shouldApplyEffectOnWindow(window)) {
        applyEffectOnWindow(window);
    }
}

void TemplateEffect::slotWindowClosed(KWin::EffectWindow *window) {
    if (shouldApplyEffectOnWindow(window)) {
        removeEffectFromWindow(window);
    }
}

