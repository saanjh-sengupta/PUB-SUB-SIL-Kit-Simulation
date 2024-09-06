#include "GPSData.h"

std::vector<uint8_t> GpsData::Serialize() const
{
    // Individually members are called; data structure is complex
    SilKit::Util::SerDes::Serializer serializer;
    serializer.Serialize(latitude);
    serializer.Serialize(longitude);

    constexpr std::size_t signalQualityBitSize = 32;
    serializer.Serialize(signalQuality, signalQualityBitSize);

    // Releases the serialized data from the buffer and returs as a byte vector.
    return serializer.ReleaseBuffer();
}

GpsData GpsData::Deserialize(const std::vector<uint8_t>& data)
{
    // Creating an instance of the class Gps (return value of the function)
    GpsData gpsData;

    // Deserializing the data into it's actual value
    SilKit::Util::SerDes::Deserializer deserializer(data);
    gpsData.latitude = deserializer.Deserialize<double>();
    gpsData.longitude = deserializer.Deserialize<double>();

    constexpr std::size_t signalQualityBitSize = 32;
    gpsData.signalQuality = deserializer.Deserialize<int>(signalQualityBitSize);

    return gpsData;
}
