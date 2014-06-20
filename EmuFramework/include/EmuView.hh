#pragma once

/*  This file is part of EmuFramework.

	Imagine is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	Imagine is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with EmuFramework.  If not, see <http://www.gnu.org/licenses/> */

#include <imagine/gfx/GfxSprite.hh>
#include <imagine/gfx/GfxBufferImage.hh>
#include <VideoImageOverlay.hh>
#include <VideoImageEffect.hh>
#include <imagine/gui/View.hh>
#include <EmuOptions.hh>
#include <EmuVideoLayer.hh>
#include <EmuInputView.hh>

class EmuView : public View
{
public:
	EmuVideoLayer *layer = nullptr;
	EmuInputView *inputView = nullptr;

private:
	IG::WindowRect rect;

public:
	constexpr EmuView(Base::Window &win): View(win) {}
	void deinit() override {}
	IG::WindowRect &viewRect() override { return rect; }
	void place() override;
	void draw(Base::FrameTimeBase frameTime) override;
	void inputEvent(const Input::Event &e) override;
};
