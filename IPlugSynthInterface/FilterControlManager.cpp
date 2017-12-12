


#include "FilterControlManager.h"
#include "resource.h"

FilterControlManager::FilterControlManager(IPlugBase* pPlug, IGraphics* pGraphics, int32_t x, int32_t y, int32_t idx) :
	ControlsManager(pPlug, pGraphics, x,y,idx)
{


	IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);
	
	drive = new IKnobMultiControl(pPlug, x + DRIVE_XOFF, y + DRIVE_YOFF, startParamIdx + DRIVE_IDX, &knob);
	cutoff = new IKnobMultiControl(pPlug, x + CUTOFF_XOFF, y + CUTOFF_YOFF, startParamIdx + CUTOFF_IDX, &knob);
	resonance = new IKnobMultiControl(pPlug, x + RESONANCE_XOFF, y + RESONANCE_YOFF, startParamIdx + RESONANCE_IDX, &knob);

	controlActiveState = false;

	AddControl(drive);
	AddControl(cutoff);
	AddControl(resonance);

	detailsText = new IText(12, &COLOR_WHITE, 0, IText::kStyleNormal, IText::kAlignCenter);



	FiltModeMenu = new IPopupMenu();
	FiltModeMenu->AddItem("Low Pass");
	FiltModeMenu->AddItem("Band Pass");
	FiltModeMenu->AddItem("High Pass");
	FiltModeMenu->AddItem("Notch");

	FiltModeSelection = new ISelectionBox(pPlug, IRECT(x + MODE_XOFF,
		y + MODE_YOFF,
		x + MODE_XOFF + MODE_SELLEN,
		y + MODE_YOFF + 12), FiltModeMenu, startParamIdx + MODE_IDX);



	OrderMenu = new IPopupMenu();
	OrderMenu->AddItem("-12 dB");
	OrderMenu->AddItem("-24 dB");


	OrderSelection = new ISelectionBox(pPlug, IRECT(x + ORDER_XOFF,
		y + ORDER_YOFF,
		x + ORDER_XOFF + ORDER_SELLEN,
		y + ORDER_YOFF + 12), OrderMenu, startParamIdx + ORDER_IDX);






	AddControl(OrderSelection);
	AddControl(FiltModeSelection);






	IRECT detailsPos = IRECT(getX() + CUTOFF_TXT_XOFF, getY() + CUTOFF_TXT_YOFF, getX() + CUTOFF_TXT_XOFF + 1 , getY() + CUTOFF_TXT_YOFF + 30 );
	CutoffString = new ITextString(pPlug, detailsPos, *detailsText, TEXT_STRING_LEN);

	detailsPos = IRECT(getX() + RESONANCE_TXT_XOFF, getY() + RESONANCE_TXT_YOFF, getX() + RESONANCE_TXT_XOFF + 1, getY() + RESONANCE_TXT_YOFF + 30);


	ResonanceString = new ITextString(pPlug, detailsPos, *detailsText, TEXT_STRING_LEN);

	AddControl(CutoffString);
	AddControl(ResonanceString);

}



FilterControlManager::~FilterControlManager(void)
{
	delete drive;
	delete cutoff;
	delete resonance;

	delete CutoffString;
	delete ResonanceString;

	delete detailsText;
}

void FilterControlManager::Update(void)
{
	UpdateText(getCutoff(), getResonance());
	ControlsManager::Update();
}

void FilterControlManager::UpdateText(float cutoff, float res)
{
	char outputString[20];

	sprintf( &outputString[0], "Cutoff\n%4.2f", cutoff);

	CutoffString->SetString(outputString);

	sprintf(&outputString[0], "Resonance\n%4.2f", res);
	ResonanceString->SetString(outputString);
}


float FilterControlManager::getCutoff(void)
{
	return std::max((double)(kFilterMinF), pow(kFilterMaxF, cutoff->GetValue()));
}

float FilterControlManager::getResonance(void)
{
	return pow(kFilterMaxQ, resonance->GetValue());
}