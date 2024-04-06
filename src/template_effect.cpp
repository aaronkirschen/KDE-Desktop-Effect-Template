/*
    SPDX-FileCopyrightText: 2024 Aaron Kirschen <aaronkirschen@gmail.com>
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#include "template_effect.h"
#include "template_effect_config.h"
#include "customization.h"

#include <QDBusError>
#include <QtDBus/QDBusConnection>

#include <KGlobalAccel>
#include <KLocalizedString>
#include <QAction>

TemplateEffect::TemplateEffect()
        : m_inited(false), m_valid(false) {
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

bool TemplateEffect::loadData() {
    m_inited = true;
    return m_inited;
}

void TemplateEffect::setupDBusConnection() {
    auto connection = QDBusConnection::sessionBus();
    if (!connection.isConnected()) {
        qWarning() << Customization::EffectName << ": Cannot connect to the D-Bus session bus.";
        return;
    }

    if (!connection.registerService("org.kde." + Customization::EffectName)) {
        qWarning() << Customization::EffectName << ": " << connection.lastError().message();
        return;
    }

    if (!connection.registerObject("/" + Customization::EffectName, this,
                                   QDBusConnection::ExportAllSlots)) {
        qWarning() << Customization::EffectName << ": " << connection.lastError().message();
        return;
    }
}

void TemplateEffect::setupShortcuts() {
    auto *toggleAction = new QAction(this);
    toggleAction->setObjectName(Customization::ToggleActionName);
    toggleAction->setText(i18n(Customization::ToggleActionDescription));
    KGlobalAccel::self()->setDefaultShortcut(toggleAction, Customization::ToggleActionShortcut);
    KGlobalAccel::self()->setShortcut(toggleAction, Customization::ToggleActionShortcut);
    connect(toggleAction, &QAction::triggered, this,
            &TemplateEffect::toggleEffectOnActiveWindow);
}

bool TemplateEffect::shouldApplyEffectOnWindow(KWin::EffectWindow *effectWindow) {
    // Add conditions here to determine if the effect should be applied on the window
    // For example:
    // if (effectWindow->isNormalWindow() && effectWindow->isMovable()) {
    //     return true;
    // }
    // return false;
    return false; // Placeholder, replace with actual conditions
}

bool TemplateEffect::isEffectAppliedOnWindow(KWin::EffectWindow *effectWindow) {
    return m_windowsWithEffect.contains(effectWindow);
}

void TemplateEffect::slotWindowAdded(KWin::EffectWindow *effectWindow) {
    if (!effectWindow) {
        qWarning() << Customization::EffectName << ": Null window pointer in slotWindowAdded";
        return;
    }

    if (shouldApplyEffectOnWindow(effectWindow) && !isEffectAppliedOnWindow(effectWindow)) {
        applyEffectOnWindow(effectWindow);
    }
}

void TemplateEffect::slotWindowClosed(KWin::EffectWindow *effectWindow) {
    if (!effectWindow) {
        qWarning() << Customization::EffectName << ": Null window pointer in slotWindowClosed";
    }

    if (isEffectAppliedOnWindow(effectWindow)) {
        removeEffectFromWindow(effectWindow);
    }
}

void TemplateEffect::applyEffectOnWindow(KWin::EffectWindow *effectWindow) {
    if (m_windowsWithEffect.contains(effectWindow)) {
        qWarning() << Customization::EffectName << ": applyEffectOnWindow called on a window that already has effect!" << effectWindow;
        return;
    }

    m_windowsWithEffect.append(effectWindow);
    qDebug() << Customization::EffectName << ": Effect applied on window" << effectWindow;
    handleApplyEffectOnWindow(effectWindow);
}

void TemplateEffect::handleApplyEffectOnWindow(KWin::EffectWindow *effectWindow) {
    qDebug() << Customization::EffectName << ": Handling adding effect on window" << effectWindow;
}

void TemplateEffect::removeEffectFromWindow(KWin::EffectWindow *effectWindow) {
    if (!m_windowsWithEffect.contains(effectWindow)) {
        qWarning() << Customization::EffectName << ": removeEffectFromWindow called on a window that does not have effect!" << effectWindow;
        return;
    }
    m_windowsWithEffect.removeOne(effectWindow);
    qDebug() << Customization::EffectName << ": Effect removed on window" << effectWindow;
    handleRemoveEffectFromWindow(effectWindow);
}

void TemplateEffect::handleRemoveEffectFromWindow(KWin::EffectWindow *effectWindow) {
    qDebug() << Customization::EffectName << ": Handling removing effect from window" << effectWindow;
}

void TemplateEffect::toggleEffectOnWindow(KWin::EffectWindow *effectWindow) {
    qDebug() << Customization::EffectName << ": Toggling effect on window" << effectWindow;
    if (isEffectAppliedOnWindow(effectWindow)) {
        qDebug() << Customization::EffectName << ": Removing effect from window" << effectWindow;
        removeEffectFromWindow(effectWindow);
    } else {
        qDebug() << Customization::EffectName << ": Applying effect on window" << effectWindow;
        applyEffectOnWindow(effectWindow);
    }
}

void TemplateEffect::toggleEffectOnActiveWindow() {
    auto effectWindow = KWin::effects->activeWindow();
    if (!effectWindow) {
        qWarning() << Customization::EffectName << ": No active window to toggle effect on.";
        return;
    }
    qDebug() << Customization::EffectName << ": Toggling effect on active window" << effectWindow;
    toggleEffectOnWindow(effectWindow);
}