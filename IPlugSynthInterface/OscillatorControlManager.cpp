


#include "OscillatorControlManager.h"
#include "resource.h"

OscillatorControlManager::OscillatorControlManager(IPlugBase* pPlug, IGraphics* pGraphics, int32_t x, int32_t y, int32_t idx) :
	ControlsManager(pPlug, pGraphics, x,y,idx)
{


	loopModeMenu = new IPopupMenu();


	loopModeMenu->AddItem("Forward");
	loopModeMenu->AddItem("Reverse");
	loopModeMenu->AddItem("Ping Pong");

	loopModeMenu->AddItem("One Shot");
	loopModeMenu->AddItem("One Shot Loop");
	loopModeMenu->AddItem("One Shot Ping Pong");

	

	wavDisp = new IWaveformDisplayInteractive(pPlug, IRECT(x + WAVEDISP_XOFF, y + WAVEDISP_YOFF, x + WAVEDISP_XOFF + WAVEDISP_XLEN, y + WAVEDISP_YOFF + WAVEDISP_YLEN), -1, 
		&COLOR_BLACK, startParamIdx + START_PT, startParamIdx + LOOP_PT, startParamIdx + END_PT);
	

	loopModeSelection = new ISelectionBox(pPlug, IRECT(	x + LOOPSEL_XOFF, 
														y + LOOPSEL_YOFF, 
														x + LOOPSEL_XOFF + LOOPSEL_XLEN,
														y + LOOPSEL_YOFF + LOOPSEL_YLEN), loopModeMenu, startParamIdx + LOOP_MODE);

	IBitmap NormButton = pGraphics->LoadIBitmap(OSC1_NORMALISE_ID, O1_NORMALISE_FN, 2);
	normaliseButton = new ISwitchControl(pPlug, x + NORM_XOFF, y + NORM_YOFF, startParamIdx + NORMALISE, &NormButton);

}

OscillatorControlManager::~OscillatorControlManager(void)
{
	delete loopModeMenu;
	delete wavDisp;
	delete loopModeSelection;
}



void OscillatorControlManager::AttachControls(IGraphics* pGraphics)
{
	pGraphics->AttachControl(wavDisp);
	pGraphics->AttachControl(loopModeSelection);
	pGraphics->AttachControl(normaliseButton);
}