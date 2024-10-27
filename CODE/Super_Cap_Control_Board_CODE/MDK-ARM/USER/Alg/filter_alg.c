#include"filter_alg.h"
#include "Cap_Buck_Control.h"

void WMA_filter_Init(WMA_filterdata* pdata)
{
	for(int i=0;i<16;i++)
	{
		pdata->group_data[i]=0;
	}
	pdata->flag_data=0;
	pdata->sum_group_data=0;
}
void kalman_filter_Init(kalman_filterdata* pdata,float last_P,float now_P,float out,float Kg,float Q,float R,float x_last,float x_now)
{
    pdata->last_P = last_P;
    pdata->now_P = now_P;
    pdata->out = out;
	  pdata->Kg = Kg;
    pdata->Q = Q;
    pdata->R = R;
	  pdata->x_last = x_last;
	  pdata->x_now = x_now;
}
float WMA_filter(WMA_filterdata* pdata ,float pvalue) 
{
	int k=8;//位数
	pdata->group_data[pdata->flag_data]=pvalue;
	pdata->flag_data=(pdata->flag_data+1)%k;//标志位+1
	pdata->sum_group_data=0;
	for(int flag=0;flag<k;flag++)//加权取平均
	{
		if((pdata->flag_data-flag+k)%k<(k/2)&&(pdata->flag_data-flag+k)%k>=0)//高4位数据
		{
		pdata->sum_group_data+=pdata->group_data[flag]*1.1f;
		}
		else//低4位数据
		pdata->sum_group_data+=pdata->group_data[flag]*0.9f;
  }
	pdata->value=pdata->sum_group_data/((float)k);
	return pdata->value;
}

float kalman_filter(kalman_filterdata* kfp,float input){
	
	float x_mid = kfp->x_last;
	kfp->now_P = kfp->last_P + kfp->Q;
	kfp->Kg = (kfp->now_P )/(kfp->now_P + kfp->R);
	kfp->x_now = x_mid+kfp->Kg *(input - x_mid);
	//kfp->out = kfp->out + kfp->Kg *(input- kfp->out);
	kfp->last_P = (1-kfp->Kg) * kfp->now_P;
	kfp->out = kfp->x_now;
	kfp->x_last = kfp->x_now;
	return kfp->out;
}


