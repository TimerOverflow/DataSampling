/*********************************************************************************/
/*
 * Author : Jeong Hyun Gu
 * File name : DataSampling.c
*/
/*********************************************************************************/
#include <stdlib.h>
#include "DataSampling.h"
/*********************************************************************************/
#if(DATA_SAMPLING_REVISION_DATE != 20200724)
#error wrong include file. (DataSampling.h)
#endif
/*********************************************************************************/

/*********************************************************************************/
/** Global variable **/

static tS8 *pS8;
static tS16 *pS16;
static tS32 *pS32;
static tS32 Ret;

/*********************************************************************************/
/*
  @brief
    - 버퍼에 데이터 삽입.

  @param
    - Smp : tag_DataSampling 인스턴스의 주소.
    - Idx : 데이터 삽입 위치를 지시하는 index.
    - Data : 삽입할 데이터.

  @retval
    - 없음.
*/
inline static void InToBuf(tag_DataSampling *Smp, tU8 Idx, tS32 Data)
{
  switch(Smp->DataSize)
  {
    case  sizeof(tS8)   :  pS8 = (tS8 *) Smp->Buf; pS8[Idx] = Data; break;
    case  sizeof(tS16)  :  pS16 = (tS16 *) Smp->Buf; pS16[Idx] = Data; break;
    case  sizeof(tS32)  :  pS32 = (tS32 *) Smp->Buf; pS32[Idx] = Data; break;
  }
}
/*********************************************************************************/
/*
  @brief
    - 버퍼에서 특정 index의 데이터를 반환한다.

  @param
    - Smp : tag_DataSampling 인스턴스의 주소.
    - Idx : 데이터 삽입 위치를 지시하는 index.

  @retval
    - 읽은 데이터.
*/
inline static tS32 OutFromBuf(tag_DataSampling *Smp, tU8 Idx)
{
  switch(Smp->DataSize)
  {
    case  sizeof(tS8)   : pS8 = (tS8 *) Smp->Buf; Ret = pS8[Idx]; break;
    case  sizeof(tS16)  : pS16 = (tS16 *) Smp->Buf; Ret = pS16[Idx]; break;
    case  sizeof(tS32)  : pS32 = (tS32 *) Smp->Buf; Ret = pS32[Idx]; break;
  }

  return Ret;
}
/*********************************************************************************/
/*
  @brief
    - 인수로 전달받은 데이터로 버퍼를 채움.

  @param
    - Smp : tag_DataSampling 인스턴스의 주소.
    - Data : 버퍼를 채울 데이터.

  @retval
    - 없음.
*/
static void FillBuffer(tag_DataSampling *Smp, tS32 Data)
{
  tS16 i;

  Smp->Sum = Smp->Index = 0;
  for(i = 0; i < Smp->Level; i++)
  {
    InToBuf(Smp, i, Data);
    Smp->Sum += Data;
  }
}
/*********************************************************************************/
/*
  @brief
    - 'tag_DataSampling' 인스턴스의 필수 초기화 실행.
    - DataSampling 모듈을 사용하기 위해 선행적 실행 필요.

  @param
    - Smp : tag_DataSampling 인스턴스의 주소.
    - BufSize : 동적할당 받을 버퍼의 크기.

  @retval
    - 0 : 초기화 실패.
    - 1 : 초기화 성공.
*/
tU8 DataSamplingInitGeneral(tag_DataSampling *Smp, tS16 BufSize, tS8 DataSize)
{
  if(Smp->Bit.InitGeneral == true)
  {
    return true;
  }

  if((DataSize != sizeof(tS8)) && (DataSize != sizeof(tS16)) && (DataSize != sizeof(tS32)))
  {
    return false;
  }

  Smp->DataSize = DataSize;

  Smp->Buf = malloc(Smp->DataSize * BufSize);

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
  @brief
    - 샘플링 수준(Level) 변경.

  @param
    - Smp : tag_DataSampling 인스턴스의 주소.
    - Level : 변경할 샘플링 수준.

  @retval
    - 없음.
*/
void DataSamplingChangeLevel(tag_DataSampling *Smp, tS16 Level)
{
  if((Smp->Bit.InitGeneral == false) || (Smp->Bit.InitFillBuffer == false))
  {
    return;
  }

  if((Smp->Level != Level) && (1 <= Level) && (Level <= Smp->Level))
  {
    Smp->Level = Level;
    FillBuffer(Smp, OutFromBuf(Smp, 0));
  }
}
/*********************************************************************************/
/*
  @brief
    - ring buffer 형식으로 데이터를 입력 받아 평균을 내어 샘플링.

  @param
    - Smp : tag_DataSampling 인스턴스의 주소.
    - Data : 데이터.

  @retval
    - Result : 샘플링된 데이터.
*/
tS32 DataSamplingGetData(tag_DataSampling *Smp, tS32 Data)
{
  tS32 Result;

  if(Smp->Bit.InitGeneral == false)
  {
    return 0;
  }

  if(Smp->Bit.InitFillBuffer == false)
  {
    Smp->Bit.InitFillBuffer = true;
    FillBuffer(Smp, Data);
  }

  Smp->Sum -= OutFromBuf(Smp, Smp->Index);
  InToBuf(Smp, Smp->Index, Data);
  Smp->Sum += OutFromBuf(Smp, Smp->Index);
  Result = Smp->Sum / Smp->Level;

  if(++(Smp->Index) >= Smp->Level) Smp->Index = 0;

  return Result;
}
/*********************************************************************************/
/*
  @brief
    - 버퍼 초기화 실행.

  @param
    - Smp : tag_DataSampling 인스턴스의 주소.

  @retval
    - 없음.
*/
void DataSamplingResetData(tag_DataSampling *Smp)
{
  if((Smp->Bit.InitGeneral == false) || (Smp->Bit.InitFillBuffer == false))
  {
    return;
  }

  Smp->Bit.InitFillBuffer = false;
}
/*********************************************************************************/
