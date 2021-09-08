/*********************************************************************************/
/*
 * Author : Jeong Hyun Gu
 * File name : DataSampling.c
*/
/*********************************************************************************/
#include <stdlib.h>
#include "DataSampling.h"
/*********************************************************************************/
#if(DATA_SAMPLING_REVISION_DATE != 20171017)
#error wrong include file. (DataSampling.h)
#endif
/*********************************************************************************/
/** Global variable **/


/*********************************************************************************/
/*
	1) 인수
		- Smp : tag_DataSampling 인스턴스의 주소.
		- Data : 버퍼를 채울 데이터.

	2) 반환
		- 없음.

	3) 설명
		- 인수로 전달받은 데이터로 버퍼를 채움.
*/
static void FillBuffer(tag_DataSampling *Smp, int Data)
{
	int i;

	Smp->Sum = Smp->Index = 0;
	for(i = 0; i < Smp->Level; i++)
	{
		Smp->Buf[i] = Data;
		Smp->Sum += Data;
	}
}
/*********************************************************************************/
/*
	1) 인수
		- Smp : tag_DataSampling 인스턴스의 주소.
		- BufSize : 동적할당 받을 버퍼의 크기.

	2) 반환
		- 0 : 초기화 실패.
		- 1 : 초기화 성공.

	3) 설명
		- 'tag_DataSampling' 인스턴스의 필수 초기화 실행.
		- DataSampling 모듈을 사용하기 위해 선행적 실행 필요.
*/
char DataSamplingInitGeneral(tag_DataSampling *Smp, int BufSize)
{
	if(Smp->Bit.InitGeneral == true)
	{
		return true;
	}

	Smp->Buf = (int *) malloc(sizeof(int) * BufSize);

	if(Smp->Buf != null)
	{
		Smp->Level = Smp->BufSize = BufSize;
		Smp->Sum = Smp->Index = 0;
		Smp->Bit.InitGeneral = true;
	}

	return Smp->Bit.InitGeneral;
}
/*********************************************************************************/
/*
	1) 인수
		- Smp : tag_DataSampling 인스턴스의 주소.
		- Level : 변경할 샘플링 수준.

	2) 반환
		- 없음.

	3) 설명
		- 샘플링 수준(Level) 변경.
*/
void DataSamplingChangeLevel(tag_DataSampling *Smp, int Level)
{
	if((Smp->Bit.InitGeneral == false) || (Smp->Bit.InitFillBuffer == false))
	{
		return;
	}

	if((Smp->Level != Level) && (1 <= Level) && (Level <= Smp->Level))
	{
		Smp->Level = Level;
		FillBuffer(Smp, Smp->Buf[0]);
	}
}
/*********************************************************************************/
/*
	1) 인수
		- Smp : tag_DataSampling 인스턴스의 주소.
		- Data : 데이터.

	2) 반환
		- Result : 샘플링된 데이터.

	3) 설명
		- ring buffer 형식으로 데이터를 입력 받아 평균을 내어 샘플링.
*/
int DataSamplingGetData(tag_DataSampling *Smp, int Data)
{
	int Result;

	if(Smp->Bit.InitGeneral == false)
	{
		return 0;
	}

	if(Smp->Bit.InitFillBuffer == false)
	{
		Smp->Bit.InitFillBuffer = true;
		FillBuffer(Smp, Data);
	}

	Smp->Sum -= Smp->Buf[Smp->Index];
	Smp->Buf[Smp->Index] = Data;
	Smp->Sum += Smp->Buf[Smp->Index];
	Result = Smp->Sum / Smp->Level;

	if(++(Smp->Index) >= Smp->Level) Smp->Index = 0;

	return Result;
}
/*********************************************************************************/
