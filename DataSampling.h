/*********************************************************************************/
/*
 * Author : Jeong Hyun Gu
 * File name : DataSampling.h
*/
/*********************************************************************************/
#ifndef __DATA_SAMPLING_H__
#define	__DATA_SAMPLING_H__
/*********************************************************************************/
#define DATA_SAMPLING_REVISION_DATE		20180403
/*********************************************************************************/
/** REVISION HISTORY **/
/*
	2018. 04. 03.					- DataSamplingResetData() 함수 추가.
	Jeong Hyun Gu

	2017. 10. 17.					- 초기버전.
	Jeong Hyun Gu
*/
/*********************************************************************************/
/**Define**/

#define	false				0
#define	true				1
#define null				0

/*********************************************************************************/
/**Enum**/


/*********************************************************************************/
/**Struct**/

typedef struct
{
	struct
	{
		char InitGeneral			:				1;				//필수초기화
		char InitFillBuffer		:				1;				//버퍼초기화
	}Bit;

	int *Buf;
	int Index, BufSize, Level;
	long Sum;
}tag_DataSampling;

/*********************************************************************************/
/**Function**/

char DataSamplingInitGeneral(tag_DataSampling *Smp, int BufSize);
void DataSamplingChangeLevel(tag_DataSampling *Smp, int Level);
int DataSamplingGetData(tag_DataSampling *Smp, int Data);
void DataSamplingResetData(tag_DataSampling *Smp);

/*********************************************************************************/
#endif //__DATA_SAMPLING_H__
