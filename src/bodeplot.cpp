#include "bodeplot.h"
#include <math.h>

namespace Elektronica {

  void Bodeplot::initialize_DDS(SPI * SPI_DDS, DigitalOut * CS_DDS) {
    this->SPI_DDS = SPI_DDS;
    this->CS_DDS = CS_DDS;
    CS_DDS->write(1);
    SPI_DDS->format(8,2);
    SPI_DDS->frequency(1000000);
  }

  void Bodeplot::setFrequency_DDS(float frequency) {
    uint32_t FREQREG = frequency / 0.05960464477;

    uint16_t LSB_frequency = FREQREG & 0x00003FFF;
    uint16_t MSB_frequency = (FREQREG >> 14) & 0x00003FFF;
    
    CS_DDS->write(0);
    char tx[2] = {0x21, 0x00};
    SPI_DDS->write(tx, 2, nullptr, 0);

    char txLSB[2] = {((LSB_frequency >> 8) + 0x40) , (LSB_frequency & 0x00FF)};
    SPI_DDS->write(txLSB, 2, nullptr, 0);

    char txMSB[2] = {((MSB_frequency >> 8) + 0x40) , (MSB_frequency & 0x00FF)};
    SPI_DDS->write(txMSB, 2, nullptr, 0);

    char txRESET[2] = {0x00, 0x00};
    SPI_DDS->write(txRESET, 2, nullptr, 0);
    CS_DDS->write(1);
  }

  void Bodeplot::sweep_DDS(uint32_t steps, float min_frequency, float max_frequency) {
    float log_range =  log10(max_frequency/min_frequency);
    float min_value = log10(min_frequency);
    for(int i = 0; i <= log_range*steps; i++) {
        float frequency = pow(10, min_value+(i/steps));
        setFrequency_DDS(frequency);
        set_DDS_AMP(2);
        set_IN_AMP(1);
        discharge_CAP();
        wait_us(50000);
        wait_us(5*(1/frequency)*1000000);
        float voltage = getVoltage_ADC();
        float hf = 20 * log10((voltage/2)/0.3);
        printf("%f %f\n", frequency, hf);
    }
  }

  void Bodeplot::initialize_DDS_AMP(DigitalOut * DDS_AMP_A1, DigitalOut * DDS_AMP_A0) {
    this->DDS_AMP_A1 = DDS_AMP_A1;
    this->DDS_AMP_A0 = DDS_AMP_A0;
  }

  void Bodeplot::set_DDS_AMP(int amplification) {
    switch(amplification) {
      case 1:
        DDS_AMP_A1->write(0);
        DDS_AMP_A0->write(0);
        break;
      case 2:
        DDS_AMP_A1->write(0);
        DDS_AMP_A0->write(1);
        break;
      case 4:
        DDS_AMP_A1->write(1);
        DDS_AMP_A0->write(0);
        break;
      case 8:
        DDS_AMP_A1->write(1);
        DDS_AMP_A0->write(1);
        break;
      default:
        DDS_AMP_A1->write(0);
        DDS_AMP_A0->write(0);
    }
  }

  void Bodeplot::initialize_IN_AMP(DigitalOut * IN_AMP_2, DigitalOut * IN_AMP_4, DigitalOut * IN_AMP_10, DigitalOut * IN_AMP_100) {
    this->IN_AMP_2 = IN_AMP_2;
    this->IN_AMP_4 = IN_AMP_4;
    this->IN_AMP_10 = IN_AMP_10;
    this->IN_AMP_100 = IN_AMP_100;
  }

  void Bodeplot::set_IN_AMP(int amplification) {
    switch(amplification) {
      case 1:
        IN_AMP_2->write(0);
        IN_AMP_4->write(0);
        IN_AMP_10->write(0);
        IN_AMP_100->write(0);
        break;
      case 2:
        IN_AMP_2->write(1);
        IN_AMP_4->write(0);
        IN_AMP_10->write(0);
        IN_AMP_100->write(0);
        break;
      case 4:
        IN_AMP_2->write(0);
        IN_AMP_4->write(1);
        IN_AMP_10->write(0);
        IN_AMP_100->write(0);
        break;
      case 8:
        IN_AMP_2->write(1);
        IN_AMP_4->write(1);
        IN_AMP_10->write(0);
        IN_AMP_100->write(0);
        break;
      case 10:
        IN_AMP_2->write(0);
        IN_AMP_4->write(0);
        IN_AMP_10->write(1);
        IN_AMP_100->write(0);
        break;
      case 20:
        IN_AMP_2->write(1);
        IN_AMP_4->write(0);
        IN_AMP_10->write(1);
        IN_AMP_100->write(0);
        break;
      case 40:
        IN_AMP_2->write(0);
        IN_AMP_4->write(1);
        IN_AMP_10->write(1);
        IN_AMP_100->write(0);
        break;
      case 80:
        IN_AMP_2->write(1);
        IN_AMP_4->write(1);
        IN_AMP_10->write(1);
        IN_AMP_100->write(0);
        break;
      case 100:
        IN_AMP_2->write(0);
        IN_AMP_4->write(0);
        IN_AMP_10->write(0);
        IN_AMP_100->write(1);
        break;
      case 200:
        IN_AMP_2->write(1);
        IN_AMP_4->write(0);
        IN_AMP_10->write(0);
        IN_AMP_100->write(1);
        break;
      case 400:
        IN_AMP_2->write(0);
        IN_AMP_4->write(1);
        IN_AMP_10->write(0);
        IN_AMP_100->write(1);
        break;
      case 800:
        IN_AMP_2->write(1);
        IN_AMP_4->write(1);
        IN_AMP_10->write(0);
        IN_AMP_100->write(1);
        break;
      default:
        IN_AMP_2->write(0);
        IN_AMP_4->write(0);
        IN_AMP_10->write(0);
        IN_AMP_100->write(0);
    }
  }

  void Bodeplot::initialize_CAP_DIS(DigitalOut * CAP_DIS) {
    this->CAP_DIS = CAP_DIS;
  }

  void Bodeplot::discharge_CAP(void) {
    CAP_DIS->write(1);
    wait_us(5000); // 5 * tijdsconstante = 5ms → 5000µs wachten
    CAP_DIS->write(0);
  }

  void Bodeplot::initialize_ADC(SPI * SPI_ADC, DigitalOut * CS_ADC) {
    this->SPI_ADC = SPI_ADC;
    this->CS_ADC = CS_ADC;
    CS_ADC->write(1);
    SPI_ADC->format(8,0);   //8 bits + Mode 0
    SPI_ADC->frequency(1000000);
  }

  float Bodeplot::getVoltage_ADC(void) {
    CS_ADC->write(0);

    uint8_t high = SPI_ADC->write(0x00);
    uint8_t low = SPI_ADC->write(0x00);
    
    CS_ADC->write(1);

    high = high & 0x1F;
    low = low >> 1;
    low = low | ((high & 0x01) << 7);
    high = high >> 1;
    
    uint16_t d_out = ( high << 8 ) | low;
    float voltage = (d_out * 3.3) / 4096;

    return voltage;
  }
}