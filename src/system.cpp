/*
 *  This file is part of KT Game Engine (KTGE).
 *
 *  KTGE is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  KTGE is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <cstdarg>
#include <new>				// std::set_new_handler
#include <OpenLayer.hpp>	// FpsCounter
#include "basictypes.hpp"
#include "display.hpp"
#include "game.hpp"
#include "input.hpp"
#include "system.hpp"

using namespace kt;

namespace kt {
	KTSystem System;
}


static void SystemNewHandler() {
	System.Error("operator new failed");
}


KTSystem::KTSystem() :
	m_bQuit(false),
	m_pGame(NULL)
{

}

KTSystem::~KTSystem() {

}

void KTSystem::Init() {
	std::set_new_handler(SystemNewHandler);

	// Initialize display
	// THIS MUST BE DONE FIRST, as openlayer calls allegro_init
	Display.Init(1280, 800);
	Input.Init();
	
	if (install_sound(DIGI_AUTODETECT, MIDI_NONE/*MIDI_AUTODETECT*/, NULL) < 0) {
		System.Warning("NO SOUND. %s", allegro_error);
		m_bSound = false;
	} else {
		m_bSound = true;
	}

	m_pGame = new kt::Game();
	m_pGame->Init();

	this->Font    = new ol::TextRenderer("data/Neuropol.ttf", 24, 28, Rgba::RED);
	this->BigFont = new ol::TextRenderer("data/Neuropol.ttf", 40, 40);
	
	if (!this->Font->IsValid())
		Error("Could not load font 'data/Neuropol.ttf'");
	if (!this->BigFont->IsValid())
		Error("Big font error");
	
	m_bNeedRender = true;
	m_tsTime      = 0.0f;
	m_rInterval   = 0.0f;
	m_rDesiredFPS = 60.0f;
}


void KTSystem::Shutdown() {
	delete Font;
	delete BigFont;
	
	m_pGame->Shutdown();
	Input.Shutdown();
	Display.Shutdown();

	delete m_pGame;
}


void KTSystem::Print(int x, int y, PCSZ pszFormat, ...) {
	char szText[500];
	va_list arg_ptr;

	va_start(arg_ptr, pszFormat);
	vsprintf(szText, pszFormat, arg_ptr);
	va_end(arg_ptr);

	this->Font->Print(szText, x, y);
}

void KTSystem::PrintBig(int x, int y, PCSZ pszFormat, ...) {
	char szText[500];
	va_list arg_ptr;

	va_start(arg_ptr, pszFormat);
	vsprintf(szText, pszFormat, arg_ptr);
	va_end(arg_ptr);

	this->BigFont->Print(szText, x, y);
}



void KTSystem::Error(PCSZ pszFormat, ...) {
	static bool bFirst = true;
	char szText[500];
	va_list arg_ptr;

	va_start(arg_ptr, pszFormat);
	vsprintf(szText, pszFormat, arg_ptr);
	va_end(arg_ptr);

	if (bFirst) {
		bFirst = false;
		printf("ERROR: %s\n", szText);
		fflush(stdout);
		exit(-1);
	} else {
		printf("Additional error: %s\n", szText);
	}
}


void KTSystem::Warning(PCSZ pszFormat, ...) {
    char szText[500];
    va_list arg_ptr;

    va_start(arg_ptr, pszFormat);
    vsprintf(szText, pszFormat, arg_ptr);
    va_end(arg_ptr);

	printf("WARNING: %s\n", szText);
	fflush(stdout);
}

void KTSystem::Log(PCSZ pszFormat, ...) {
    char szText[500];
    va_list arg_ptr;

    va_start(arg_ptr, pszFormat);
    vsprintf(szText, pszFormat, arg_ptr);
    va_end(arg_ptr);

	// TODO logfile
    printf("LOG: %s\n", szText);
	fflush(stdout);
}



/*
 * Run game logic by ticks.
 */
void KTSystem::Tick() {
	real rInt = ol::FpsCounter::GetDeltaTime();
	m_rInterval += rInt;
	m_tsTime    += rInt;

	if (m_rInterval < 1.0f) {
		rest(2);
	}

	while (m_rInterval >= 1.0f) {
		Input.Tick(m_tsTime);
		m_pGame->Tick(m_tsTime);
		m_rInterval -= 1.0f;
		m_bNeedRender = true;
	}
}


/*
 * Render
 */
void KTSystem::Render() {
	Display.BeginFrame();
	m_pGame->Render();
	Display.EndFrame();
	m_bNeedRender = false;
	
}


/*
 * Tells the system that a repaint is needed.
 */
void KTSystem::NeedRender() {
	m_bNeedRender = true;
}


/*
 * Tells the system to quit the program.
 */
void KTSystem::Quit() {
	m_bQuit = true;
}


/*
 * Runs the program.
 */
void KTSystem::Run() {
	ol::FpsCounter::Start(m_rDesiredFPS);

	do {
		ol::FpsCounter::NewFrameStarted();

		Tick();

		if (m_bNeedRender) {
			Render();
		}

	} while (!m_bQuit);
}


