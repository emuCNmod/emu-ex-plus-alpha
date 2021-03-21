#pragma once

/*  This file is part of Imagine.

	Imagine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Imagine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Imagine.  If not, see <http://www.gnu.org/licenses/> */

#include <imagine/config/defs.hh>

#if defined CONFIG_BASE_X11
#include <imagine/base/x11/XApplicationContext.hh>
#elif defined CONFIG_BASE_ANDROID
#include <imagine/base/android/AndroidApplicationContext.hh>
#elif defined CONFIG_BASE_IOS
#include <imagine/base/iphone/IOSApplicationContext.hh>
#endif

#include <imagine/util/bitset.hh>
#include <imagine/base/baseDefs.hh>
#include <imagine/fs/FSDefs.hh>
#include <imagine/io/FileIO.hh>
#include <vector>

namespace Base
{

enum class Permission
{
	WRITE_EXT_STORAGE,
	COARSE_LOCATION
};

static constexpr uint32_t
	SYS_UI_STYLE_NO_FLAGS = 0,
	SYS_UI_STYLE_DIM_NAV = bit(0),
	SYS_UI_STYLE_HIDE_NAV = bit(1),
	SYS_UI_STYLE_HIDE_STATUS = bit(2);

class ApplicationContext : public ApplicationContextImpl
{
public:
	using ApplicationContextImpl::ApplicationContextImpl;

	// defined in user code
	static const char *const applicationName;
	static const char *const applicationId;

	// Called on app startup, defined in user code
	[[gnu::cold]] static void onInit(ApplicationContext, int argc, char** argv);

	Window *makeWindow(WindowConfig, WindowInitDelegate);
	unsigned windows() const;
	Window *window(unsigned idx) const;
	Window &mainWindow();

	Screen &addScreen(std::unique_ptr<Screen>, bool notify);
	Screen *findScreen(ScreenId) const;
	std::unique_ptr<Screen> removeScreen(ScreenId, bool notify);
	void removeSecondaryDisplays();
	unsigned screens() const;
	Screen *screen(unsigned idx) const;
	Screen &mainScreen();
	bool screensArePosted() const;
	void setActiveForAllScreens(bool active);

	// App Callbacks

	// Called when another process sends the app a message
	void setOnInterProcessMessage(InterProcessMessageDelegate);
	void dispatchOnInterProcessMessage(const char *filename);
	bool hasOnInterProcessMessage() const;

	// Called when a Screen is connected/disconnected or its properties change
	void setOnScreenChange(ScreenChangeDelegate del);

	// Called when app returns from backgrounded state
	bool addOnResume(ResumeDelegate, int priority = APP_ON_RESUME_PRIORITY);
	bool removeOnResume(ResumeDelegate);
	bool containsOnResume(ResumeDelegate) const;
	void dispatchOnResume(bool focused);

	// Called when OS needs app to free any cached data
	void setOnFreeCaches(FreeCachesDelegate del);
	void dispatchOnFreeCaches(bool running);

	// Called when app will finish execution
	// If backgrounded == true, app may eventually resume execution
	bool addOnExit(ExitDelegate, int priority = APP_ON_EXIT_PRIORITY);
	bool removeOnExit(ExitDelegate);
	bool containsOnExit(ExitDelegate) const;
	void dispatchOnExit(bool backgrounded);

	// Called when device changes orientation and the sensor is enabled
	void setOnDeviceOrientationChanged(DeviceOrientationChangedDelegate);

	// Called when system UI changes orientation
	void setOnSystemOrientationChanged(SystemOrientationChangedDelegate);

	// App run state
	bool isRunning() const;

	// Inter-process messages
	void registerInstance(int argc, char** argv, const char *appId = applicationId);
	void setAcceptIPC(bool on, const char *appId = applicationId);

	// external services
	void openURL(const char *url);

	// file system paths & asset loading, thread-safe
	FS::PathString assetPath(const char *appName = applicationName);
	FS::PathString libPath(const char *appName = applicationName);
	FS::PathString supportPath(const char *appName = applicationName);
	FS::PathString cachePath(const char *appName = applicationName);
	FS::PathString sharedStoragePath();
	FS::PathLocation sharedStoragePathLocation();
	std::vector<FS::PathLocation> rootFileLocations();
	FS::RootPathInfo nearestRootPath(const char *path);
	AssetIO openAsset(const char *name, IO::AccessHint access, const char *appName = applicationName);

	// OS UI management (status & navigation bar)
	void setSysUIStyle(uint32_t flags);
	bool hasTranslucentSysUI() const;
	bool hasHardwareNavButtons() const;
	void setSystemOrientation(Orientation);
	Orientation defaultSystemOrientations() const;
	Orientation validateOrientationMask(Orientation oMask) const;

	// Sensors
	void setDeviceOrientationChangeSensor(bool on);

	// vibration support
	bool hasVibrator();
	void vibrate(IG::Milliseconds ms);

	// Notification/Launcher icons
	void addNotification(const char *onShow, const char *title, const char *message);
	void addLauncherIcon(const char *name, const char *path);

	// Power Management
	void setIdleDisplayPowerSave(bool on);
	void endIdleByUserActivity();

	// Permissions
	bool usesPermission(Permission p) const;
	bool requestPermission(Permission p);

	// App exit
	void exit(int returnVal);
	void exit() { exit(0); }
	[[gnu::format(printf, 3, 4)]]
	void exitWithErrorMessagePrintf(int exitVal, const char *format, ...);
	void exitWithErrorMessageVPrintf(int exitVal, const char *format, va_list args);
};

class OnExit
{
public:
	constexpr OnExit() {}
	OnExit(ExitDelegate, ApplicationContext, int priority = APP_ON_EXIT_PRIORITY);
	OnExit(OnExit &&);
	OnExit &operator=(OnExit &&);
	~OnExit();
	ApplicationContext appContext() const;

protected:
	ExitDelegate del{};
	ApplicationContext app{};
};

}