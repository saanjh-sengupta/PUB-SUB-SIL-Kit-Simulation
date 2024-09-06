#ifndef PARTICIPANTHANDLER_H
#define PARTICIPANTHANDLER_H

#include "GPSData.h"
#include <thread>

// ParticipantHandler Class Definition
/*
        ParticipantHandler is a class definition which is used for implementing all necessary service operations needed to run the simulation. Comprises of the following:
                (1) _name for the Participant Name (Access Mode)
                (2) _lifecycleService for handling the lifecycle of the simulation
*/
class ParticipantHandler
{
    private:
    std::string _name;
    SilKit::Services::Orchestration::ILifecycleService* _lifecycleService;

    public:
    ParticipantHandler() = default;
    ~ParticipantHandler() = default;
    ParticipantHandler(const ParticipantHandler& other) = delete;
    ParticipantHandler& operator=(const ParticipantHandler& other) = delete;
    ParticipantHandler(ParticipantHandler&& other) = default;
    ParticipantHandler& operator=(ParticipantHandler&& other) = default;

    // Parameterized constructor for constructing the class
    ParticipantHandler(const std::string& name, SilKit::Services::Orchestration::ILifecycleService* lifecycleService);
    
    /*
        SetupLifecycleHandlers() is a function which takes two abstract class IDataPublisher pointer instances (default values set to nullptr); function returns void. 

        The purpose of this function is to set and manage the lifecycle of the simulation. 
    */
    void SetupLifecycleHandlers(SilKit::Services::PubSub::IDataPublisher* gpsPublisher = nullptr, SilKit::Services::PubSub::IDataPublisher* temperaturePublisher = nullptr);

    /*
        PublishData() is a static function which takes two abstract class IDataPublisher pointer instances by l-value; function returns void. 

        The purpose of the function is to Publish the data to the Subscriber; after changes in the data is made. 
    */
    static void PublishData(SilKit::Services::PubSub::IDataPublisher* gpsPublisher, SilKit::Services::PubSub::IDataPublisher* temperaturePublisher);
    
    
    /*
        ReceiveGpsData() is a static callback function which takes one abstract IDataSubscriber class (one constant and one non-constant) pointer instance and one structure instance;  all by l-value reference; function returns void. 

        The purpose of the function is to handle any incoming gps data from the Publisher to the Subscriber. 
    */
    static void ReceiveGpsData(SilKit::Services::PubSub::IDataSubscriber* /*subscriber*/, const SilKit::Services::PubSub::DataMessageEvent& dataMessageEvent);

    
    /*
        ReceiveTemparatureData() is a static function which takes one abstract class IDataSubscriber pointer instance and one structure instance; all by l-value reference; function returns void. 

        The purpose of the function is to handle any incoming data for temperature from the Publisher to the Subscriber. 
    */
    static void ReceiveTemperatureData(SilKit::Services::PubSub::IDataSubscriber* /*subscriber*/, const SilKit::Services::PubSub::DataMessageEvent& dataMessageEvent);
};

#endif // PARTICIPANTHANDLER_H
