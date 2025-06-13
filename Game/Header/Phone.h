#pragma once
#include "Color.h"

class Phone
{
public:

	Phone()
	{
		m_colorState = ColorState::E_INACTIVE;
	}

    const ColorState& getState() const
	{
	    return  m_colorState;
	}
	void setState(const ColorState& state)
	{
		m_colorState = state;
	}
	void swapColorState(ColorState& colorState);

private:

	ColorState	m_colorState;
};