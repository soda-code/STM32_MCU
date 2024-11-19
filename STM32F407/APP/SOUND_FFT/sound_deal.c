#include "./sound/sound_deal.h"
#include "./CMSIS/DSP/Include/arm_math.h"
#include "./BSP/ADC/adc.h"

#define LENGTH 512//信号长度
/******************************************************************
*　一维卷积函数
*
*　说明: 循环卷积,卷积结果的长度与输入信号的长度相同
*
*　输入参数: data[],输入信号; core[],卷积核; cov[],卷积结果;
*             n,输入信号长度; m,卷积核长度.
******************************************************************/
void Covlution(float data[], float core[], float cov[], int n, int m)
{
    int i = 0;
    int j = 0;
    int k = 0;

    //将cov[]清零
    for(i = 0; i < n; i++)
    {
        cov[i] = 0;
    }

    //前m/2+1行
    i = 0;
    for(j = 0; j < m/2; j++, i++)
    {
        for(k = m/2-j; k < m; k++ )
        {
            cov[i] += data[k-(m/2-j)] * core[k];//k针对core[k]
        }

        for(k = n-m/2+j; k < n; k++ )
        {
            cov[i] += data[k] * core[k-(n-m/2+j)];//k针对data[k]
        }
    }

    //中间的n-m行
    for( i = m/2; i <= (n-m)+m/2; i++)
    {
        for( j = 0; j < m; j++)
        {
            cov[i] += data[i-m/2+j] * core[j];
        }
    }

    //最后m/2-1行
    i = (n - m) + m/2 + 1;
    for(j = 1; j < m/2; j++, i++)
    {
        for(k = 0; k < j; k++)
        {
            cov[i] += data[k] * core[m-j-k];//k针对data[k]
        }

        for(k = 0; k < m-j; k++)
        {
            cov[i] += core[k] * data[n-(m-j)+k];//k针对core[k]
        }
    }

}

/******************************************************************
*　一维小波变换函数
*　说明: 一维小波变换,只变换一次
*　输入参数: input[],输入信号; output[],小波变换结果，包括尺度系数和
*　小波系数两部分; temp[],存放中间结果;h[],Daubechies小波基低通滤波器系数;
*　g[],Daubechies小波基高通滤波器系数;n,输入信号长度; m,Daubechies小波基紧支集长度.
******************************************************************/
void DWT1D(float input[], float output[], float temp[], float h[], 
           float g[], int n, int m)
{

    
    int i = 0;
/*
    //尺度系数和小波系数放在一起
    Covlution(input, h, temp, n, m);

    for(i = 0; i < n; i += 2)
    {
        output[i] = temp[i];
    }

    Covlution(input, g, temp, n, m);

    for(i = 1; i < n; i += 2)
    {
        output[i] = temp[i];
    }
*/

    //尺度系数和小波系数分开
    //Covlution(input, h, temp, n, m);

    for(i = 0; i < n; i += 2)
    {
        output[i/2] = temp[i];//尺度系数
    }

    Covlution(input, g, temp, n, m);

    for(i = 1; i < n; i += 2)
    {
        output[n/2+i/2] = temp[i];//小波系数
    }

}
		   
float h[] = {0.332670552950, 0.806891509311, 0.459877502118, -0.135011020010, -0.085441273882, 0.035226291882};
float g[] = {0.035226291882, 0.085441273882, -0.135011020010, -0.459877502118,0.806891509311, -0.332670552950};


float data_adc[LENGTH]={0};//输入信号
float temp_[LENGTH]={0};//中间结果
float data_output[LENGTH]={0};//一维小波变换后的结果
int n = 0;//输入信号长度
int m = 6;//Daubechies正交小波基长度

uint8_t Sound_main(int * data_return  )
{
	int *return_value=data_return;
	data_adc[n] = (float)adc_get_result(ADC_ADCX_CHY);
	n++;
	if(n==512)
	{

		DWT1D(data_adc, data_output, temp_, h, g, n, m);
		memcpy(return_value,data_output,sizeof(data_output));
		n=0;	
		return 1;
	}


	return 0;

}

