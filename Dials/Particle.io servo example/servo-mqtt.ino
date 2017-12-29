// This #include statement was automatically added by the Particle IDE.
#include "MQTT/MQTT.h"


//Setup MQTT
void callback(char* topic, byte* payload, unsigned int length);

MQTT client("op-en.se", 1883, callback);


//Init servo
Servo serv;

//Handler for MQTT messages.
void callback(char* topic, byte* payload, unsigned int length) {
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    String message(p);

    //Convert the text to an int and set the servo. 
    serv.write(atoi(p));

}


void setup() {
    
    //Servo is on pin D0
    serv.attach(D0);
    //serv.write(0);
    
    // connect to the server
    client.connect("sparkclient");

    // publish/subscribe
    if (client.isConnected()) {
        //Anounce our presense
        client.publish("/device/servo","Connected");
        
        //Subscribe to the data
        client.subscribe("/device/servo");
    }
}


void loop() {
    
    //Handle MQTT messages with the handler. 
    if (client.isConnected())
        client.publish("/test/lightsensor",analogRead(15));
        delay(100);
        client.loop();
        
    
    

}


