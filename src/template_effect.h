/*
    SPDX-FileCopyrightText: 2024 Aaron Kirschen <aaronkirschen@gmail.com>
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once

#include <set>

#include <qconfig.h>

#include <effect/effecthandler.h>
#include <effect/offscreeneffect.h>

class TemplateEffect final : public KWin::OffscreenEffect {
Q_OBJECT

public:
    /**
     * @brief Constructor for TemplateEffect.
     */
    TemplateEffect();

    ~TemplateEffect() override;

    bool isActive() const override;
    [[nodiscard]] int requestedEffectChainPosition() const override { return 99; }
    [[nodiscard]] bool blocksDirectScanout() const override { return false; }

    /**
     * @brief Check if the effect is enabled by default.
     *
     * This function provides a way for an effect to override the default at runtime, e.g. based on the capabilities of the hardware.
     *
     * This method is optional; the effect doesn't have to provide it.
     *
     * Note that this function is only called if the supported() function returns true, and if X-KDE-PluginInfo-EnabledByDefault is set to true in the .desktop file.
     *
     * This method is optional, by default true is returned.
     *
     * @return True if the effect is enabled by default, false otherwise.
     */
    static bool enabledByDefault();

    /**
     * @brief Check if the effect is supported.
     *
     * An Effect can implement this method to determine at runtime whether the Effect is supported.
     * If the current compositing backend is not supported it should return false.
     *
     * This method is optional, by default true is returned.
     *
     * @return True if the effect is supported, false otherwise.
     */
    static bool supported();

public Q_SLOTS:

    /**
     * @brief Slot called when a window is added.
     * @param window The added window.
     */
    void slotWindowAdded(KWin::EffectWindow *w);

    /**
     * @brief Slot called when a window is closed.
     * @param window The closed window.
     */
    void slotWindowClosed(KWin::EffectWindow *w);

protected:
    /**
     * @brief Load the necessary data for the effect.
     * @return True if the data is loaded successfully, false otherwise.
     */
    bool loadData();

private:
    /**
     * @brief List of windows with the effect applied.
     */
    QList<KWin::EffectWindow *> m_windowsWithEffect;

    /**
     * @brief Flag indicating if the effect is initialized.
     */
    bool m_inited;

    /**
     * @brief Flag indicating if the effect is valid.
     */
    bool m_valid;

    /**
     * @brief Set up the D-Bus connection.
     */
    void setupDBusConnection();

    /**
     * @brief Set up the D-Bus connection.
     */
    void setupShortcuts();

    /**
     * @brief Determine if the effect should be applied to a window.
     * @param window The window to check.
     * @return True if the effect should be applied, false otherwise.
     */
    bool shouldApplyEffectOnWindow(KWin::EffectWindow *w);

    /**
     * @brief Apply the effect on a window.
     * @param window The window to apply the effect on.
     */
    void applyEffectOnWindow(KWin::EffectWindow *w);

    /**
     * @brief Remove the effect from a window.
     * @param window The window to remove the effect from.
     */
    void removeEffectFromWindow(KWin::EffectWindow *w);

    /**
     * @brief Toggle the effect on a specific window.
     * @param window The window to toggle the effect on.
     */
    void toggleEffectOnWindow(KWin::EffectWindow *window);

    /**
     * @brief Toggle the effect on the active window.
     */
    void toggleEffectOnActiveWindow();
};
