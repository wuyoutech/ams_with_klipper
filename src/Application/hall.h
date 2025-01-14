#include <gd32f30x.h>
#include <stdbool.h>

struct adc_sample_value {
    uint16_t feeder_hall1;
    uint16_t feeder_hall2;
    uint16_t feeder_hall3;
    uint16_t feeder_hall4;

    uint16_t mux_hall1;
    uint16_t mux_hall2;
    uint16_t mux_hall3;
    uint16_t mux_hall4;
};

enum hall_sensor_channel {
    feeder_hall1,
    feeder_hall2,
    feeder_hall3,
    feeder_hall4,
    mux_hall1,
    mux_hall2,
    mux_hall3,
    mux_hall4
};

void hall_sensor_init(void);

bool hall_sensor_get_status(uint8_t channel_name);