#include"mppt_alg.h"
#include"Cap_Control.h"


mppt_incc_typedef mppt_incc_handle;
//*********** Structure Init Function ****//
 
//*********** Structure Definition ********//

//*********** Structure Init Function ****//
void MPPT_PNO_F_init(mppt_incc_typedef *v)
{
	v->Ipv=0;
	v->Vpv=0;
	v->DeltaPmin=(0.01);
	v->MaxVolt=(25);
	v->MinVolt=15;
	v->Stepsize=(0.1);
	v->VmppOut=20;
	v->DeltaP=0;
	v->PanelPower=0;
	v->PanelPower_Prev=0;
	v->mppt_enable=1;
	v->mppt_first=1;
}
 
//*********** Function Definition ********//
void MPPT_PNO_F_FUNC(mppt_incc_typedef *v)
{
	if (v->mppt_enable==1)
	{
		if (v->mppt_first == 1)
		{
			v->VmppOut= v->Vpv;
			v->mppt_first=0;
			v->PanelPower_Prev=v->PanelPower;
		}
		else
		{
			v->PanelPower= ( v->Vpv* v->Ipv );
			v->DeltaP=v->PanelPower-v->PanelPower_Prev;
			if (v->DeltaP > v->DeltaPmin)
			{
				v->VmppOut=v->Vpv+v->Stepsize;
			}
			else
			{
				if (v->DeltaP < -v->DeltaPmin)
				{
					//v->Stepsize=-v->Stepsize;
					v->VmppOut=v->Vpv-v->Stepsize;
				}
			}
			v->PanelPower_Prev = v->PanelPower;
		}
		if(v->VmppOut < v->MinVolt) v->VmppOut = v->MinVolt;
		if(v->VmppOut > v->MaxVolt) v->VmppOut = v->MaxVolt;
	}
}
void Task_Mppt_Control(void const * argument)
{
	
	MPPT_PNO_F_init(&mppt_incc_handle);
	
	for(ever) {
		mppt_incc_handle.Vpv = Adc_Vcc;
		mppt_incc_handle.Ipv = Adc_Current_In;
		MPPT_PNO_F_FUNC(&mppt_incc_handle);
		
		osDelay(50);
	}
		
}

