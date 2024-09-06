#include "GPSData.h"
#include "ParticipantHandler.h"
#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 3)
    {
        std::cerr << "Missing arguments! Start demo with: " << argv[0] << " <ParticipantConfiguration.yaml|json> <ParticipantName> [RegistryUri] [--async]" << std::endl << "Use \"Publisher\" or \"Subscriber\" as <ParticipantName>." << std::endl;
        return -1;
    }
 
    std::string participantName(argv[2]);
 
    if (participantName != "Publisher" && participantName != "Subscriber")
    {
        std::cout << "Wrong participant name provided. Use either \"Publisher\" or \"Subscriber\"." << std::endl;
        return 1;
    }
 
    std::string mediaType{SilKit::Util::SerDes::MediaTypeData()};
    SilKit::Services::PubSub::PubSubSpec gpsDataSpec{"GpsDataTopic", mediaType};
    SilKit::Services::PubSub::PubSubSpec temperatureSpec{"TemperatureTopic", mediaType};
 
    std::string participantConfigurationFilename(argv[1]);
    std::string registryUri = "silkit://localhost:8500";  // Default registry URI
 
    // Check if a registry URI is provided as an argument
    if (argc >= 4)
    {
        registryUri = argv[3];
    }
 
    // Load participant configuration
    auto participantConfiguration = SilKit::Config::ParticipantConfigurationFromFile(participantConfigurationFilename);
 
    // Create the participant with the provided registry URI
    auto participant = SilKit::CreateParticipant(participantConfiguration, participantName, registryUri);
 
    // Constructing the LifecycleConfiguration with the desired OperationMode
    SilKit::Services::Orchestration::LifecycleConfiguration lifecycleConfig{SilKit::Services::Orchestration::OperationMode::Coordinated};
 
    // Pass the LifecycleConfiguration object to CreateLifecycleService
    auto* lifecycleService = participant->CreateLifecycleService(lifecycleConfig);
    ParticipantHandler participantHandler(participantName, lifecycleService);
 
    if (participantName == "Publisher")
    {
        auto* gpsPublisher = participant->CreateDataPublisher("GpsPublisher", gpsDataSpec, 0);
        auto* temperaturePublisher = participant->CreateDataPublisher("TemperaturePublisher", temperatureSpec, 0);
        participantHandler.SetupLifecycleHandlers(gpsPublisher, temperaturePublisher);
    }
    else
    {
        auto* gpsSubscriber = participant->CreateDataSubscriber("GpsSubscriber", gpsDataSpec, &ParticipantHandler::ReceiveGpsData);
        auto* temperatureSubscriber = participant->CreateDataSubscriber("TemperatureSubscriber", temperatureSpec, &ParticipantHandler::ReceiveTemperatureData);
        participantHandler.SetupLifecycleHandlers();
    }
 
    return 0;
}