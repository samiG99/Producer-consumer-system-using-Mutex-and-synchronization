# Producer-Consumer Example with FreeRTOS on Arduino

This simple system implements a producer-consumer scenario where producers generate random integers and add them to a shared buffer, while consumers retrieve and process these integers. Synchronization between producers and consumers is achieved using semaphores.

## Components

- **Monitor Structure**: Manages access to the shared buffer through semaphores.
- **Producer Tasks**: Generate random integers and add them to the buffer.
- **Consumer Tasks**: Retrieve integers from the buffer and process them.

## Usage

1. Set up the Arduino environment with FreeRTOS.
2. Upload the code to the Arduino board.
3. Monitor the serial output to observe producer-consumer interactions.

## Sami Almousa
