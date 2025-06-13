#pragma once

#include"Phone.h"

void Phone::swapColorState(ColorState& colorState)
{
	ColorState temp = m_colorState;
	m_colorState = colorState;
	colorState = temp;
}

