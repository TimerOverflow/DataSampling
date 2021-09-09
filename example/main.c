
#include "DataSampling.h"

#define BUFFER_SIZE        10
#define DATA_SIZE          sizeof(tS16)

tS16 GetTemperature(void)
{
  static tS16 temp;

  temp++;
  //some code write here..

  return temp;
}

tag_DataSampling SmpTemperatrue;
tS16 AvgTemp;
tU8 IsInitOk;

int main( void )
{
  IsInitOk = DataSamplingInitGeneral(&SmpTemperatrue, BUFFER_SIZE, DATA_SIZE);

  while(1)
  {
    if(IsInitOk == true)
    {
      AvgTemp = (tS16) DataSamplingGetData(&SmpTemperatrue, (tS32) GetTemperature());
    }
  }

  return 0;
}
