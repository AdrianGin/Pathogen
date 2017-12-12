#pragma once


#include "IControl.h"
#include "ControlsManager.h"
#include "IPopupMenu.h"
#include "ISelectionBox.h"
#include "ITextString.h"
#include <algorithm>

class FilterControlManager : public ControlsManager
{

public:
	FilterControlManager(IPlugBase* pPlug, IGraphics* pGraphics, int32_t x, int32_t y, int32_t idx);
	virtual ~FilterControlManager();

	void UpdateText(float cutoff, float res);

	virtual void Update(void);

	IPopupMenu* getModeBox(void) { return FiltModeMenu; }
	IPopupMenu* getOrderBox(void) { return OrderMenu; }

	float getCutoff(void);
	float getResonance(void);

protected:
	
	//Parameter index offsets
	enum {
		DRIVE_IDX = 0,
		CUTOFF_IDX,
		RESONANCE_IDX,
		MODE_IDX,
		ORDER_IDX,
		NUM_PARAMS,
	};

	enum {
		kKnobFrames = 60,
		kKnobHeight = 45,
		kKnobInterval = 48 + 12,
		kFilterMaxF = 16000,
		kFilterMinF = 300,
		kFilterMaxQ = 8,
	};

	enum {
		DRIVE_XOFF = 0,
		DRIVE_YOFF = 0,

		CUTOFF_XOFF = kKnobInterval,
		CUTOFF_YOFF = 0,
		CUTOFF_TXT_XOFF = CUTOFF_XOFF + kKnobHeight / 2,
		CUTOFF_TXT_YOFF = CUTOFF_YOFF + kKnobHeight,

		RESONANCE_XOFF = CUTOFF_XOFF + kKnobInterval,
		RESONANCE_YOFF = 0,
		RESONANCE_TXT_XOFF = RESONANCE_XOFF + kKnobHeight / 2,
		RESONANCE_TXT_YOFF = RESONANCE_YOFF + kKnobHeight,

		MODE_XOFF = RESONANCE_XOFF + kKnobInterval,
		MODE_YOFF = 0,
		MODE_SELLEN = 60,
		
		ORDER_XOFF = MODE_XOFF,
		ORDER_YOFF = 12,
		ORDER_SELLEN = 20,

	};

	enum {
		TEXT_STRING_LEN = 30,
	};
	
	IKnobMultiControl* drive;
	IKnobMultiControl* cutoff;
	IKnobMultiControl* resonance;
	
	IPopupMenu* FiltModeMenu;
	ISelectionBox* FiltModeSelection;

	IPopupMenu* OrderMenu;
	ISelectionBox* OrderSelection;


	IText* detailsText;


	ITextString* CutoffString;
	ITextString* ResonanceString;
};