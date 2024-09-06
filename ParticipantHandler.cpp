#include "ParticipantHandler.h"

ParticipantHandler::ParticipantHandler(const std::string& name,
                                       SilKit::Services::Orchestration::ILifecycleService* lifecycleService)
                : _name(name), _lifecycleService(lifecycleService){}


void ParticipantHandler::SetupLifecycleHandlers(SilKit::Services::PubSub::IDataPublisher* gpsPublisher,
                                                SilKit::Services::PubSub::IDataPublisher* temperaturePublisher)
{
    _lifecycleService->SetStopHandler( []() { std::cout << "Stop handler called" << "\n"; } );
    _lifecycleService->SetShutdownHandler( []() { std::cout << "Shutdown handler called" << "\n"; } );
    _lifecycleService->SetAbortHandler( [](auto lastState) { std::cout << "Abort handler called while in state " << lastState << "\n"; } );

    // Data can be published only when the data for the gpsPublisher and the temperaturePublisher is active
    if (gpsPublisher && temperaturePublisher)
    {
        auto* timeSyncService = _lifecycleService->CreateTimeSyncService();
        
        _lifecycleService->SetCommunicationReadyHandler( [this]() { std::cout << "Communication ready handler called for " << _name << "\n"; } );

        timeSyncService->SetSimulationStepHandler
        (
            [gpsPublisher, temperaturePublisher](std::chrono::nanoseconds now, std::chrono::nanoseconds /*duration*/) 
            {
                auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(now);
                std::cout << "now=" << nowMs.count() << "ms" << "\n";

                PublishData(gpsPublisher, temperaturePublisher);
                
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }, std::chrono::seconds(1)
        );
    }

    // If the caller is the Subscriber; only course of action is to print the data
    else
    {
        auto* timeSyncService = _lifecycleService->CreateTimeSyncService();
        _lifecycleService->SetCommunicationReadyHandler( [this]() { std::cout << "Communication ready handler called for " << _name << "\n"; } );

        timeSyncService->SetSimulationStepHandler
        (
            [](std::chrono::nanoseconds now, std::chrono::nanoseconds /*duration*/) 
            {
                auto nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(now);
                std::cout << "now=" << nowMs.count() << "ms" << "\n";
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }, std::chrono::seconds(1)
        );
    }

    // Starting of the participant lifecycle for the simulation
    auto finalStateFuture = _lifecycleService->StartLifecycle();
    auto finalState = finalStateFuture.get();

    std::cout << "Simulation stopped. Final State: " << finalState << "\n";
    std::cout << "Press enter to stop the process..." << "\n";
    std::cin.ignore();
    }

    void ParticipantHandler::PublishData(SilKit::Services::PubSub::IDataPublisher* gpsPublisher, SilKit::Services::PubSub::IDataPublisher* temperaturePublisher)
    {
        GpsData gpsData;

        // Setting random latitude and longitude values
        gpsData.setLatitude(48.8235 + static_cast<double>((rand() % 150)) / 100000);
        gpsData.setLongitude(9.0965 + static_cast<double>((rand() % 150)) / 100000);
        gpsData.setSignalQuality(1);

        // Serialization of data before transmission
        auto gpsSerialized = gpsData.Serialize();

        // Publishing gpsData
        gpsPublisher->Publish(gpsSerialized);

        std::cout << ">> Published GPS data: latitude=" << gpsData.getLatitude() << ", longitude=" << gpsData.getLongitude() << ", signalQuality=" << gpsData.getSignalQuality() << "\n";
        
        double temperature = 25.0 + static_cast<double>(rand() % 10) / 10.0;

        SilKit::Util::SerDes::Serializer temperatureSerializer;
        temperatureSerializer.Serialize(temperature);

        // Publishing temperature data
        temperaturePublisher->Publish(temperatureSerializer.ReleaseBuffer());
        std::cout << ">> Published temperature data: temperature=" << temperature << "\n"; 
    }

    void ParticipantHandler::ReceiveGpsData(SilKit::Services::PubSub::IDataSubscriber*, const SilKit::Services::PubSub::DataMessageEvent& dataMessageEvent)
    {
        // Convert the data into a vector of unsigned int of 8 bit integer to deserialize it (function requires such data structure)
        auto eventData = SilKit::Util::ToStdVector(dataMessageEvent.data);
        auto gpsData = GpsData::Deserialize(eventData);

        std::cout << "<< Received GPS data: lat=" << gpsData.getLatitude() << ", lon=" << gpsData.getLongitude() << ", signalQuality=" << gpsData.getSignalQuality() << "\n";
    }

    void ParticipantHandler::ReceiveTemperatureData(SilKit::Services::PubSub::IDataSubscriber*, const SilKit::Services::PubSub::DataMessageEvent& dataMessageEvent)
    {
        // Convert the data into a vector of unsigned int of 8 bit integer to deserialize it
        auto eventData = SilKit::Util::ToStdVector(dataMessageEvent.data);
        SilKit::Util::SerDes::Deserializer deserializer(eventData);
        double temperature = deserializer.Deserialize<double>();

        std::cout << "<< Received temperature data: temperature=" << temperature << "\n";
    }
