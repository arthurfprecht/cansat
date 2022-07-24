/*
  farmerkeith_BMP280.h - library for the Bosch Sensortec BMP280 pressure and temperature sensor
  created by Keith Hungerford, 15 september 2017
  Updated for bme280 an reorganising code 18 Dec 2018
  Released into the public domain for personal use and not-for-profit projects.
  Leo
*/
#ifndef farmerkeith_BMP280_h
#define farmerkeith_BMP280_h

class bmp280 { // for use with BMP280 sensors
  public:
    // constructors
    bmp280 (uint8_t Addr, uint8_t debug);
    bmp280 (uint8_t Addr);
    bmp280 ();

    // begin and init functions
    bool begin(uint8_t osrs_t, uint8_t osrs_p, uint8_t mode, uint8_t t_sb, uint8_t filter, uint8_t spi3W_en);
    bool begin();                // get calibration factors, set configuration
    bool init();
    // temperature functions
    double readTemperature ();   // returns degrees Celsius
    float getTemperature();      // compatability function, returns degrees Celsius
    double calcTemperature (long rawTemperature); // returns degrees Celsius
    double calcTemperature (long rawTemperature, double &t_fine); // returns degrees Celsius
    // pressure functions
    double readPressure (); // pressure in hPa
    double readPressure (double &temperature); // returns pressure in hPa, temperature in degrees Celsius
    long readRawPressure (long &rawTemperature); // returns raw pressure uncalibrated
    double calcPressure (long rawPressure, double t_fine); // returns pressure in hPa
    uint32_t getPressure(); // compatability function, returns pressure in Pa (1Pa = 0.01 hPa)
    // utility functions
    float calcAltitude (double pressure, float seaLevelhPa); // hPa, returns meters
    float calcAltitude (double pressure); // hPa, standard seaLevelPressure assumed, returns meters
    float calcNormalisedPressure (double pressure, float altitude); // hPa, meters
    // configuration controls
    uint8_t readF4Sleep();           // function
    uint8_t readF5Sleep();           // function
    uint8_t updateF4Control(uint8_t osrs_t, uint8_t osrs_p, uint8_t mode); // function
    uint8_t updateF4Control16xNormal();       // function
    uint8_t updateF4Control16xSleep();        // function
    uint8_t updateF4ControlSleep(uint8_t value); // function
    uint8_t updateF5Config(uint8_t t_sb, uint8_t filter, uint8_t spi3W_en);// function
    uint8_t updateF5Config1msIIR16I2C();      // function
    uint8_t updateF5ConfigSleep(uint8_t value);  // function
    // calibration functions
    void getBmpCalibratonData();            // function
    // general tools
    uint8_t readRegister(uint8_t reg);           // function
    uint8_t updateRegister(uint8_t reg, uint8_t value); // function
    uint8_t readByteArray(uint8_t reg, uint8_t length, uint8_t data[]); // function
    // public data and variables
    uint8_t bmp280Debug = 0;
    // BMP280 I2C address is 0x76(108) when pin SDO is connected to GND
    // or 0x77(109) when pin SDO is connected to Vddio (+3.3V)
    const uint8_t bmp280Addr = 0x76 ; // base address
    uint8_t address;                  // base address + device index
  private:
    // private variables
    uint16_t dig_T1, dig_P1;                // temperature and pressure calibration
    int16_t dig_T2, dig_T3;                 // temperature calibration
    int16_t dig_P2, dig_P3, dig_P4, dig_P5; // pressure calibration
    int16_t dig_P6, dig_P7, dig_P8, dig_P9; // pressure calibration

}; // end of class bmp280

class BMP280 : public bmp280 { // compatability version
  public:
    BMP280 ();
};

class bme280 : public bmp280 {             // for use with bme280 sensors
    // class bme280 inherits all the data and functions of bmp280
  public:
    // constructors
    bme280 (uint8_t Addr, uint8_t debug);
    bme280 (uint8_t Addr);
    bme280 ();
    // begin and init functions
    bool begin();                          // get calibration factors, set configuration
    bool begin(uint8_t osrs_t, uint8_t osrs_p, uint8_t mode, uint8_t t_sb, uint8_t filter, uint8_t spi3W_en, uint8_t osrs_h);
    bool init();
    // humidity functions
    double readHumidity ();                // relative humidity in percent
    double readHumidity (double &temperature, double &pressure);  // RH%, Celsius, hPa
    long readRawHumidity (long &rawTemperature, long &rawPressure); // reads raw humidity, temperature and pressure
    double calcHumidity(long rawHumidity, double t_fine); // convert raw humidity code into %RH
    uint32_t getHumidity();                // for compatability. Relative humidity in percent
    // configuration controls
    void updateF2Control(uint8_t osrs_h);     // function

  private:
    // calibration functions
    void getBmeCalibratonData();           // function
    // variables
    uint8_t  dig_H1, dig_H3;           // humidity calibration
    int16_t  dig_H2, dig_H4, dig_H5;   // humidity calibration
    int8_t   dig_H6;                   // humidity calibration
}; // end of class bme280

class BME280 : public bme280 { // compatability version
  public:
    BME280 ();
};

#endif // for #ifndef farmerkeith_BMP280_h
// end of file
