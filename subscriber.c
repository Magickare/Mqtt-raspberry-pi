#include <stdio.h>
#include <mosquitto.h>

void message_callback(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg) {
    printf("Received message on topic '%s': %s\n", msg->topic, (char *)msg->payload);
}

int main() {
    struct mosquitto *mosq = mosquitto_new("subscriber", true, NULL);
    if (!mosq) {
        fprintf(stderr, "Failed to create Mosquitto instance.\n");
        return 1;
    }

    mosquitto_message_callback_set(mosq, message_callback);
    mosquitto_connect(mosq, "localhost", 1883, 60);
    mosquitto_subscribe(mosq, NULL, "MCW/topic", 0);

    mosquitto_loop_forever(mosq, -1, 1);

    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
}