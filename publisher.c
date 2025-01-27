#include <stdio.h>
#include <mosquitto.h>

int main() {
    struct mosquitto *mosq = mosquitto_new("publisher", true, NULL);
    if (!mosq) {
        fprintf(stderr, "Failed to create Mosquitto instance.\n");
        return 1;
    }

    mosquitto_connect(mosq, "localhost", 1883, 60);
    mosquitto_publish(mosq, NULL, "MCW/topic", 15 , "Hello Everyone", 0, false);
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
}