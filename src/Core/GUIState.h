
#pragma once
#ifndef __CORE_GUISTATE_H__
#define __CORE_GUISTATE_H__

#include <stdint.h>

namespace core
{
	class GUIState
	{

		/* ***************************************************** */
		/* ********************** MEMBERS ********************** */
		/* ***************************************************** */
	protected:
		bool		m_bPanLock;
		bool		m_bTurnLock;
		bool		m_bZoomLock;
		int32_t		m_iLockPositionX;
		int32_t		m_iLockPositionY;
		int			m_iCamera;
		double		m_dTime; 
		bool		m_bPlaying;

		/* ***************************************************** */
		/* ************* CONSTUCTION AND DESTUCTION ************ */
		/* ***************************************************** */
	public:
		static GUIState * create_ptr(void);
		static void release_ptr(GUIState * ptr);

	protected:
		GUIState(void);
		~GUIState(void);

		void init(void);
		void release(void);

	private:
		// Disable copy constructor
		GUIState& operator= (const GUIState& p_state){};
		GUIState(const GUIState& p_state){};

		/* ***************************************************** */
		/* ********************** METHODS ********************** */
		/* ***************************************************** */
	public:
	protected:
	private:

		/* ***************************************************** */
		/* *********************** GETTER ********************** */
		/* ***************************************************** */
	public:
		bool isTurnLock(void);
		void setTurnLock(bool b_turnLock);
		bool isZoomLock(void);
		void setZoomLock(bool b_zoomLock);
		bool isPanLock(void);
		void setPanLock(bool b_panLock);
		int32_t getLockPosition(int i_index);
		void setLockPosition(int32_t i_positionX, int32_t i_positionY);

		static const float get_mouse_pan_speed(){ return 0.001f; }
		static const float get_mouse_zoom_speed(){ return 0.05f;  }
		static const float get_mouse_turn_speed(){ return 0.005f;  }
		
	}; // class GUIState

	inline bool GUIState::isTurnLock(void){ return m_bTurnLock; }
	inline bool GUIState::isZoomLock(void){ return m_bZoomLock; }
	inline bool GUIState::isPanLock(void){ return m_bPanLock;  }

}; // namespace Core

#endif //__CORE_GUISTATE_H__
