
#include "GUIState.h"

#include <cstdio>
#include <cassert>

namespace core
{
	GUIState * GUIState::create_ptr(void)
	{
		GUIState * ptr = new GUIState();
		ptr->init();
		return ptr;
	}

	void GUIState::release_ptr(GUIState * ptr)
	{
#ifndef NDEBUG
		assert(ptr != NULL);
#endif
		ptr->release();
		delete ptr;
	}

	GUIState::GUIState(void)
		: m_bPanLock(false)
		, m_bTurnLock(false)
		, m_bZoomLock(false)
		, m_iLockPositionX(0)
		, m_iLockPositionY(0)
	{}


	GUIState::~GUIState(void)
	{}

	void GUIState::init(void)
	{
	}

	void GUIState::release(void)
	{
	}

	void GUIState::setTurnLock(bool b_turnLock)
	{
		m_bTurnLock = b_turnLock;
	}

	void GUIState::setZoomLock(bool b_zoomLock)
	{
		m_bZoomLock = b_zoomLock;
	}

	void GUIState::setPanLock(bool b_panLock)
	{
		m_bPanLock = b_panLock;
	}


	double GUIState::getLockPosition(int32_t i_index)
	{
		if (i_index == 0)
		{
			return m_iLockPositionX;
		}
		else if (i_index == 1)
		{
			return m_iLockPositionY;
		}
		
		return -1;
	}

	void GUIState::setLockPosition(double i_positionX, double i_positionY)
	{
		m_iLockPositionX = i_positionX;
		m_iLockPositionY = i_positionY;
	}

} // namespace Core

