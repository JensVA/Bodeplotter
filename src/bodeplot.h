#pragma once

#include "mbed.h"

namespace Elektronica {

  class Bodeplot {

    public:
      void initialize_DDS(SPI * SPI_DDS, DigitalOut * CS_DDS);
      void setFrequency_DDS(float frequency);
      void sweep_DDS(uint32_t steps, float min_frequency, float max_frequency);

      void initialize_DDS_AMP(DigitalOut * DDS_AMP_A1, DigitalOut * DDS_AMP_A0);
      void set_DDS_AMP(int amplification);

      void initialize_IN_AMP(DigitalOut * IN_AMP_2, DigitalOut * IN_AMP_4, DigitalOut * IN_AMP_10, DigitalOut * IN_AMP_100);
      void set_IN_AMP(int amplification);

      void initialize_CAP_DIS(DigitalOut * CAP_DIS);
      void discharge_CAP(void);

      void initialize_ADC(SPI * SPI_ADC, DigitalOut * CS_ADC);
      float getVoltage_ADC(void);

    private:
      SPI * SPI_DDS;
      DigitalOut * CS_DDS;

      SPI * SPI_ADC;
      DigitalOut * CS_ADC;

      DigitalOut * DDS_AMP_A1;
      DigitalOut * DDS_AMP_A0;
      DigitalOut * IN_AMP_2;
      DigitalOut * IN_AMP_4;
      DigitalOut * IN_AMP_10;
      DigitalOut * IN_AMP_100;
      DigitalOut * CAP_DIS;
  };

}