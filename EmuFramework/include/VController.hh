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

#pragma once
#include <imagine/util/number.h>
#include <imagine/gfx/GfxSprite.hh>
#include <imagine/base/Base.hh>
#include <imagine/input/DragPointer.hh>
#include <EmuOptions.hh>
#include <EmuSystem.hh>
#include <TurboInput.hh>
#include <cmath>

extern TurboInput turboActions;

class VControllerDPad
{
public:
	Gfx::GCRect padBase;
	IG::WindowRect padBaseArea, padArea;
	int deadzone = 0;
	float diagonalSensitivity = 1.;
	Gfx::Sprite spr;
	uint state = 1;
	Gfx::BufferImage mapImg;
	IG::ManagedPixmap mapPix {PixelFormatRGB565};
	Gfx::Sprite mapSpr;
	bool visualizeBounds = 0;

	constexpr VControllerDPad() {}
	void init();
	void setImg(Gfx::BufferImage &dpadR, Gfx::GC texHeight);
	void draw();
	void setBoundingAreaVisible(bool on);
	int getInput(int cx, int cy);
	IG::WindowRect bounds() const;
	void setPos(IG::Point2D<int> pos);
	void setSize(uint sizeInPixels);
	void setDeadzone(int newDeadzone);
	void setDiagonalSensitivity(float newDiagonalSensitivity);

private:
	int btnSizePixels = 0;

	void updateBoundingAreaGfx();
};

class VControllerKeyboard
{
public:
	Gfx::Sprite spr;
	IG::WindowRect bound;
	uint keyXSize = 0, keyYSize = 0;
	static const uint cols = 10;
	uint mode = 0;

	constexpr VControllerKeyboard() {}
	void init();
	void updateImg();
	void setImg(Gfx::BufferImage *img);
	void place(Gfx::GC btnSize, Gfx::GC yOffset);
	void draw();
	int getInput(int cx, int cy);
};

class VControllerGamepad
{
public:
	// center buttons
	IG::WindowRect centerBtnBound[systemCenterBtns];
	IG::WindowRect centerBtnsBound;
	Gfx::Sprite centerBtnSpr[systemCenterBtns];
	uint centerBtnsState = 1;

	uint lTriggerState = 1;
	uint rTriggerState = 1;

	IG::WindowRect faceBtnBound[systemFaceBtns];
	IG::WindowRect faceBtnsBound, lTriggerBound, rTriggerBound;
	uint faceBtnsState = 1;
	Gfx::Sprite circleBtnSpr[systemFaceBtns];
	VControllerDPad dp;

	bool triggersInline = false;
	uint activeFaceBtns = systemFaceBtns;
	int btnSpacePixels = 0, btnStaggerPixels = 0, btnRowShiftPixels = 0;
	Gfx::GC btnSpace = 0, btnStagger = 0, btnRowShift = 0;//, btnAreaXOffset = 0;
	Gfx::GC btnExtraXSize = 0.001, btnExtraYSize = 0.001, btnExtraYSizeMultiRow = 0.001;
	bool showBoundingArea = false;

	constexpr VControllerGamepad() {}
	void init(float alpha);
	void setBoundingAreaVisible(bool on);
	bool boundingAreaVisible();
	void setImg(Gfx::BufferImage &pics);
	uint rowsForButtons(uint activeButtons);
	void setBaseBtnSize(uint sizeInPixels);
	IG::WindowRect centerBtnBounds() const;
	void setCenterBtnPos(IG::Point2D<int> pos);
	IG::WindowRect lTriggerBounds() const;
	void setLTriggerPos(IG::Point2D<int> pos);
	IG::WindowRect rTriggerBounds() const;
	void setRTriggerPos(IG::Point2D<int> pos);
	void layoutBtnRows(uint a[], uint btns, uint rows, IG::Point2D<int> pos);
	IG::WindowRect faceBtnBounds() const;
	void setFaceBtnPos(IG::Point2D<int> pos);
	void getCenterBtnInput(int x, int y, int btnOut[2]);
	void getBtnInput(int x, int y, int btnOut[2]);
	void draw(bool showHidden);

private:
	Gfx::GC btnSize = 0;
	int btnSizePixels = 0;
};

class VController
{
public:
	static constexpr int C_ELEM = 0, F_ELEM = 8, D_ELEM = 32;
	static constexpr uint TURBO_BIT = IG::bit(31), ACTION_MASK = 0x7FFFFFFF;
	int ptrElem[Input::maxCursors][2] {{0}}, prevPtrElem[Input::maxCursors][2] {{0}};
	#ifdef CONFIG_VCONTROLS_GAMEPAD
	VControllerGamepad gp;
	#endif

	// menu button
	Gfx::Sprite menuBtnSpr;
	IG::WindowRect menuBound;
	uint menuBtnState = 1;

	// fast-forward button
	Gfx::Sprite ffBtnSpr;
	IG::WindowRect ffBound;
	uint ffBtnState = 1;

	float alpha = 0;
	typedef uint Map[D_ELEM+9];
	Map map {0};
	#ifdef CONFIG_VCONTROLLER_KEYBOARD
	VControllerKeyboard kb;
	uint kbMode = 0;
	typedef uint KbMap[40];
	KbMap kbMap {0};
	#else
	static constexpr uint kbMode = 0;
	#endif
	#ifdef CONFIG_BASE_ANDROID
	bool useScaledCoordinates = true;
	#else
	static constexpr bool useScaledCoordinates = false;
	#endif

	constexpr VController() {}
	Gfx::GC xMMSize(Gfx::GC mm);
	Gfx::GC yMMSize(Gfx::GC mm);
	int xMMSizeToPixel(const Base::Window &win, Gfx::GC mm);
	int yMMSizeToPixel(const Base::Window &win, Gfx::GC mm);
	void updateMapping(uint player);
	#ifdef CONFIG_VCONTROLLER_KEYBOARD
	void updateKeyboardMapping();
	#endif
	bool hasTriggers() const;
	void setImg(Gfx::BufferImage &pics);
	void setBoundingAreaVisible(bool on);
	bool boundingAreaVisible();
	void setMenuBtnPos(IG::Point2D<int> pos);
	void setFFBtnPos(IG::Point2D<int> pos);
	void inputAction(uint action, uint vBtn);
	void resetInput(bool init = 0);
	void init(float alpha, uint gamepadBtnSizeInPixels, uint uiBtnSizeInPixels, const Gfx::ProjectionPlane &projP);
	void place();
	#ifdef CONFIG_VCONTROLLER_KEYBOARD
	void toggleKeyboard();
	#endif
	void findElementUnderPos(const Input::Event &e, int elemOut[2]);
	void applyInput(const Input::Event &e);
	void draw(bool emuSystemControls, bool activeFF, bool showHidden = false);
	void draw(bool emuSystemControls, bool activeFF, bool showHidden, float alpha);
	int numElements() const;
	IG::WindowRect bounds(int elemIdx) const;
	void setPos(int elemIdx, IG::Point2D<int> pos);
	void setState(int elemIdx, uint state);
	uint state(int elemIdx);
	void setBaseBtnSize(uint gamepadBtnSizeInPixels, uint uiBtnSizeInPixels, const Gfx::ProjectionPlane &projP);
};

typedef VController SysVController;
extern SysVController vController;
void updateVControllerMapping(uint player, SysVController::Map &map);
#ifdef CONFIG_VCONTROLLER_KEYBOARD
void updateVControllerKeyboardMapping(uint mode, SysVController::KbMap &map);
#endif
