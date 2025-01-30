#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg) {
    printf("Received message on topic '%s': %s\n", msg->topic, (char *)msg->payload);
}

int main() {
    mosquitto_lib_init();

    struct mosquitto *mosq = mosquitto_new("mqtt_client", true, NULL);
    if (!mosq) {
        fprintf(stderr, "Error: Unable to create Mosquitto instance.\n");
        return 1;
    }

    mosquitto_message_callback_set(mosq, on_message);

    const char *broker = "broker.hivemq.com";
    int port = 1883;
    if (mosquitto_connect(mosq, broker, port, 60) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error: Unable to connect to broker.\n");
        mosquitto_destroy(mosq);
        return 1;
    }
    printf("Connected to broker %s on port %d\n", broker, port);

    const char *sub_topic = "mcw/topic";
    if (mosquitto_subscribe(mosq, NULL, sub_topic, 0) != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Error: Unable to subscribe to topic.\n");
        mosquitto_destroy(mosq);
        return 1;
    }
    printf("Subscribed to topic: %s\n", sub_topic);

    mosquitto_loop_start(mosq);

    const char *pub_topic = "mcw/topic";
    char message[256];
    printf("Enter messages to publish (type 'exit' to quit):\n");
    while (1) {
        printf("> ");
        if (fgets(message, sizeof(message), stdin) == NULL) {
            break;
        }

        message[strcspn(message, "\n")] = '\0';

        if (strcmp(message, "exit") == 0) {
            break;
        }

        if (mosquitto_publish(mosq, NULL, pub_topic, strlen(message), message, 0, false) != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "Error: Unable to publish message.\n");
        } else {
            printf("Published message: %s\n", message);
        }
    }

    mosquitto_loop_stop(mosq, true);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    printf("Disconnected from broker.\n");
    return 0;
}
