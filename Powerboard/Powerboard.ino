// This #include statement was automatically added by the Particle IDE.
#include <Stepper.h>
#include <AccelStepper.h>
#include <MQTT.h>
#include "math.h"
#include "application.h"

const int stepsPerRevolution = 200;  // change this to fit the number of steps per revolution
// for your motor
int newStep;
int targetStep;

//These values has to be changed for the different devices. It can be found under det Devices list.
String photonDeviceId = "320048001347343339383037";
String photonDeviceName = "powerArtefact";

// initialize the stepper library on pins D2 through D5:
//Stepper myStepper(stepsPerRevolution, D2, D3, D4, D5);
AccelStepper myStepper;

void callback(char* topic, byte* payload, unsigned int length);



//#####################################################################
//############ EDIT BETWEEN THE HASHTAG LINES #########################
// www.qsco2.se 


char* inputTopic = "/open/powerArtefact/stepperInt";
char* outputTopic = "/open/powerArtefact/chat";


//#####################################################################
// MQTT connection methods (domain or ip)
// byte server[] = { XXX,XXX,XXX,XXX };
byte server[] = { 213,168,249,129 };
// MQTT client(server, 1883, callback);
// want to use domain name,
// MQTT client("www.sample.com", 1883, callback);
MQTT client(server, 1883, callback);


void idHandler(const char *topic, const char *data) {
    String myDeviceID = String(data);
    String myDeviceTopic = String(topic);
}


// recieve message
void callback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    
    String message(p);
    String myTopic(topic);
    
    int myPayload = message.toInt(); // convert message to int
    
    //Publish to the particle log for debugging
    Particle.publish(outputTopic, message);

    //if the incoming topic = /qsco2/lab/poc/percent -> do something
    if (myTopic.equals(inputTopic)) {
        
        Particle.publish(outputTopic, photonDeviceName  + ": " + message);
        targetStep = myPayload; //oldStep = 80, myPayload = 120/ myPayload = 40

        /*
        if (currStep > newStep) {
            
            currStep = newStep - oldStep;
            myStepper.step(currStep);
            oldStep = currStep;
            client.publish(outputTopic, photonDeviceName  + " up:  " + currStep);
            delay(50);
        } else if (newStep < oldStep) {
            
            currStep = oldStep - newStep;
            myStepper.step(-currStep);
            oldStep = currStep;
            client.publish(outputTopic, photonDeviceName  + " down:  " + currStep);
            delay(50);
        }
        */
        
    }
    
}


void setup() {
    
    myStepper.setMaxSpeed(1000);
    myStepper.setSpeed(1000);
    myStepper.setAcceleration(1000);

    // connect to the server
    client.connect(photonDeviceId);

    // publish/subscribe
    if (client.isConnected()) {
        //RGB.color(0, 255, 0);
        client.publish(outputTopic, "hello MQTT this is "  + photonDeviceName);
        client.subscribe(inputTopic);
    } else {
        //RGB.color(255, 0, 0);
    }
    
    
}


void loop() {
    
    if (client.isConnected())
    client.loop();
    myStepper.moveTo(targetStep);
    myStepper.run();
    
}
