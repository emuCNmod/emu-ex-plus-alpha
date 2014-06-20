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

#include <imagine/base/GLContext.hh>
#include <imagine/logger/logger.h>

namespace Base
{

CallResult GLContext::init(const GLConfigAttributes &attr)
{
	if(attr.majorVersion() == 1)
		context_ = (void*)CFBridgingRetain([[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES1]);
	else if(attr.majorVersion() == 2)
		context_ = (void*)CFBridgingRetain([[EAGLContext alloc] initWithAPI:kEAGLRenderingAPIOpenGLES2]);
	else
		bug_exit("unsupported OpenGL ES major version: %d", attr.majorVersion());
	assert(context());
	auto success = [EAGLContext setCurrentContext:context()];
	assert(success);
	if(attr.preferredColorBits() <= 16)
	{
		config.useRGB565 = true;
	}
	return OK;
}

GLConfig GLContext::bufferConfig()
{
	return config;
}

void IOSGLContext::setCurrentContext(IOSGLContext *c, Window *win)
{
	if(c)
	{
		auto success = [EAGLContext setCurrentContext:c->context()];
		assert(success);
		c->setCurrentDrawable(win);
	}
	else
	{
		auto success = [EAGLContext setCurrentContext:nil];
		assert(success);
	}
}

void IOSGLContext::setCurrentDrawable(Window *win)
{
	if(win)
		[win->glView() bindDrawable];
}

bool IOSGLContext::isRealCurrentContext()
{
	return [EAGLContext currentContext] == context();
}

void GLContext::present(Window &win)
{
	[context() presentRenderbuffer:GL_RENDERBUFFER];
}

GLContext::operator bool() const
{
	return context_;
}

void GLContext::deinit()
{
	if(context_)
	{
		if(current() == this)
		{
			GLContext::setCurrent(nullptr, nullptr);
		}
		CFRelease(context_);
	}
	*this = {};
}

}
