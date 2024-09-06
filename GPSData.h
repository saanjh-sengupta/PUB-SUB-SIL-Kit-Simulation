#ifndef GPSDATA_H
#define GPSDATA_H

#include <vector>
#include <iostream>

#include "silkit/SilKit.hpp"
#include "silkit/services/orchestration/string_utils.hpp"
#include "silkit/util/serdes/Serialization.hpp"

/*
    GpsData is a class which holds data:
        (1) Latitude : Double
        (2) Longitude : Double
        (3) signalQuality : String
*/
class GpsData
{
    private:
    double latitude;
    double longitude;
    int signalQuality;


    public:
    GpsData() = default;
    ~GpsData() = default;
    GpsData(const GpsData& other) = delete;
    GpsData& operator=(const GpsData& other) = delete;
    GpsData(GpsData&& other) = default;
    GpsData& operator=(GpsData&& other) = default;
    

    /*
        Serialize() is a constant function which takes no arguments and returns a vector of unsigned integer of 8 bit. 

        The purpose of this function is to convert the GPS structure into a byte stream (vector of uint8_t);
        making it suitable for transmission.
    */
    std::vector<uint8_t> Serialize() const;
    
    
    /*
        Deserialize is a static function which takes a constant vector of unsigned integer of 8 bit by l-value reference; function returns an instance of the GpsData class. 

        The purpose of this function is to deserialize the vector byte stream into the GPS structure;
        re-coverting it back into the original data container -- transmitted data will be serialized.
    */
    static GpsData Deserialize(const std::vector<uint8_t>& data);


    // Required Getters & Setters
    double getLatitude() const { return latitude; }
    double getLongitude() const { return longitude; }
    int getSignalQuality() const { return signalQuality; }
    void setLatitude(double latitude_) { latitude = latitude_; }
    void setLongitude(double longitude_) { longitude = longitude_; }
    void setSignalQuality(const int &signalQuality_) { signalQuality = signalQuality_; }
};

#endif // GPSDATA_H
