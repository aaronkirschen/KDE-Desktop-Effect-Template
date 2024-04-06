/*
    SPDX-FileCopyrightText: 2024 Aaron Kirschen <aaronkirschen@gmail.com>
    SPDX-License-Identifier: GPL-3.0-or-later
*/

#pragma once

#include <effect/effecthandler.h>
#include <effect/offscreeneffect.h>

class TemplateEffect final : public KWin::Effect {
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

private Q_SLOTS:
    /**
     * @brief Slot called when a window is added.
     * @param effectWindow The added window.
     */
    void slotWindowAdded(KWin::EffectWindow *effectWindow);

    /**
     * @brief Slot called when a window is closed.
     * @param effectWindow The closed window.
     */
    void slotWindowClosed(KWin::EffectWindow *effectWindow);

protected:
    /**
     * @brief Initializes and loads the necessary data for the effect to function properly.
     *
     * This method is used to load all required resources and data
     * needed by the effect. It is called during the object construction phase of the
     * TemplateEffect instance. If any part of the data loading process fails, the
     * effect will be marked as invalid and will not be displayed in the user interface.
     *
     * The 'm_valid' flag is set based on the outcome of
     * this method. A return value of 'true' indicates that all necessary data was loaded
     * successfully and the effect is ready to be used. Conversely, a return value of
     * 'false' signifies a failure to load the necessary data, leading to the effect being
     * considered invalid.
     *
     * @return True if the data is loaded successfully, false otherwise.
     */
    bool loadData();

private:
    /**
     * @brief List of windows with the effect applied.
     */
    QList<KWin::EffectWindow *> m_windowsWithEffect;

    /**
     * @brief Flag indicating if the effect has successfully completed its initialization process.
     *
     * Set by the loadData() method, this flag indicates if the initial loading of data
     * and resources required for the effect's operation has been completed.
     *
     * This flag is used for ensuring that initialization procedures, such as resource
     * loading and initial configuration, are only executed once and have been adequately addressed.
     */
    bool m_inited;

    /**
     * @brief Flag indicating if the effect is in a state that can operate correctly.
     *
     * Determined by the outcome of the loadData() method, 'm_valid' reflects the effect's readiness
     * and capability to function as intended.
     *
     * A value of 'true' indicates that all necessary data
     * and resources have been successfully loaded and initialized, allowing the effect to perform
     * its operations.
     *
     * Conversely, a 'false' value indicates issues encountered during the data
     * loading process, which will prevent the effect from operating correctly.
     */
    bool m_valid;

    /**
     * @brief Establishes the D-Bus connection necessary for the effect's operation.
     *
     * This method initializes the D-Bus connection by attempting to connect to the session bus
     * and register the service and object required for the effect's communication. If the connection
     * or registration fails, it logs the issue but does not halt the effect's execution.
     *
     * Successful D-Bus setup enables the effect to communicate with other applications and services
     * over D-Bus, allowing for remote control and interaction. This setup is critical for effects
     * that rely on external triggers or need to expose functionality to other applications.
     */
    void setupDBusConnection();

    /**
     * @brief Configures keyboard shortcuts for toggling the effect and establishes their signal connections.
     *
     * This method sets up global keyboard shortcuts using the KGlobalAccel framework, defining how users
     * can activate or deactivate the effect. It associates these shortcuts with actions and connects them
     * to the appropriate slots to handle the effect's toggling.
     *
     * Each shortcut is connected to a slot that executes the corresponding action,
     * ensuring immediate response to user inputs.
     */
    void setupShortcuts();


    /**
     * @brief Determine if the effect is currently applied on a window.
     *
     * This method checks the m_windowsWithEffect list to see if the given window
     * is present, indicating that the effect is currently applied on that window.
     *
     * @param effectWindow The window to check.
     * @return True if the effect is currently applied, false otherwise.
     */
    bool isEffectAppliedOnWindow(KWin::EffectWindow *effectWindow);

    /**
     * @brief Determine if the effect should be automatically applied to a window.
     *
     * This method is called when a window is added to decide whether the effect
     * should be automatically applied based on custom conditions.
     *
     * Implement your custom conditions in this method to control which windows
     * should have the effect automatically applied when they are added.
     *
     * @param effectWindow The window to check.
     * @return True if the effect should be automatically applied, false otherwise.
     */
    bool shouldApplyEffectOnWindow(KWin::EffectWindow *effectWindow);

    /**
     * @brief Apply the effect on a window.
     *
     * This method adds the given window to the m_windowsWithEffect list and
     * performs any necessary setup or initialization for the effect on that window.
     *
     * It should be called when the effect needs to be applied on a window, either
     * automatically when the window is added or manually through the toggle methods.
     *
     * @param effectWindow The window to apply the effect on.
     */
    void applyEffectOnWindow(KWin::EffectWindow *effectWindow);

    /**
     * @brief Remove the effect from a window.
     *
     * This method removes the given window from the m_windowsWithEffect list and
     * performs any necessary cleanup or teardown for the effect on that window.
     *
     * It should be called when the effect needs to be removed from a window, either
     * when the window is closed or manually through the toggle methods.
     *
     * @param effectWindow The window to remove the effect from.
     */
    void removeEffectFromWindow(KWin::EffectWindow *effectWindow);

    /**
     * @brief Toggle the effect on a specific window.
     *
     * This method checks if the effect is currently applied on the given window
     * using isEffectAppliedOnWindow().
     *
     * If the effect is applied, it removes the effect using removeEffectFromWindow().
     *
     * If the effect is not applied, it applies the effect using applyEffectOnWindow().
     *
     * @param effectWindow The window to toggle the effect on.
     */
    void toggleEffectOnWindow(KWin::EffectWindow *effectWindow);

    /**
     * @brief Toggle the effect on the active window.
     *
     * This method is triggered by a shortcut to manually toggle the effect on the
     * currently active window, regardless of the conditions specified in
     * shouldApplyEffectOnWindow().
     *
     * It retrieves the active window using KWin::effects->activeWindow() and
     * passes it to toggleEffectOnWindow() to toggle the effect on that window.
     *
     * If the effect is already applied on the active window, it will be removed.
     *
     * If the effect is not applied, it will be applied.
     */
    void toggleEffectOnActiveWindow();

    /**
     * @brief Handles the implementation of applying the effect on a window.
     *
     * This method is called by applyEffectOnWindow() to perform the actual
     * implementation of applying the effect on the given window.
     *
     * Implement your custom effect behavior for applying the effect in this method,
     * such as modifying the window's properties, applying shaders, or handling animations.
     *
     * @param effectWindow The window that the effect will be applied on.
     */
    void handleApplyEffectOnWindow(KWin::EffectWindow *effectWindow);

    /**
     * @brief Handles the implementation of removing the effect from a window.
     *
     * This method is called by removeEffectFromWindow() to perform the actual
     * implementation of removing the effect from the given window.
     *
     * Implement your custom effect behavior for removing the effect in this method,
     * such as reverting the window's properties, removing shaders, or cleaning up animations.
     *
     * @param effectWindow The window that the effect will be removed from.
     */
    void handleRemoveEffectFromWindow(KWin::EffectWindow *effectWindow);
};