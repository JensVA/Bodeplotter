#include "mbed.h"
#include <iostream>
#include "src/bodeplot.h"

SPI SPI_DDS(PC_12, NC, PC_10); // mosi, miso (niet gebruikt, random pin), sclk  
DigitalOut CS_DDS(PA_13, 1);
DigitalOut DDS_AMP_A1(PA_4);
DigitalOut DDS_AMP_A0(PB_0);
DigitalOut IN_AMP_2(PA_12);
DigitalOut IN_AMP_4(PA_11);
DigitalOut IN_AMP_10(PB_12);
DigitalOut IN_AMP_100(PB_11);
DigitalOut CAP_DIS(PC_5);
SPI SPI_ADC(NC, PB_4, PB_3); // mosi (niet gebruikt), miso , sclk
DigitalOut CS_ADC(PA_10);

using namespace std;
using namespace Elektronica;

int main() {
  //initialization:
  Bodeplot bodeplotter;
  bodeplotter.initialize_DDS(&SPI_DDS, &CS_DDS);
  bodeplotter.initialize_ADC(&SPI_ADC, &CS_ADC);
  bodeplotter.initialize_CAP_DIS(&CAP_DIS);
  bodeplotter.initialize_DDS_AMP(&DDS_AMP_A1, &DDS_AMP_A0);
  bodeplotter.initialize_IN_AMP(&IN_AMP_2, &IN_AMP_4, &IN_AMP_10, &IN_AMP_100);
  //-------test-code--------
  bodeplotter.discharge_CAP();
  bodeplotter.set_DDS_AMP(1);
  bodeplotter.set_IN_AMP(1);
  //bodeplotter.setFrequency_DDS(1000);
  //bodeplotter.sweep_DDS(100, 1, 100000);
  bodeplotter.sweep_DDS(200, 15, 200);
  while(1) {
    //bodeplotter.discharge_CAP();
    //wait_us(10000);
    //bodeplotter.sweep_DDS(100, 10, 10000);
    //printf("%fV\n", bodeplotter.getVoltage_ADC());
    //wait_us(10000);
    //wait_us(5000000);
  }
  return 0;
}